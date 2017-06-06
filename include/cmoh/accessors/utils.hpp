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
#ifndef CMOH_ACCESSORS_UTILS_HPP__
#define CMOH_ACCESSORS_UTILS_HPP__


// std includes
#include <type_traits>


// local includes
#include <cmoh/utils.hpp>


namespace cmoh {
namespace accessors {


/**
 * Check whether a type is an addressable property
 *
 * A property has a key which can be retrieved via the `key()` method.
 */
template <
    typename Property,
    typename = void
>
struct is_property : std::false_type {};

template <
    typename Property
>
struct is_property<Property, util::void_t<decltype(Property::key())>> : std::true_type {};


/**
 * Check whether a supposed accessor is a factory
 *
 * Detection is performed by detecting the `is_initializable_from` member.
 */
template <
    typename Accessor, ///< accessor to check
    typename = void
>
struct is_factory : std::false_type {};

// Specialization for factories
template <
    typename Accessor
>
struct is_factory<
    Accessor,
    util::void_t<typename Accessor::template is_initializable_from<>>
> : std::true_type {};


/**
 * Check whether a supposed accessor is an attribute accessor
 *
 * An attribute accessor has a method `get()` which accepts an object of the
 * cotnained object_type.
 */
template <
    typename Accessor, ///< accessor to check
    typename = void
>
struct is_attribute_accessor : std::false_type {};

// Specialization for attributes
template <
    typename Accessor
>
struct is_attribute_accessor<
    Accessor,
    util::void_t<decltype(std::declval<Accessor>().get(
            std::declval<typename Accessor::object_type>()
    ))>
> : std::true_type {};


/**
 * Enumeration type for detecting different types of accessors
 */
enum accessor_type {
    none, ///< not an accessor
    factory_implementation, ///< a factory
    attribute_accessor ///< an attribute accessor
};


/**
 * Meta function for querying the accessor type of a type
 *
 * Returns the `accessor_type` associated with the `Accessor` supplied via the
 * `value` member.
 */
template <
    typename Accessor
>
using accessor_type_of = std::integral_constant<
    accessor_type,
    is_factory<Accessor>::value ? factory_implementation :
    is_attribute_accessor<Accessor>::value ? attribute_accessor :
    none
>;


/**
 * Query an accessor's underlying type
 *
 * Returns an accessor's underlying property (e.g. the type containing the
 * members `key_type` and `key`) or void via the member `type`.
 */
template <
    typename Accessor
>
struct property {
private:
    template <
        typename T,
        accessor_type acc_type
    >
    struct helper {
        typedef T type;
    };

    template <typename T>
    struct helper<T, factory_implementation> {
        typedef void type;
    };

    template <typename T>
    struct helper<T, attribute_accessor> {
        typedef typename T::property type;
    };

public:
    typedef typename helper<
        Accessor,
        accessor_type_of<Accessor>::value
    >::type type;
};


/**
 * Query an accessor's key type
 */
template <
    typename Accessor
>
struct key_type {
private:
    template <
        typename T,
        typename = void
    >
    struct helper {
        typedef typename cmoh::accessors::property<Accessor>::type::key_type
            type;
    };

    template <
        typename T
    >
    struct helper<T, util::void_t<typename T::key_type>> {
        typedef typename Accessor::key_type type;
    };

public:
    typedef typename helper<Accessor>::type type;
};


/**
 * Query the key associated with an accessor's attribute
 */
template <
    typename Accessor
>
constexpr
typename std::remove_reference<typename key_type<Accessor>::type>::type
key(
    Accessor const& accessor
) {
    return cmoh::accessors::property<Accessor>::type::key();
}


/**
 * Check whether an accessor accesses one of several properties
 *
 * Provides the member `value`, which is true if the accessor provided accesses
 * any property represented by `keys` and false otherwise.
 */
template <
    typename Accessor, ///< accessor to check
    typename KeyType, ///< key type to use
    KeyType ...keys ///< keys to compare to
>
struct accesses {
private:
    template <
        typename T,
        typename = void
    >
    struct helper : std::false_type {};

    template <
        typename T
    >
    struct helper<T, util::void_t<decltype(T::key())>> :
        util::disjunction<std::integral_constant<bool, T::key() == keys>...> {};

public:
    enum : bool {value = helper<typename property<Accessor>::type>::value};
};


/**
 * Get the one property having a specific key
 *
 * The property identified by `key` provided by the `Accessor` supplied is
 * exposed via the member `type`. If no such property exists, `type` will be
 * `void`.
 */
template <
    typename Accessor, ///< accessor from which to extract the property
    typename KeyType, ///< key type to use
    KeyType key ///< key to compare to
>
struct property_by_key {
private:
    template <
        typename T,
        typename = void
    >
    struct helper {
        typedef typename std::conditional<
            accesses<T, KeyType, key>::value,
            typename property<T>::type,
            void
        >::type type;
    };

    template <typename T>
    struct helper<T, util::void_t<typename T::template property_by_key<key>>> :
        T::template property_by_key<key> {};

public:
    typedef typename helper<Accessor>::type type;
};


template <
    typename Accessor,
    typename KeyType,
    KeyType ...keys
>
struct is_initializable_from {
    private:
    template <
        typename T,
        typename = void
    >
    struct helper : std::false_type {};

    template <typename T>
    struct helper<T, util::void_t<typename T::template is_initializable_from<>>> :
        T::template is_initializable_from<keys...> {};

public:
    enum : bool {value = helper<Accessor>::value};
};


}
}




#endif
