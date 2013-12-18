#!/usr/bin/python /usr/bin/scons
# -*- coding: utf-8 -*-

import os
import SCons.Errors
import pprint
import sys
import platform
from site_scons import SCutils
from multiprocessing import cpu_count



###############################################################
# User options:
#
# Disable colorized output and produce full compiler messages
#
SCutils.add_option('noauto_j', 'disable automatic num_jobs detection', 0)
SCutils.add_option('verbose', 'verbose build', 0)
SCutils.add_option('colorblind', 'no colors', 0)
SCutils.add_option('build', 'specify build=(debug|release)', 1)
SCutils.add_option('ccache', 'ccache=(yes|no)', 1)
SCutils.add_option('gprof_profile', "compile with profiling support", 0)

# Auto -j
if not SCutils.has_option('noauto_j'):
    PARALLELISM = int(cpu_count() * 1.15)
    print 'Autoselecting number of jobs: {0} (use --noauto_j to disable)'.format(PARALLELISM)
    SetOption('num_jobs', PARALLELISM)

else:
    PARALLELISM = 1


###############################################################

cppdefines = []
ccflags = [
    '-Wall',
    '-Winvalid-pch',
    '-Werror',
]

cxxflags = [
    '-std=c++11',
    '-Wnon-virtual-dtor'

]
linkflags = [
    '-rdynamic'
]
cpppath = [
    Dir('src/')
]
libs = []
libpath = []


if SCutils.has_option('gprof_profile'):
    ccflags.append('-pg')
    linkflags.append('-pg')

for k, v in ARGLIST:
    if k == 'ccflag':
        ccflags.append(v)
        print '*****************\nAdding custom ccflag: {0}\nlist of current ccflags: '.format(v), ccflags,\
            '\n****************\n'

    elif k == 'define':
        cppdefines.append(v)
        print '****************\nAdding custom define: {0}\nlist of current defines: '.format(v), cppdefines,\
            '\n****************\n'

    elif k == 'cxxflag':
        cxxflags.append(v)
        print '****************\nAdding custom cxxflag: {0}\nlist of current cxxflags: '.format(v), cxxflags,\
            '\n****************\n'


############################################################################
# Configure debug or release build environment
#
build = GetOption('build')
if not build:
    build = 'debug'

#
# DEBUG
#
if build == 'debug':
    cppdefines.append('DEBUG')
    ccflags.extend([
        '-O0',
        '-ggdb2'
    ])

elif build == 'release':
    ccflags.extend([
        '-O3'
    ])
else:
    raise SCons.Errors.StopError('build {0} not supported'.format(build))

env = Environment(
    platform = 'posix',
    CCFLAGS=ccflags,
    CXXFLAGS=cxxflags,
    CPPDEFINES=cppdefines,
    LINKFLAGS=linkflags,
    CPPPATH=cpppath,
    LIBS=libs,
    LIBPATH=libpath)
#    tools=['default', 'gch'],
#    toolpath='.')

env.Decider('MD5-timestamp')

############################################################################

############################################################################
# Build verbosity
if not SCutils.has_option('verbose'):
    SCutils.setup_quiet_build(env, True if SCutils.has_option('colorblind') else False)
############################################################################


############################################################################
# Look if ccache available or is explicitly disabled
if (GetOption('ccache')  and GetOption('ccache') == 'yes') or (SCutils.which('ccache') and not GetOption('ccache') == 'no'):
    env['CC'] ='ccache gcc'
    env['CXX'] ='ccache g++'

############################################################################





#BOOST_PYTHON_INC = Dir('/usr/include/python3.3m')

env.ParseConfig('pkg-config --cflags --libs protobuf')
env.Append(LIBS = [ 'boost_system'])

BOOST_PYTHON_LIB = 'boost_python-py33'
pyenv = env.Clone(SHLIBPREFIX='')
#pyenv.Append(CPPDEFINES=['EXPORT_PYTHON_INTERFACE'], CPPPATH=[BOOST_PYTHON_INC], SHLIBPREFIX='', LIBS=[BOOST_PYTHON_LIB])
pyenv.Append(CPPDEFINES=['EXPORT_PYTHON_INTERFACE'], LIBS=[BOOST_PYTHON_LIB])
pyenv.ParseConfig('pkg-config --cflags --libs python-3.3')

unit_test_env = env.Clone()
unit_test_env.Append(LIBS=['boost_unit_test_framework'])


binaries = SConscript('src/SConscript', exports=['env', 'pyenv', 'unit_test_env'], variant_dir = 'build/{}'.format(build), duplicate=0)

# Tests
t = Alias('test', 'build/{}/test'.format(build), 'build/{}/test'.format(build))
AlwaysBuild(t)
#Default(t)

