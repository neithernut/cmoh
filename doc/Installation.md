Installation
============

Installation is done using [CMake](http://cmake.org/). Generally, this is a two
step process. CMake generates a build system, which then can be used for
building and/or installing the software package. In this document, we assume
that the default generator is the UNIX Makefile generator on your platform.
Generators can be manually selected by invoking CMake with the `-G` parameter
when initializing the build system.

To perform set-up and installation, run

    cmake <path-to-source>
    make install

in the directory in which you indent to set up the library. In-tree builds are
supported (supply `.` as the source directory).

The examples can also be build using CMake. Once you have initialized the build
environment, run `make examples`. `make pdfdoc` builds this documentation. This
functionality is, however, only available if the build system was initialized
with [pandoc](http://pandoc.org/) installed.


Testing
-------

The build system comes with some generic sanity checks. Also the examples do
serve as tests. The tests are run by the `test` target. Due to a bug in CMake,
the tests have to be build manually before being invoked. Run

    make selfsufficiency
    make examples
    make test

to run the tests.


