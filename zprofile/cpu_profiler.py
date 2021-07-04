# Copyright 2018 Google LLC with subsequent modifications.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
"""CPU time profiler."""

import logging
from zprofile import _profiler
from zprofile import builder

_NANOS_PER_SEC = 1000 * 1000 * 1000

logger = logging.getLogger(__name__)


class CPUProfiler(object):
  """CPU time profiler.

  The profiler collects CPU time usage data and builds the data as
  a gzip-compressed profile proto.
  """

  def __init__(self, period_ms=10):
    """Constructs the CPU time profiler.

    Args:
      period_ms: An optional integer specifying the sampling interval in
        milliseconds. Defaults to 10.
    """
    self._profile_type = 'CPU'
    self._period_ms = period_ms

  def profile(self, duration_secs):
    """Profiles the CPU time usage for the given duration.

    Args:
      duration_secs: An float specifying the duration to profile in seconds.

    Returns:
      A bytes object containing gzip-compressed profile proto.
    """
    duration_ns = int(_NANOS_PER_SEC * duration_secs)
    traces = self._profile(duration_ns)
    return self._build_profile(duration_ns, traces)

  def _profile(self, duration_ns):
    return _profiler.profile_cpu(duration_ns, self._period_ms)

  def _build_profile(self, duration_ns, traces):
    profile_builder = builder.Builder()
    profile_builder.populate_profile(traces, self._profile_type, 'nanoseconds',
                                     self._period_ms * 1000 * 1000, duration_ns)
    return profile_builder.emit()
