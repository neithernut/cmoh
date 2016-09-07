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
#ifndef CMOH_CHAR_TRAITS_HPP__
#define CMOH_CHAR_TRAITS_HPP__


// std includes
#include <string>


namespace cmoh {


/**
 * Char traits suitable for compile time usage
 *
 * For performance reasons, the methods `compare()`, `length()` and `find()` of
 * the standard char traits are not `constexpr`.
 *
 * As we require those to be evaluatable during compile time, we ship our own
 * `char_traits`, which is based on the standard one but exchanges those three
 * functions with recursive, `constexpr` implementations.
 *
 * Note that those are expected to be slow compared to optimized
 * implementations. Hence, users and developers are discouraged from using them
 * for runtime code.
 */
template <typename CharT>
class char_traits : public std::char_traits<CharT> {
    typedef std::char_traits<CharT> super;
public:
    static
    constexpr
    int
    compare(
        const typename super::char_type* s1,
        const typename super::char_type* s2,
        std::size_t count
    ) noexcept {
        if (count == 0)
            return 0;
        if (*s1 != *s2)
            return *s2 > *s1 ? 1 : -1;
        return compare(s1 + 1, s2 + 1, count - 1);
    }

    static
    constexpr
    std::size_t
    length(
        const typename super::char_type* s
    ) noexcept {
        if (*s)
            return length(s + 1) + 1;
        return 0;
    }

    static
    constexpr
    const typename super::char_type*
    find(
        const typename super::char_type* p,
        std::size_t count,
        const typename super::char_type& ch
    ) noexcept {
        if (count == 0)
            return nullptr;
        if (*p == ch)
            return p;
        return find(p + 1, count - 1, ch);
    }
};


}




#endif
