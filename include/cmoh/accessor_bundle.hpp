/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Julian Ganz
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef CMOH_ACCESSOR_BUNDLE_HPP__
#define CMOH_ACCESSOR_BUNDLE_HPP__


// std includes
#include <type_traits>
#include <utility>

// local includes
#include <cmoh/accessors/utils.hpp>
#include <cmoh/optional.hpp>
#include <cmoh/properties.hpp>
#include <cmoh/selectable_items.hpp>
#include <cmoh/utils.hpp>


namespace cmoh {


/**
 * Accessor bundle
 *
 * Instantiations of this template bundle accessors and make them conveniently
 * accessible as a group, posing as an abstraction layer.
 *
 * In an accessor bundle, accessors are typically addressed using the accessed
 * property's key. For each type of property, the bundle contains access
 * methods (e.g. getters and setters for attributes).
 *
 * Users are discouraged from constructing accessor bundles directly. Use
 * `bundle()` instead as a factory.
 */
template <
    typename ...Accessors
>
struct accessor_bundle {
private:
    typedef selectable_items<Accessors...> accessors;

public:
    /**
     * Common object type of all accessors
     */
    typedef
        typename util::common_type<typename Accessors::object_type...>::type
        object_type;

    /**
     * Common key type of all accessors
     */
    typedef typename util::common_type<
        typename cmoh::accessors::key_type<Accessors>::type...
    >::type key_type;

    /**
     * Get the property identified by a specific key
     *
     * Instantiations will either yield the attribute type associated with the
     * `key` supplied or void, if no such property could be found.
     */
    template <
        key_type key
    >
    using property_by_key = typename util::common_type<
        typename cmoh::accessors::property_by_key<
            Accessors,
            key_type,
            key
        >::type...
    >::type;


    accessor_bundle(Accessors... accessors) :
            _accessors(std::forward<Accessors>(accessors)...) {}
    accessor_bundle(accessor_bundle const&) = default;
    accessor_bundle(accessor_bundle&&) = default;
    accessor_bundle() = delete;


    /**
     * Set the value of a specific attribute on an object
     */
    template <
        key_type ...keys ///< keys of attributes from which to construct an object
    >
    object_type
    create(
        typename property_by_key<keys>::type&&... values ///< values to use
    ) const {
        auto factory = _accessors.template get<
            cmoh::accessors::is_initializable_from<
                Accessors,
                key_type,
                keys...
            >...
        >();

        // construct the object itself
        auto retval{factory.template create<property_by_key<keys>...>(
            std::forward<typename property_by_key<keys>::type>(values)...
        )};

        initialize_if_unused<decltype(factory), keys...>(
            retval,
            std::forward<typename property_by_key<keys>::type>(values)...
        );

        return retval;
    }


    /**
     * Get the value of a specific attribute from an object
     *
     * \returns the value of the attribute
     */
    template <
        key_type key ///< key of attribute to get
    >
    typename property_by_key<key>::type
    get(
        object_type const& obj ///< object from which to get the value
    ) const {
        return _accessors.template get<
            cmoh::accessors::accesses<Accessors, key_type, key>...
        >().get(obj);
    }

    /**
     * Set the value of a specific attribute on an object
     */
    template <
        key_type key ///< key of attribute to set
    >
    void
    set(
        object_type& obj, ///< object on which to set the attribute
        typename property_by_key<key>::type&& value ///< value to set
    ) const {
        _accessors.template get<
            cmoh::accessors::accesses<Accessors, key_type, key>...
        >().set(obj, std::forward<typename property_by_key<key>::type>(value));
    }


    /**
     * Get the value of a specific attribute from an object
     *
     * \returns the value of the attribute
     */
    template <
        typename Type, ///< type of the attribute to get
        typename KeyType = key_type ///< key type to use
    >
    optional<Type>
    get(
        object_type const& obj, ///< object from which to get the value
        KeyType&& key ///< key of the attribute to get
    ) const {
        optional<Type> retval;

        visit_attributes<Type>([&] (auto accessor) {
            if (!(cmoh::accessors::key(accessor) == key))
                return;
            retval = accessor.get(obj);
        });

        return retval;
    }

