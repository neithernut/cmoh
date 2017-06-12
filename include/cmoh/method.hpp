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
#ifndef CMOH_METHOD_HPP__
#define CMOH_METHOD_HPP__


// std includes
#include <type_traits>
#include <utility>


namespace cmoh {


/**
 * Method declaration utility
 *
 * Using this template, a programmer may declare methods she wants to make
 * accessible to the cmoh system. Use like:
 *
 *     enum methods {foo, bar};
 *     using foo_attr = cmoh::method<methods, foo, false, void, int>;
 *     using bar_attr = cmoh::method<methods, bar, true, int(std::string)>;
 *
 */
template <
    typename KeyType, ///< type of the key used to identify the method
    KeyType Key, ///< identifier of the method
    typename ReturnType, ///< return type of the method
    typename ...Arguments ///< types of arguments pased to the method
>
struct method {
    typedef KeyType key_type;
    typedef ReturnType return_type;


    static
    constexpr
    typename std::remove_reference<key_type>::type
    key() {
        return Key;
    }
};


// convenience specialization, so you can declare a method using function type
// notation
template <
    typename KeyType,
    KeyType Key,
    typename ReturnType,
    typename ...Arguments
>
struct method<KeyType, Key, ReturnType(Arguments...)> :
    method<KeyType, Key, ReturnType, Arguments...> {};
}


#endif
