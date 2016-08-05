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
 * TODO: mention of factory
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

    accessor_bundle(accessor accessor, Accessors... accessors) :
            _accessor(accessor), _next(accessors...) {};
    accessor_bundle(accessor_bundle const&) = default;
    accessor_bundle(accessor_bundle&&) = default;
    accessor_bundle() = delete;

    // TODO: access methods
};


// Recursion terminator
template <>
class accessor_bundle<> {
public:
    typedef void object_type;

    // TODO: access methods
};


}


#endif
