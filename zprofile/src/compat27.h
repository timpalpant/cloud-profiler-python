// Copyright 2019 Timothy Palpant

#ifndef CLOUDPROFILER_SRC_COMPAT27_H_
#define CLOUDPROFILER_SRC_COMPAT27_H_

#include <Python.h>

// Best effort replacement for PyGILState_GetThisThreadState on Python <3.2.
// We can't use the one provided by CPython because it uses the custom TLS
// implementation with a mutex (keymutex) that is not safe to call from a
// signal handler (may deadlock).
//
// This implementation is not totally safe but is the best that can be done.
PyThreadState *UnsafeGetThisThreadState();

#endif  // CLOUDPROFILER_SRC_COMPAT27_H_
