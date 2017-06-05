Properties
==========

CMOH, conceptually, allows for arbitrary kinds of (object) properties,
e.g. attributes, methods and signals. Currently, however, only attributes are
supported.

Properties are declared by instantiating a the template corresponding to the
attribute type. The template parameters taken by CMOH property templates are
composed of a key type, a key (of the key type specified) and property type
specific parameters (see below for details), in that order.

The key is used or addressing properties in an object. The idea is to use the
key for binding a property to an external representation/an RPC interface in
libraries. Hence, the key is always exported as `<property>::key()` while the
key type is exported as the member type `<property>::key_type`.

Properties also expose a static method `accessor()`, which serves as a factory
for (potentially diverse) accessor types available for the type of property.
Accessors are small objects which abstract the method of accessing/calling the
(abstract) property and provide an (property type specific) interface.

The `accessor()` method will generally require a template parameter `ObjType`,
which will be the concrete C++ type the accessor will target. As this template
parameter will, in most cases, not be deducible, it is highly recommended to
always provide it.

Users will most likely never touch an accessor, but feed them directly to an
[accessor bundle](AccessorBundle.md) instead.


Key types to use
----------------

Properties are represented by (full) specializations of templates. Keys are
generally provided as template arguments in these specializations. This was
partly motivated by the need for keys to be comparable at compile time.

The non-type template parameters are limited to a certain set of values which
the compiler can evaluate at compile time. For example, one cannot simply pass
a string as a template parameter. One can, however, pass enumerations, integral
values and references to certain objects.

Generally, CMOH compares keys using the `==` operator. Furthermore, overloads of
`operator == ()` must be declared `constexpr` to be of use. Hence, any class
with appropriate constructors and a `constexpr operator == ()` overload can be
used as a key. In those cases, the key type would be a const reference.

In many cases, enumerations and integers should be sufficient for the user.
If a string key type is required, we recommend using `cmoh::basic_string`, which
is a predefinition/alias of `std::basic_string` (expected in C++17) using
adapted character traits, making it suitable for compile time comparison.

Note that for some compilers (e.g. clang), it may be necessary to include
`<cmoh/string_view>` prior to other cmoh headers for the `operator == ()`
overload to be found.

The use of different string types is demonstrated in the examples.


Attributes
----------

Attributes are declared using the `cmoh::attribute` template declared in
`<cmoh/attribute.hpp>`:

    template <typename KeyType, KeyType Key, typename Type>
    struct attribute;

In addition to the standard property parameters `KeyType` and `Key`, the
template also takes the `Type` of the attribute as a parameter. `Type` may also
be `const` to denote read-only attributes. In this case, `<attribute>::is_const`
is true, otherwise it's false. The original `Type` is exported as the member
type `<attribute>::type`, with eventual `const` specifiers removed.

It is not required that an attribute actually exists in a specific C++ type. An
attribute can as well be a deduced value. However, the attribute's type has to
be `const` in this case.

The following overloads to the `accessor()` factory provided by attributes
exist. However, some are available for const/non-const attributes only:
 *      accessor<typename ObjType>(
            Getter,
            Setter
        )
    will create an accessor based on a getter and a setter. Both may either be
    pointers to methods of the target type, function pointers or callable
    objects (e.g. lambdas).

    The return parameter of the getter is implicitly converted to the attribute
    type. Likewise, the "value" parameter of the setter may have any type which
    is implicitly convertible from the attribute type.

 *      accessor<typename ObjType>(
            Getter
        )
    will create an accessor based on only a getter of the target type. Otherwise
    it behaves as the variant also taking a setter.

 *      accessor<typename ObjType, typename ValueType>(
            std::size_t offset
        )
   will create an accessor which accessed a member of type `ValueType` at the
   supplied offset within an object of type `ObjType` directly. Users are highly
   discouraged from using this accessor types unless the situation does not
   allow using any of the above variants.


