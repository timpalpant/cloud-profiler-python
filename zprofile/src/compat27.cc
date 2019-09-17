// Copyright 2019 Timothy Palpant
// Adapted from a similar implementation in vmprof (MIT).

#include "compat27.h"

#include <pythread.h>

#include <atomic>
#include <csetjmp>
#include <csignal>

static std::atomic_flag spinlock = ATOMIC_FLAG_INIT;
static std::jmp_buf restore_point;

// Iterate through all known thread states in the main interpreter
// until we find the one for this thread.
static PyThreadState *FindThreadState() {
  auto thread_id = PyThread_get_thread_ident();
  auto istate = PyInterpreterState_Head();
  if (istate == nullptr) {
    return nullptr;
  }

  while (istate != nullptr) {
    auto tstate = PyInterpreterState_ThreadHead(istate);
    while (tstate != nullptr) {
      if (tstate->thread_id == thread_id) {
        return tstate;
      }

      tstate = PyThreadState_Next(tstate);
    }

    istate = PyInterpreterState_Next(istate);
  }

  return nullptr;
}

static void SegfaultHandler(int arg) { std::longjmp(restore_point, SIGSEGV); }

// The API functions we use are inherently unsafe because they require holding
// a lock (HEAD_LOCK) that is not exposed. As a result, we install a segfault
// handler before doing our racy reads. If we're unable to find the thread state
// then the sample will be skipped.
//
// This hack is courtesy of the very clever authors of vmprof.
//
// See:
//   - https://bugs.python.org/issue1021318
//   - https://bugs.python.org/issue26461
PyThreadState *UnsafeGetThisThreadState() {
  PyThreadState *result;

  while (spinlock.test_and_set(std::memory_order_acquire));
  auto prevhandler = std::signal(SIGSEGV, &SegfaultHandler);
  int fault_code = setjmp(restore_point);
  if (fault_code == 0) {
    result = FindThreadState();
  } else {
    result = nullptr;
  }
  std::signal(SIGSEGV, prevhandler);
  spinlock.clear(std::memory_order_release);

  return result;
}
