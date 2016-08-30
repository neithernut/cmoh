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


