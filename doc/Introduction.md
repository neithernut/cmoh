Introduction
============

CMOH is a header-only C++ library providing an unintrusive, low-cost meta
object facility for C and C++ structures and classes. Unintrusive in this
context mans that a class doesn't has to be inherited from a special "object"
class like in Qt, nor does CMOH require you to inherit from a class you want to
access. You don't have to manually wrap objects. Also, CMOH itself does require
neither code annotations of your types nor code generation.

Instead, CMOH is designed to provide means to declare
[properties](Properties.md), e.g. attributes, completely independent from any
concrete implementation. Using "accessors", one can specify how specific
properties are accessed for a concrete C/C++ type. Those accessors can be
bundled in an [accessor bundle](AccessorBundle.md), which provides a common
interface through with properties can be accessed for a specific C++ type using
keys attached to the individual properties.

CMOH also does not require users to supply a default constructor for a type to
be deserialized. Instead, the [factory](Factory.md) facilities may be used to
specify how an object should be created.

CMOH is designed to be used by (de)serialization and RPC libraries/adapters,
rather than by applications directly. Those libraries and adapters will, in most
cases, expose aliases of property types. An accessor bundle might then be used
internally for addressing properties and (de)serialization and/or access code
using meta programming techniques.


