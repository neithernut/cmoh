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
#ifndef CMOH_STRING_UTILS_HPP__
#define CMOH_STRING_UTILS_HPP__


// std includes
#include <string>

// local includes
#include <cmoh/string_view.hpp>




// work-arounds for comparing strings to string views
template <
    typename Traits
>
bool
operator == (
    std::string const& string,
    cmoh::basic_string_view<std::string::value_type, Traits> view
) {
    auto v = cmoh::std_traits_using(view);
    return string.compare(0, string.length(), v.data(), v.size()) == 0;
}

template <
    typename Traits
>
bool
operator == (
    cmoh::basic_string_view<std::string::value_type, Traits> view,
    std::string const& string
) {
    return string == view;
}


template <
    typename Traits
>
bool
operator != (
    std::string const& string,
    cmoh::basic_string_view<std::string::value_type, Traits> view
) {
    return !(string == view);
}

template <
    typename Traits
>
bool
operator != (
    cmoh::basic_string_view<std::string::value_type, Traits> view,
    std::string const& string
) {
    return !(string == view);
}




#endif
