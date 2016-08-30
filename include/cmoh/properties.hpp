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
#ifndef CMOH_PROPERTIES_HPP__
#define CMOH_PROPERTIES_HPP__


// local includes
#include <cmoh/utils.hpp>


namespace cmoh {
namespace properties {


/**
 * Get the type of an attribute
 *
 * Exports the attribute's type via the member `type`. If the property is not
 * an attribute, `type` will be void.
 */
template <
    typename Property, ///< property to check
    typename = void
>
struct type_of_attribute {
    typedef void type;
};

// Specialization for attributes
template <
    typename Property
>
struct type_of_attribute<
    Property,
    util::void_t<typename Property::type>
> {
    typedef typename Property::type type;
};


/**
 * Check whether a property is an attribute with a specific type
 *
 * If the property supplied is an attribute with the type `Type`, the static
 * member `value` will be true. Otherwise, `value` will be false.
 */
template <
    typename Property, ///< property to check
    typename Type, ///< type to compare to
    typename = void
>
struct is_attribute_with_type : std::false_type {};

// Specialization for attributes
template <
    typename Property,
    typename Type
>
struct is_attribute_with_type<
    Property,
    Type,
    util::void_t<typename Property::type>
> : std::is_same<typename Property::type, Type> {};


}
}


#endif
