Utilities
=========

CMOH ships a few utilities, of which a few are pre-definitions of types that can
be found in the C++17 standard proposal. Those utilities mainly reside in the
header `<cmoh/utils.hpp>`, in the `cmoh::util` namespace.


`cmoh::optional`
----------------

Additionally, CMOH ships with an partial implementation of the `std::optional`
template, but in the `cmoh` namespace. However, for post-C++14 builds the
headers `<optional>` and `<experimental/optional>` are searched for. If found,
`cmoh::optional` will be an alias of either `cmoh::optional` or
`std::experimental::optional`, with the `cmoh::optional` being preferred.


`cmoh::char_traits`
-------------------

This class template is an extension to the `std::char_traits` template. It
replaces a few methods with `constexpr` variants, enabling compile-time usage.
It is used in the `cmoh::basic_string_view` and specializations.


`cmoh::basic_string_view` and (full) specializations
----------------------------------------------------

This template is an alias/partial implementation of the `std::basic_string_view`
with the traits defaulting to `cmoh::char_traits`. This enables the use of
instances at compile time, e.g. as property keys.

Like `cmoh::optional`, the template will be an alias for post-C++14 builds, if
the header `<string_view>` is present. The header `<experimental/string_view>`
is not considered, because known implementations contain defects preventing the
use in CMOH. For C++14 builds, `cmoh::basic_string_view` will be a partial
implementation of the class proposed in the standard draft.

The header `<cmoh/string_view>` also contains the following typedefs, which
correspond to their standard counterparts (again, with `cmoh::char_traits`
instead of the standard one):

 * `string_view`
 * `wstring_view`
 * `u16string_view`
 * `u32string_view`


