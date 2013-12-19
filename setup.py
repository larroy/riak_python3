#!/usr/bin/env python

from distutils.core import setup
#from distutils.command.build_py import build_py as _build_py
import distutils.cmd
from site_scons import pbs

from sys import version_info


# Actually it could work on Python2 but there's already a good riak library
assert (version_info[0] >= 3), ('This requires Python 3')


class Build_py_cmd(distutils.cmd.Command):
    def initialize_options(self):
        pass

    def finalize_options(self):
        pass

    def run(self):
        print("Calling SCons to build the module")
        pbs.scons()

class Clean_cmd(distutils.cmd.Command):
    user_options = []
    def initialize_options(self):
        pass

    def finalize_options(self):
        pass

    def run(self):
        pbs.scons('-c')


class Check_cmd(distutils.cmd.Command):
    user_options = []
    def initialize_options(self):
        pass

    def finalize_options(self):
        pass

    def run(self):
        pbs.scons('test')




setup(name = 'riak3k',
      packages = ['riak3k'],
      py_modules = ['build/release/riak3k.so'],
      package_dir = {'': 'build/release'},
      cmdclass = {
        'build_py': Build_py_cmd,
        'clean': Clean_cmd,
        'check': Check_cmd
      },
      ext_package = 'riak3k',
      version = '0.1',
      description = 'Riak module for python3',
      author = 'Pedro Larroy',
      author_email = 'pedro.larroy.lists@gmail.com',
      license = 'Apache License 2.0',
      keywords = 'riak python3',
      url='https://github.com/larroy/riak_python3',
      classifiers = [
        'Development Status :: 2 - Pre-Alpha',
        'Intended Audience :: Developers',
        'Operating System :: MacOS :: MacOS X',
        'Operating System :: Microsoft :: Windows',
        'Operating System :: POSIX',
        'Topic :: System :: Networking',
        'Topic :: Database',
        'Topic :: Internet'
      ]
)
