/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Julian Ganz
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
#ifndef CMOH_ATTRIBUTE_BY_INVOCABLE_HPP__
#define CMOH_ATTRIBUTE_BY_INVOCABLE_HPP__


// std includes
#include <type_traits>
#include <utility>


// local includes
#include <cmoh/utils.hpp>


namespace cmoh {
namespace accessors {
namespace attribute {


/**
 * Attribute accessor using invocables
 *
 * This accessor provides read-only access to the attribute of the target C++
 * struct/class via an invocable getter. It is constructed from an appropriate
 * one.
 *
 * Users are discouraged from constructing method accessors directly. Use
 * one of the `accessor()` overloads provided by the attribute instead.
 */
template <
    typename Attribute, ///< attribute type being accessed
    typename ObjType, ///< type of the class or struct with the attribute
    typename Getter ///< type of the getter
>
struct by_invocable_const {
    typedef Attribute attribute; ///< type of attribute being accessed
    typedef ObjType object_type; ///< object being accessed

    typedef Getter getter; // type of the getter used


    static_assert(
        util::is_invocable<getter, object_type>::value,
        "Getter not invokable with object"
    );

    static_assert(
        std::is_convertible<
            decltype(util::invoke(std::declval<getter>(), std::declval<object_type>())),
            typename attribute::type
        >::value,
        "Value returned by getter is not convertible to attribute type"
    );


    by_invocable_const(getter&& g) : _getter(std::forward<getter>(g)) {};
    by_invocable_const(by_invocable_const const&) = default;
    by_invocable_const(by_invocable_const&&) = default;


    /**
     * Get the attribute from an object
     *
     * \returns the attribute's value
     */
    typename attribute::type
    get(
        object_type const& obj ///< object from which to get the value
    ) const {
        return util::invoke(_getter, obj);
    }

private:
    getter _getter;
};


/**
 * Attribute accessor using invocables
 *
 * This accessor provides access to the attribute of the target C++ struct/class
 * via an invocable getter. It is constructed from an appropriate one.
 *
 * Users are discouraged from constructing method accessors directly. Use
 * one of the `accessor()` overloads provided by the attribute instead.
 */
template <
    typename Attribute, ///< attribute type being accessed
    typename ObjType, ///< type of the class or struct with the attribute
    typename Getter, ///< type of the getter
    typename Setter ///< type of the setter
>
struct by_invocable : by_invocable_const<Attribute, ObjType, Getter> {
    typedef Attribute attribute; ///< type of attribute being accessed
    typedef ObjType object_type; ///< object being accessed

    typedef Getter getter; // type of the getter used
    typedef Setter setter; // type of the setter used


    static_assert(
        util::is_invocable<
            setter,
            object_type,
            typename attribute::type
        >::value,
        "Getter not invokable with object"
    );


    by_invocable(getter g, setter s) :
        by_invocable_const<Attribute, ObjType, Getter>(std::forward<getter>(g)),
        _setter(std::forward<setter>(s)) {};
    by_invocable(by_invocable const&) = default;
    by_invocable(by_invocable&&) = default;


    /**
     * Set the attribute on an object
     */
    void
    set(
        object_type& obj, ///< object on which to set the attribute
        typename attribute::type&& value ///< value to set
    ) const {
        util::invoke(_setter, obj, std::forward<typename attribute::type>(value));
    }

private:
    setter _setter;
};


}
}
}



#endif
