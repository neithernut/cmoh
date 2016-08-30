CMOH: C++ Meta Object Helpers
=============================

This is a header-only library providing an unintrusive meta object facility for
the C++ language (requires at least C++14).

Using this library, one can formally declare properties and specify how those
are accessed in concrete C++ types using "accessors". Multiple accessors
operating on one C++ type can be bundled together in an "accessor bundle"
which provides a uniform way of accessing an object's property based on the
aforementioned attribute declarations.

Such a bundle may then be used by other libraries to access objects' properties
and thus provide serializing, parsing or remote method invocation based on a
formal interface specification.

This library is still heavily in development. Proper documentation will follow
at some point. For now, have a look at the examples to get an idea of how things
are done from a user's point of view.


Using this library
------------------

As this is a header-only library, no linking is required. Both a CMake module
and a pkg-config-file are installed along with the header files. The library is
called "cmoh" in both cases. If you are using neither CMake nor pkg-config-based
setups, add `<installation_prefix>/include` to your include directories.


Dependencies
------------

CMOH makes heavy use of templates and C++14 features. Hence, a C++14 compliant
tool chain is required. We recommend using clang-3.8 or gcc-5.3 or later.


Installation
------------

Installation is done using CMake. Run

```
cmake <path-to-source>
make install
```
in the directory in which you indent to set up the library. In-tree builds are
supported (supply `.` as the source directory).

The examples can also be build using CMake. Once you have initialized the build
environment, run `make examples`.


Licensing
---------

This library is licensed under the MIT license. Have a look at the LICENSE file
for more information.


