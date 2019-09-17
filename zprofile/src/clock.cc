// Copyright 2018 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "clock.h"

namespace {
Clock DefaultClockInstance;
}  // namespace

Clock *DefaultClock() { return &DefaultClockInstance; }

struct timespec TimeAdd(const struct timespec t1, const struct timespec t2) {
  struct timespec t = {t1.tv_sec + t2.tv_sec, t1.tv_nsec + t2.tv_nsec};
  if (t.tv_nsec > kNanosPerSecond) {
    t.tv_sec += t.tv_nsec / kNanosPerSecond;
    t.tv_nsec = t.tv_nsec % kNanosPerSecond;
  }
  return t;
}

bool TimeLessThan(const struct timespec &t1, const struct timespec &t2) {
  return (t1.tv_sec < t2.tv_sec) ||
         (t1.tv_sec == t2.tv_sec && t1.tv_nsec < t2.tv_nsec);
}

struct timespec NanosToTimeSpec(int64_t nanos) {
  time_t seconds = nanos / kNanosPerSecond;
  int32_t nano_seconds = nanos % kNanosPerSecond;
  return timespec{seconds, nano_seconds};
}

void TimeSub(struct timespec *ts_out,
             const struct timespec *ts_in) {
    /* out = in - out,
       where in > out
     */
    ts_out->tv_sec = ts_in->tv_sec - ts_out->tv_sec;
    ts_out->tv_nsec = ts_in->tv_nsec - ts_out->tv_nsec;
    if (ts_out->tv_sec < 0) {
        ts_out->tv_sec = 0;
        ts_out->tv_nsec = 0;
    } else if (ts_out->tv_nsec < 0) {
        if (ts_out->tv_sec == 0) {
            ts_out->tv_sec = 0;
            ts_out->tv_nsec = 0;
        } else {
            ts_out->tv_sec = ts_out->tv_sec - 1;
            ts_out->tv_nsec = ts_out->tv_nsec + kNanosPerSecond;
        }
    } else {}
}

// Emulate clock_nanosleep for CLOCK_MONOTONIC and TIMER_ABSTIME
// Works on POSIX and OS X (where clock_nanosleep is not available).
// Adapted from ChisholmKyle/PosixMachTiming (ISC).
int clock_nanosleep_abstime(const struct timespec *req) {
    struct timespec ts_delta;
    int retval = clock_gettime(CLOCK_MONOTONIC, &ts_delta);
    if (retval == 0) {
        TimeSub(&ts_delta, req);
        retval = nanosleep(&ts_delta, NULL);
    }
    return retval;
}
