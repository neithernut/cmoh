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
#ifndef CMOH_STRING_VIEW_HPP__
#define CMOH_STRING_VIEW_HPP__


// We try to detect whether a standard `string_view` is available, but only for
// post-C++14 (we don't expect a C++14 STL to ship it). We don't consider the
// `experimental/string_view` header, since at least one implementation (the one
// shipped with GCC), lacks `constexpr` in critical places.
#if __cplusplus > 201402L
# if __has_include(<string_view>)
#  define has_string_view 1
# endif
#endif


// local includes
#include <cmoh/char_traits.hpp>


// use one of the `string_view` implementations
#ifdef has_string_view
#include <string_view>
namespace cmoh {
    template <
        typename CharT,
        typename Traits = char_traits<CharT>
    >
    using basic_string_view = std::basic_string_view<CharT, Traits>;
}
#else


// std includes
#include <algorithm>
#include <cstddef>
#include <limits>
#include <ostream>
#include <stdexcept>


namespace cmoh {


/**
 * Predefinition of the C++17 std::basic_string_view type
 *
 * This template provides an optional type which is modeled after the
 * std::basic_string_view which is part of the C++17 standard proposal. Note
 * that only a subset of the interface is provided. Also note that the Traits
 * defaults to a custom `char_traits`, which has some additional compile
 * capabilities but is somewhat slow at run time.
 *
 * For documentation, refer to the C++17 proposal or your favorite STL
 * documentation site.
 */
template <
    typename CharT,
    typename Traits = char_traits<CharT>
>
class basic_string_view {
public:
    typedef Traits traits_type;

    // value types
    typedef CharT value_type;
    typedef CharT* pointer;
    typedef CharT const* const_pointer;
    typedef CharT& reference;
    typedef CharT const& const_reference;

    // TODO: iterator types

    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;


    constexpr basic_string_view() noexcept : _data(nullptr), _count(0) {}
    constexpr basic_string_view(const basic_string_view& other) noexcept =
        default;
    constexpr basic_string_view(const CharT* data, size_type count) noexcept :
        _data(data), _count(count) {}
    constexpr basic_string_view(const CharT* data) :
        _data(data), _count(traits_type::length(data)) {}

    basic_string_view& operator=( const basic_string_view& view ) = default;


    // TODO: iterators


    constexpr const_reference operator[](size_type pos) const {
        return _data[pos];
    }

    constexpr const_reference at(size_type pos) const {
        if (pos >= size())
            throw std::out_of_range("Access out-of-bounds element.");
        return _data[pos];
    }

    constexpr const_reference front() const {
        return _data[0];
    }

    constexpr const_reference back() const {
        return _data[size() - 1];
    }

    constexpr const_pointer data() const noexcept {
        return _data;
    }


    constexpr size_type size() const noexcept {
        return _count;
    }

    constexpr size_type length() const noexcept {
        return size();
    }

    constexpr size_type max_size() const noexcept {
        return std::numeric_limits<size_type>::max();
    }

    constexpr bool empty() const noexcept {
        return size() == 0;
    }


    constexpr void remove_prefix(size_type n) {
        _data+=n;
        _count-=n;
    }

    constexpr void remove_suffix(size_type n) {
        _count-=n;
    }

    constexpr void swap(basic_string_view& v) {
        const_pointer* data = v._data;
        size_type count = v._count;

        v._data = _data;
        v._count = _count;

        _data = data;
        _count = count;
    }


    size_type copy(CharT* dest, size_type count, size_type pos = 0) const {
        if (pos >= size())
            throw std::out_of_range("Access out-of-bounds element.");
        count = std::min(count, size() - pos);
        traits_type::copy(
            dest,
            &_data[pos],
            count*sizeof(value_type)/sizeof(char)
        );
        return count;
    }

    constexpr basic_string_view
    substr(size_type pos = 0, size_type count = npos) const {
        if (pos >= size())
            throw std::out_of_range("Access out-of-bounds element.");
        return basic_string_view(_data + pos, std::min(count, size() - pos));
    }

    constexpr int compare(basic_string_view v) const noexcept {
        auto retval = traits_type::compare(
            data(),
            v.data(),
            std::min(size(), v.size())
        );
        if (retval != 0)
            return retval;
        if (size() == v.size())
            return 0;
        return size() < v.size() ? -1 : 1;
    }

    constexpr int
    compare(size_type pos1, size_type count1, basic_string_view v) const {
        return substr(pos1, count1).compare(v);
    }

    constexpr int
    compare(
        size_type pos1,
        size_type count1,
        basic_string_view v,
        size_type pos2,
        size_type count2
    ) const {
        return substr(pos1, count1).compare(v.substr(pos2, count2));
    }

    constexpr int compare(const_pointer s) const {
        return compare(basic_string_view(s));
    }

    constexpr int
    compare(size_type pos1, size_type count1, const_pointer s) const {
        return substr(pos1, count1).compare(s);
    }

    constexpr int
    compare(
        size_type pos1,
        size_type count1,
        const_pointer s,
        size_type count2
    ) const {
        return substr(pos1, count1).compare(basic_string_view(s, count2));
    }


    // TODO: find, rfind, ...


    static constexpr size_type npos = size_type(-1);


private:
    value_type const* _data;
    size_type _count;
};


}


template<
    class CharT,
    class Traits
>
constexpr bool operator == (
    cmoh::basic_string_view<CharT,Traits> lhs,
    cmoh::basic_string_view<CharT,Traits> rhs
) noexcept {
    return lhs.compare(rhs) == 0;
}

template<
    class CharT,
    class Traits
>
constexpr bool operator != (
    cmoh::basic_string_view<CharT,Traits> lhs,
    cmoh::basic_string_view<CharT,Traits> rhs
) noexcept {
    return lhs.compare(rhs) != 0;
}

template<
    class CharT,
    class Traits
>
constexpr bool operator < (
    cmoh::basic_string_view<CharT,Traits> lhs,
    cmoh::basic_string_view<CharT,Traits> rhs
) noexcept {
    return lhs.compare(rhs) < 0;
}

template<
    class CharT,
    class Traits
>
constexpr bool operator <= (
    cmoh::basic_string_view<CharT,Traits> lhs,
    cmoh::basic_string_view<CharT,Traits> rhs
) noexcept {
    return lhs.compare(rhs) <= 0;
}

template<
    class CharT,
    class Traits
>
constexpr bool operator > (
    cmoh::basic_string_view<CharT,Traits> lhs,
    cmoh::basic_string_view<CharT,Traits> rhs
) noexcept {
    return lhs.compare(rhs) > 0;
}

template<
    class CharT,
    class Traits
>
constexpr bool operator >= (
    cmoh::basic_string_view<CharT,Traits> lhs,
    cmoh::basic_string_view<CharT,Traits> rhs
) noexcept {
    return lhs.compare(rhs) >= 0;
}


template <
    class CharT,
    class Traits
>
std::basic_ostream<CharT, Traits>&
operator << (
    std::basic_ostream<CharT, Traits>& stream,
    cmoh::basic_string_view <CharT, Traits> view
) {
    auto len = view.size();
    auto pos = view.data();

    while (len > 0) {
        stream.put(*pos);
        ++pos;
        --len;
    }

    return stream;
}


// TODO: hash


#endif




namespace cmoh {
    typedef basic_string_view<char> string_view;
    typedef basic_string_view<wchar_t> wstring_view;
    typedef basic_string_view<char16_t> u16string_view;
    typedef basic_string_view<char32_t> u32string_view;
}




#endif
