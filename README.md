# zprofile

Statistical (sampling) CPU and wall-clock profilers for Python, derived from [google-cloud-profiler](https://github.com/GoogleCloudPlatform/cloud-profiler-python).
=======
# Google Cloud Python profiling agent

Python profiling agent for
[Google Cloud Profiler](https://cloud.google.com/profiler/).

See
[Google Cloud Profiler profiling Python code](https://cloud.google.com/profiler/docs/profiling-python)
for detailed documentation.

## Supported OS

Linux. Profiling Python applications is supported for Linux kernels whose
standard C library is implemented with `glibc` or with `musl`. For configuration
information specific to Linux Alpine kernels, see
[Running on Linux Alpine](https://cloud.google.com/profiler/docs/profiling-python#running_with_linux_alpine).

## Supported Python Versions

Python >= 3.7 and <= 3.11

## Installation & usage

1.  Install the profiler package using PyPI:

    ```shell
    pip3 install zprofile
    ```

2.  Enable the profiler in your application:

    ```python
    from zprofile.cpu_profiler import CPUProfiler

    p = CPUProfiler()
    pprof = p.profile(30)  # seconds

    with open("profile.pprof", "wb") as f:
      f.write(pprof)
    ```

3.  View the profile with the [`pprof` tool](https://github.com/google/pprof):

    ```
    $ go tool pprof -http localhost:8080 profile.pprof
    ```
