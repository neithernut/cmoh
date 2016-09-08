Using CMOH
==========

CMOH is a header-only library. No linking is required and CMOH does not depend
on any external libraries other than a subset of the STL. However, CMOH makes
heavy use of (variadic) templates and C++14 features. Hence, a C++14 compliant
tool chain is required with a certain set of capabilities. Clang-3.8 or gcc-5.3
or later are recommended.

Both a CMake module and a pkg-config-file are installed along with the library.
In both cases, the packet is called "cmoh". If you are using neither CMake nor
pkg-config-based setups, add `<installation_prefix>/include` to your include
directories.


Using CMOH in embedded and/or constrained environments
------------------------------------------------------

CMOH does require the standard headers `<cstddef>`, `<type_traits>` and
`<utility>`. Either of the headers `<optional>` and `<experimental/optional>`
is used in post-C++14 builds if available (see the [utilities](Utilities.md)
chapter for more information). Additionally, CMOH may contain support for
arbitrary STL types (e.g. smart pointers). Those features are, however, optional
and can be disabled using the following preprocessor definitions:

 * `CMOH_NO_UNIQUE_PTR` disables use of `std::unique_ptr`
 * `CMOH_NO_SHARED_PTR` disables use of `std::shared_ptr`
 * `CMOH_NO_TUPLE` disables use of `std::tuple`
 * `CMOH_NO_FUNCTION` disables use of `std::function`

It may also be of interest that CMOH does not allocate or free any memory,
unless a smart pointer feature is used.

The class templates `cmoh::char_traits` and `cmoh::basic_string_view` (and
specializations) make use of additional STL features and may thus not be
suitable for some environments. However, those classes are only used for
addressing properties by strings and at least one of the header files
`<cmoh/char_traits.hpp>` and `<cmoh/string_view>` must be included by the user
in these cases.


Versioning scheme
-----------------

We do [semantic versioning](http://semver.org/) version 2.0.0. As CMOH is in an
initial development phase, this doesn't give users a strong guarantee. However,
we already do try to apply the rules normally designed for post-1.0 releases
from version 0.1 onward, regarding the (public) types and functions residing
in the `cmoh` namespace. Specifically, we will only increase the C++ version
required with major releases.


