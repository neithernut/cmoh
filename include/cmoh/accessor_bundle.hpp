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
 * Check whether an accessor accesses a specific attribute
 *
 * This checks whether the accessor provided features a type `attr` identical to
 * the attribute provided.
 */
template <
    typename Accessor, ///< accessor to test for the attribute
    typename Attribute, ///< attribute to be accessed
    typename = void
>
struct accesses_attribute : std::false_type {};

// Specialization for accessors featuring a type `attr`
template <
    typename Accessor,
    typename Attribute
>
struct accesses_attribute<
    Accessor,
    Attribute,
    util::void_t<typename Accessor::attr>
> : std::is_same<Attribute, typename Accessor::attr> {};


/**
 * Accessor bundle
 *
 * Instantiations of this template bundle accessors and make them conveniently
 * accessible as a group, posing as an abstraction layer.
 *
 * TODO: further explanation
 *
 * Users are discouraged from constructing accessor bundles directly. Use
 * `bundle()` instread as a factory.
 */
template <
    typename ...Accessors
>
struct accessor_bundle {
    typedef
        typename util::common_type<typename Accessors::object_type...>::type
        object_type;


    accessor_bundle(Accessors... accessors) :
            _accessors(std::forward<Accessors>(accessors)...) {}
    accessor_bundle(accessor_bundle const&) = default;
    accessor_bundle(accessor_bundle&&) = default;
    accessor_bundle() = delete;


    /**
     * Get the value of a specific attribute from an object
     *
     * \returns the value of the attribute
     */
    template <
        typename Attribute ///< attribute to get
    >
    typename Attribute::type
    get(
        object_type const& obj ///< object from which to get the value
    ) const {
        return _accessors.
            template get<accesses_attribute<Accessors, Attribute>...>().
            get(obj);
    }

    /**
     * Set the value of a specific attribute on an object
     */
    template <
        typename Attribute ///< attribute to set
    >
    void
    set(
        object_type& obj, ///< object on which to set the attribute
        typename Attribute::type&& value ///< value to set
    ) const {
        _accessors.
            template get<accesses_attribute<Accessors, Attribute>...>().
            set(obj, std::forward<typename Attribute::type>(value));
    }


private:
    util::selectable_items<Accessors...> _accessors;
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
