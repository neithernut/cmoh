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
 * Detection is performed by detecting the `attribute` member.
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
    util::void_t<typename Accessor::attribute>
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


}
}




#endif
