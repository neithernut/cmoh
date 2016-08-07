CMOH: C++ Meta Object Helpers
=============================

This is a header-only library providing an unintrusive meta object facility for
the C++ language (requires at least C++11).

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

As this is a header-only library, no linking is required. Make sure you add
`<installation_prefix>/include` to your include directories. Support for CMake
and pkg-config will follow at some point.


Dependencies
------------

CMOH makes heavy use of templates and C++11 features. Hence, a C++11 compliant
tool chain is required.


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
for more information. The following paragraphs are a rationale for the choice of
the license by the original author, Julian Ganz:

The license was chosen because this is an infrastructure project. I primarily
intended to make my own life, and potentially the life of others, easier. This
also includes future possible work on proprietary products. Hence, I chose a
permissive license.

I am aware of arguments that permissive licenses would diminish contributions
from corporate projects using that code. I'm fine with this. This is a
(personal) infrastructure project, a cathedral, not a bazaar. My goal is not to
attract a large community of developers creeping in a ton of features.

As for corporations which keep fixes or adjustments for themselves while ripping
of the work of others: I have no interest in working with idiots who do not
understand the benefits of sharing the effort of developing and maintaining a
piece of software. Let them write their parallel workarounds and extensions. Let
them experience the pain of maintaining them and eventually being unable to
trivially upgrade to a new version. Let them waste their precious manpower on
that.


