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
libraries. Hence, the key is always exported as `<property>::key` while the key
type is exported as the member type `<property>::key_type`.

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
            Value(ObjType::* getter)(),
            void(ObjType::* setter)(Value)
        )
    will create an accessor based on a getter and a setter method which are part
    of the target type. The type of the `Value` doesn't have to match the
    attribute type completely, it just has to be implicitly convertible.

 *      accessor<typename ObjType>(
            Value(ObjType::* getter)(),
            void(ObjType::* setter)(Value const&)
        )
    will create a similar accessor, except that the setter will be required to
    take a const reference instead of a value.

 *      accessor<typename ObjType>(
            Value(ObjType::* getter)(),
            void(ObjType::* setter)(Value&&)
        )
    will create a similar accessor, except that the setter will be required to
    take a rvalue reference.

 *      accessor<typename ObjType>(
            GetterValue(ObjType::* getter)()
        )
    will create an accessor based on only a getter of the target type. Otherwise
    it behaves as the variants also taking a setter.

 *      accessor<typename ObjType, typename ValueType>(
            std::size_t offset
        )
   will create an accessor which accessed a member of type `ValueType` at the
   supplied offset within an object of type `ObjType` directly. Users are highly
   discouraged from using this accessor types unless the situation does not
   allow using any of the above variants.


