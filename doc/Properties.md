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

TODO: explanation of attributes


