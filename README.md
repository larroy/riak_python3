riak_python3
============

A native riak python3 client


scons --build=release

or

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

