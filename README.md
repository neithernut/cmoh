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


