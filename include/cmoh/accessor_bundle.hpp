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
#include <cmoh/utils.hpp>


namespace cmoh {


/**
 * Accessor bundle
 *
 * Instantiations of this template bundle accessors and make them conveniently
 * accessible as a group, posing as an abstraction layer.
 *
 * TODO: further explanation
 *
 * Users are discouraged from constructing accessor bundles directly. Usexi
 * `bundle()` instread as a factory.
 */
template <
    typename ...Accessors
>
class accessor_bundle;


// Main specialization
template <
    typename Accessor,
    typename ...Accessors
>
class accessor_bundle<Accessor, Accessors...> {
    typedef Accessor accessor;
    accessor _accessor;

    typedef accessor_bundle<Accessors...> next;
    next _next;

public:
    /**
     * The C++ type which may be accessed using the accessor bundle
     */
    typedef typename std::conditional<
        count<Accessors...>::value == 0,
        typename accessor::object_type,
        typename next::object_type
    >::type object_type;
    static_assert(
        std::is_same<typename accessor::object_type, object_type>::value,
        "All accessors in a bundle must have the same object type"
    );

    accessor_bundle(accessor current, Accessors... accessors) :
            _accessor(std::forward<accessor>(current)),
            _next(std::forward<Accessors>(accessors)...) {};
    accessor_bundle(accessor_bundle const&) = default;
    accessor_bundle(accessor_bundle&&) = default;
    accessor_bundle() = delete;

    /**
     * Get the value of a specific attribute from an object
     *
     * \returns the value of the attribute
     */
    template <typename Attribute>
    typename std::enable_if<
        !std::is_same<Attribute, typename accessor::attr>::value,
        typename Attribute::type
    >::type
    get(
        object_type const& obj ///< object from which to get the value
    ) const {
        return _next.template get<Attribute>(obj);
    }

    template <typename Attribute>
    typename std::enable_if<
        std::is_same<Attribute, typename accessor::attr>::value,
        typename Attribute::type
    >::type
    get(
        object_type const& obj ///< object from which to get the value
    ) const {
        return _accessor.get(obj);
    }

    /**
     * Set the value of a specific attribute on an object
     */
    template <typename Attribute>
    typename std::enable_if<
        !std::is_same<Attribute, typename accessor::attr>::value
    >::type
    set(
        object_type& obj, ///< object on which to set the attribute
        typename Attribute::type&& value ///< value to set
    ) const {
        _next.template set<Attribute>(obj, std::forward<typename Attribute::type>(value));
    }

    template <typename Attribute>
    typename std::enable_if<
        std::is_same<Attribute, typename accessor::attr>::value
    >::type
    set(
        object_type& obj, ///< object on which to set the attribute
        typename Attribute::type&& value ///< value to set
    ) const {
        _accessor.set(obj, std::forward<typename Attribute::type>(value));
    }
};


// Recursion terminator
template <>
class accessor_bundle<> {
public:
    typedef void object_type;

    // accept whatever comes in and error out
    template <typename Attribute, typename ObjectType>
    typename Attribute::type
    get(
        ObjectType const& obj
    ) const {
        static_assert(
            true,
            "The attribute can not be accessed via this bundle."
        );
    }

    // accept whatever comes in and error out
    template <typename ObjectType, typename Value>
    void
    get(
        ObjectType const& obj,
        Value&& value
    ) const {
        static_assert(
            true,
            "The attribute can not be accessed via this bundle."
        );
    }
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
    Accessors... accessors ///< accessors to bundle
) {
    return accessor_bundle<Accessors...>(std::forward<Accessors>(accessors)...);
}


}


#endif
