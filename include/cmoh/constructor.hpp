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
#ifndef CMOH_CONSTRUCTOR_HPP__
#define CMOH_CONSTRUCTOR_HPP__


// std includes
#include <utility>


namespace cmoh {


/**
 * Construction wrapper
 *
 * Using this template, a programmer may specify from which attributes a
 * specific C++ type may be constructed. An instantiated object may be
 * passed to an accessor bundle.
 */
template <
    typename ObjType, ///< type of the class or struct to be constructed
    typename ...Attributes ///< attributes fed to the constructor
>
struct constructor {
    typedef ObjType object_type; ///< type being constructed

    template <
        typename ...PassedAttributes ///< attrbiutes availible for construction
    >
    object_type
    construct(typename PassedAttributes::type&&... arguments) const {
        return object_type{
            Attributes::template select<PassedAttributes...>(
                std::forward<typename PassedAttributes::type>(arguments)...
            )...
        };
    }
};


}


#endif