    /**
     * Set the value of a specific attribute on an object
     */
    template <
        typename Type, ///< type of the attribute to get
        typename KeyType = key_type ///< key type to use
    >
    bool
    set(
        object_type& obj, ///< object on which to set the attribute
        KeyType&& key, ///< key of the attribute to get
        Type&& value ///< value to set
    ) const {
        bool retval = false;

        visit_attributes<Type>([&] (auto accessor) {
            if (!(cmoh::accessors::key(accessor) == key))
                return;
            accessor.set(obj, std::forward<Type>(value));
            retval = true;
        });

        return retval;
    }


    /**
     * Calls a function with every accessor accessing a property
     *
     * This method applies the function supplied on every accessor which
     * accesses a single property. The function will be calles with the
     * accessors as the only argument.
     */
    template <
        typename Function
    >
    void
    visit_properties(
        Function&& function
    ) const {
        _accessors.template visit<
            Function,
            std::integral_constant<
                bool,
                !std::is_same<
                    typename cmoh::accessors::property<Accessors>::type,
                    void
                >::value
            >...
        >(std::forward<Function>(function));
    }


private:
    /**
     * Initialize attributes which are not used by a specific constructor
     */
    template <
        typename Constructor, ///< constructor to consider
        key_type key0, ///< first of the attributes' keys
        key_type ...keys ///< rest of the attributes' keys
    >
    void
    initialize_if_unused(
        object_type& obj, ///< object on which to set the attributes' values
        typename property_by_key<key0>::type&& value0, ///< first of the values
        typename property_by_key<keys>::type&&... values ///< rest of the values
    ) const {
        initialize_single_if_unused<Constructor, key0>(
            obj,
            std::forward<typename property_by_key<key0>::type>(value0)
        );

        // recurse
        initialize_if_unused<Constructor, keys...>(
            obj,
            std::forward<typename property_by_key<keys>::type>(values)...
        );
    }

    // overload for an empty list of attributes
    template <
        typename Constructor
    >
    void
    initialize_if_unused(
        object_type& obj
    ) const {}


    /**
     * Initialize a single attribute if it is not used by a specific constructor
     */
    template <
        typename Constructor, ///< constructor to consider
        key_type key ///< attribute to set
    >
    typename std::enable_if<
        !Constructor::template uses<key>::value
    >::type
    initialize_single_if_unused(
        object_type& obj, ///< object on which to set the attribute's value
        typename property_by_key<key>::type&& value ///< value to set
    ) const {
        // TODO: static assertion for unsettable attributes
        set<key>(obj, std::forward<typename property_by_key<key>::type>(value));
    }

    // overload for attributes which are used by the constructor specified
    template <
        typename Constructor,
        key_type key
    >
    typename std::enable_if<
        Constructor::template uses<key>::value
    >::type
    initialize_single_if_unused(
        object_type& obj,
        typename property_by_key<key>::type&& value
    ) const {}


    template <
        typename Type,
        typename Function
    >
    void
    visit_attributes(
        Function&& function
    ) const {
        _accessors.template visit<
            Function,
            std::is_convertible<
                typename properties::type_of_attribute<
                    typename cmoh::accessors::property<Accessors>::type
                >::type,
                Type
            >...
        >(std::forward<Function>(function));
    }


    accessors _accessors;
};


/**
 * Construct an accessor bundle from a bunch of accessors
 *
 * \returns an accessor bundle holding all the accessors supplied
 */
template <
    typename ...Accessors
>
constexpr
const
accessor_bundle<Accessors...>
bundle(
    Accessors&&... accessors ///< accessors to bundle
) {
    return accessor_bundle<Accessors...>(std::forward<Accessors>(accessors)...);
}


}


#endif
