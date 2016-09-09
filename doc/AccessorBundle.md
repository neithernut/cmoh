Accessor bundle
===============

As the name suggests, an accessor bundle is a bundle of accessors. Using an
accessor bundle instead of single accessors has the primary advantage of a
common interface for accessing attributes, by key. On top of that, the
accessor bundle's `create()` method automatically selects a suitable factory for
creating a method and sets the remaining attributes automatically.

Users are highly discouraged from instantiating an accessor bundle directly.
Use the `cmoh::bundle()` function instead, which takes accessors as parameters
and returns an accessor bundle containing all the accessors supplied.


Object creation
---------------

The method template

    template <key_type ...keys>
    object_type create(values ...) const

creates an object of the target type, with an arbitrary number of attributes set
to user-supplied values. The attributes to be set are given by their keys as
template parameters and the values taken by instantiations of the template
will mirror those keys.

On instantiation, a [factory](Factories.md) will be selected for carrying out
the object construction. If the accessor bundle does not contain any factory
suitable for creating an object from a subset of the attributes supplied, the
instantiation fails. Hence, it is recommended to supply a factory using a
minimum number of attributes, or no attributes at all, if the `create()` method
is used.  Attributes which are not used by the factory selected are
automatically set using regular attribute accessors.

The keys supplied as template parameters must be known to the accessor bundle,
e.g. either an accessor for the attribute or a factory using it must be in the
bundle. Due to a current limitation, the necessity for un-settable attributes to
be initialized via the factory is not taken into account during factory
selection. It is therefore highly recommended to place factories taking those
arguments prior to others (e.g. factories taking no arguments at all).


Attribute access
----------------

Access to attributes addressed both statically and dynamically is supported.
In any case, the object on which to carry out the operation has to be supplied.

With statical addressing, the user supplies the key of the attribute to be
accessed at compile time, as a template parameter. The type of the value will
be determined at compile time. Compilation will fail if no attribute with the
key supplied exits.

 *      template <key_type key>
        ...  get(object_type const& obj) const
   will return the value of the attribute with key `key`.

 *      template <key_type key>
        void set(object_type& obj, ...&& value) const
   will set the value of the attribute with key `key`.


With dynamical addressing, the user supplies the key of the attribute to be
accessed at run time, as a regular function parameter. The type to/from which
the attribute's value will be translated must be supplied as a template
parameter, as it can not be determined at compile time. If the types are not
compatible, the access methods will behave as if the attribute does not exist.

 *      template <typename Type>
        optional<Type> get(object_type const& obj, key_type key) const
   will return an optional holding the value of the attribute specified by `key`
   or an empty one, if the attribute could not be found.

 *      template <typename Type>
        bool set(object_type& obj, key_type key, Type&& value) const
   will try to set the value of the attribute specified by `key`. On success,
   the method will return `true`. Otherwise, `false` will be returned.


Visiting properties
-------------------

Using the method

    template <typename Function>
    void visit_properties(Function&& function) const

one can apply a `function` to all accessors accessing an actual, single,
property. Normally, this applies to all accessors except factories.

