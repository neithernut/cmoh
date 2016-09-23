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

As this is a header-only library, no linking is required. A pkg-config-file
`cmoh.pc` is installed along with the library. If you are not using
`pkg-config`, add `<installation_prefix>/include` to your include directories.


Dependencies
------------

CMOH makes heavy use of templates and C++14 features. Hence, a C++14 compliant
tool chain is required. We recommend using clang-3.8 or gcc-5.3 or later.
Additionally, the following packages are required for installation:
 * `make`
 * `sh`
 * a number of smaller packages, which are usually part of a regular
   installation on unixoid system

If you obtained CMOH without a pre-generated `configure`-script, you may also
require
 * `autoconf`
 * `automake`

Installation
------------

Our build/installation setup is based on the autotools.
If you obtained CMOH without a pre-generated `configure` script, run

    aclocal
    autoconf
    automake --add-missing

on a machine on which the autotools are installed. If the procedure was
successful, the project's root directory will contain a `configure` script.
If the project root contains a `configure` script, you can proceed by running

    ./configure
    make install

in the project's root directory.

Additional information regarding build targets and the installation process can
be found in the user documentation.


Licensing
---------

This library is licensed under the MIT license. Have a look at the LICENSE file
for more information.


