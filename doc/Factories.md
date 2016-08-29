Factories
=========

Factories are facilities for creation of objects of a target `ObjectType`. They
are a bit special as they are, in terms of CMOH, accessors which do not
correspond directly to a specific type of property being accessed. Rather, they
specify how an object can be created from a set of attributes.

Currently, factories only create instances of objects. However, they may as well
return (smart) pointers to the target object type in the future.

Like other accessors, they are not intended for direct instantiation or use by
the user. An [accessor bundle](AccessorBundle.md) can use a factory supplied
to it to create instances of the target type from potentially arbitrary
attributes. An overload to the free function `cmoh::factory()` should be used to
create factories. The following overloads are available:

 * `factory<typename ObjType, typename... Attributes>()`
   will create a factory which uses an constructor of `ObjType` taking
   arguments which match those of the attributes supplied.

