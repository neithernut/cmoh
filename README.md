CMOH: C++ Meta Object Helpers
=============================

This is a header-only library providing an unintrusive meta object facility for
the C++ language (requires at least C++14). It provides utilities for declaring
abstract properties and binding them to properties of concrete C++ types. CMOH
is designed to be used as a base for by (de)serialization and RPC libraries.

With CMOH, you can
 * declare abstract properties (e.g. attributes),
 * specify how these properties are accessed for a specific C++ type and
 * bundle this information, giving you access to the properties by a key of
   user-defined type.

Currently, CMOH is in early development. However, a preliminary user
[user documentation](doc/README.md) does exist, as well as examples.


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

Installation is done using [CMake](http://cmake.org). Run

```
cmake <path-to-source>
make install
```
in the directory in which you indent to set up the library. In-tree builds are
supported (supply `.` as the source directory).

The examples can also be build using CMake. Once you have initialized the build
environment, run `make examples`. Additional information regarding build targets
and the installation process can be found in the user documentation.


Licensing
---------

This library is licensed under the MIT license. Have a look at the LICENSE file
for more information.


