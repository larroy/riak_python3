riak_python3
============

This repository provides the following software:

- A native Riak module for Python 
- An easy C++ library for Riak (see src/riak_client/cxx/easy.hpp) 


Depends on: SCons, protobuf, boost-python


To build the python module:

    python3 setup.py build



To get a debug build:

    scons --build=debug

Creates binary targets in build/..

Other targets:

scons test
scons --help


Example of use:

>>> c = riak3k.Client("localhost", "8086")
>>> c.put("omg", "lala", "yes")
>>> c.fetch_value("omg", "lala")
'yes'

