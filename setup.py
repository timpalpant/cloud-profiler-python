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
"""Cloud Profiler Python agent packaging script."""

from __future__ import print_function

import glob
import re
import sys
from setuptools import Extension
from setuptools import setup

install_requires = [
    'protobuf',
]

ext_module = [
    Extension(
        'zprofile._profiler',
        sources=glob.glob('zprofile/src/*.cc'),
        include_dirs=['zprofile/src'],
        language='c++',
        extra_compile_args=['-std=c++11'],
        extra_link_args=['-std=c++11', '-static-libstdc++'])
]

if not (sys.platform.startswith('linux') or sys.platform.startswith('darwin')):
  print(
      sys.platform, 'is not a supported operating system.\n'
      'Profiler Python agent modules will be installed but will not '
      'be functional. Refer to the documentation for a list of '
      'supported operating systems.\n')
  ext_module = []


def get_version():
  """Read the version from __version__.py."""

  with open('zprofile/__version__.py') as fp:
    # Do not handle exceptions from open() so setup will fail when it cannot
    # open the file
    line = fp.read()
    version = re.search(r"^__version__ = '([0-9]+\.[0-9]+(\.[0-9]+)?-?.*)'",
                        line, re.M)
    if version:
      return version.group(1)

  raise RuntimeError(
      'Cannot determine version from zprofile/__init__.py.')


setup(
    name='zprofile',
    description='Statistical CPU and wall-clock profilers for Python',
    long_description=open('README.md').read(),
    long_description_content_type='text/markdown',
    url='https://github.com/timpalpant/cloud-profiler-python',
    author='Timothy Palpant',
    version=get_version(),
    install_requires=install_requires,
    setup_requires=['wheel'],
    packages=['zprofile'],
    ext_modules=ext_module,
    license='Apache License, Version 2.0',
    keywords='cloud profiler',
    classifiers=[
        'Development Status :: 4 - Beta',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: Apache Software License',
        'Programming Language :: Python :: 2.7',
        'Programming Language :: Python :: 3.2',
        'Programming Language :: Python :: 3.3',
        'Programming Language :: Python :: 3.4',
        'Programming Language :: Python :: 3.5',
        'Programming Language :: Python :: 3.6',
        'Programming Language :: Python :: 3.7',
    ])
