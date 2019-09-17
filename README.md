# zprofile

Statistical (sampling) CPU and wall-clock profilers for Python, derived from [google-cloud-profiler](https://github.com/GoogleCloudPlatform/cloud-profiler-python).

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
