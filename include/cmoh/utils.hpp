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
#ifndef CMOH_UTILS_HPP__
#define CMOH_UTILS_HPP__


namespace cmoh {
namespace util {


/**
 * Count the number of arguments in a template parameter pack
 *
 * Instantiations of this struct will contain a static member `value`, which
 * will hold the number of arguments supplied to the template.
 */
template <
    typename ...Args
>
struct count;

// Specialization for lists of at least one argument
template <
    typename Arg0,
    typename ...Args
>
struct count<Arg0, Args...> {
    static constexpr unsigned long int value = count<Args...>::value + 1;
};

// Recursion terminator/specialization for zero arguments
template <>
struct count<> {
    static constexpr unsigned long int value = 0;
};


/**
 * Logical conjunction of type traits `Items`
 *
 * Providing that each of the `Items` types provides a member `value` of type
 * bool, this type provides a member `value` of type bool with the value of the
 * logical conjunction of each of the values taken from the parameters.
 *
 * This template mirrors a utility which is expected to ship with C++17
 * compliant STL implementations.
 */
template <
    typename ...Items
>
struct conjunction : std::true_type {};

template <
    typename Item0,
    typename ...Items
>
struct conjunction<Item0, Items...> {
    static constexpr bool value = Item0::value && conjunction<Items...>::value;
};


/**
 * Logical disjunction of type traits `Items`
 *
 * Providing that each of the `Items` types provides a member `value` of type
 * bool, this type provides a member `value` of type bool with the value of the
 * logical disjunction of each of the values taken from the parameters.
 *
 * This template mirrors a utility which is expected to ship with C++17
 * compliant STL implementations.
 */
template <
    typename ...Items
>
struct disjunction : std::false_type {};

template <
    typename Item0,
    typename ...Items
>
struct disjunction<Item0, Items...> {
    static constexpr bool value = Item0::value || disjunction<Items...>::value;
};


}
}


#endif
