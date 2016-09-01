Installation
============

Installation is done using CMake. Run

    cmake <path-to-source>
    make install

in the directory in which you indent to set up the library. In-tree builds are
supported (supply `.` as the source directory).

The examples can also be build using CMake. Once you have initialized the build
environment, run `make examples`. `make pdfdoc` builds this documentation. This
functionality is, however, only availible if the build system was initialized
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


