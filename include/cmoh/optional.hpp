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
#ifndef CMOH_OPTIONAL_HPP__
#define CMOH_OPTIONAL_HPP__


// We try to detect whether a standard `optional` is availible, but only for
// post-C++14 (we don't expect a C++14 STL to ship it).
#if __cplusplus > 201402L
# if __has_include(<optional>)
#  define has_optional 1
# elif __has_include(<experimental/optional>)
#  define has_experimental_optional 1
# endif
#endif


// Use one of the availible `optional` implementations
#ifdef has_optional
#include <optional>
namespace cmoh { template <class T> using optional = std::optional<T>; }
#elif has_experimental_optional
#include <experimental/optional>
namespace cmoh {
    template <class T> using optional = std::experimental::optional<T>;
}
#else


// std includes
#include <utility>


namespace cmoh {


/**
 * Predefinition of the C++17 std::optional type
 *
 * This template provides an optional type which is modelled after the
 * std::optional which is part of the C++17 standart proposal. Note that
 * only a subset of the interface is provided.
 *
 * For documentation, refer to the C++17 proposal or your favorite STL
 * documentation site.
 */
template <
    class T
>
class optional {
public:
    typedef T value_type;


    constexpr optional() : _has_data(false) {}

    optional(optional const& other) : _has_data(other.has_value()) {
        if (_has_data)
            construct(*(other.value_ptr()));
    }

    optional(optional&& other) : _has_data(other.has_value()) {
        if (_has_data)
            construct(std::move(*(other.value_ptr())));
    }

    constexpr optional(value_type const& value) : _has_data(true) {
        construct(value);
    }

    constexpr optional(value_type&& value) : _has_data(true) {
        construct(std::move(value));
    }


    ~optional() {
        destruct();
    }


    optional& operator=(optional const& other) {
        destruct();
        if ((_has_data = other.has_value()))
            construct(*(other.value_ptr()));

        return *this;
    }

    optional& operator=(optional&& other) {
        destruct();
        if ((_has_data = other.has_value()))
            construct(std::move(*(other.value_ptr())));

        return *this;
    }

    template <
        typename U
    >
    optional& operator=(U&& value) {
        destruct();
        construct(std::forward<U>(value));
        _has_data = true;

        return *this;
    }


    constexpr const value_type* operator->() const {
        return value_ptr();
    }

    constexpr value_type* operator->() {
        return value_ptr();
    }

    constexpr value_type const& operator*() const {
        return *value_ptr();
    }

    constexpr value_type& operator*() {
        return *value_ptr();
    }


    constexpr explicit operator bool() const noexcept {
        return has_value();
    }

    constexpr bool has_value() const noexcept {
        return _has_data;
    }


    template <typename U>
    constexpr T value_or(U&& default_value) const& {
        if (has_value())
            return **this;
        return static_cast<T>(std::forward<U>(default_value));
    }

    template <typename U>
    constexpr T value_or(U&& default_value) && {
        if (has_value())
            return std::move(**this);
        return static_cast<T>(std::forward<U>(default_value));
    }


    void reset() noexcept {
        destruct();
        _has_data = false;
    }


    template <typename ...Args>
    void emplace(Args&&... args) {
        destruct();
        construct(std::forward<Args>(args)...);
        _has_data = true;
    }

    template <typename U, typename ...Args>
    std::enable_if<
        std::is_constructible<
            value_type,
            std::initializer_list<U>&,
            Args&&...
        >::value
    >
    emplace(std::initializer_list<U> ilist, Args&&... args) {
        destruct();
        construct(ilist, std::forward<Args>(args)...);
        _has_data = true;
    }


private:
    template <
        typename ...Args
    >
    void construct(Args&&... args) {
        new(value_ptr()) value_type(std::forward<Args>(args)...);
    }

    void destruct() noexcept {
        if (has_value())
            value_ptr()->value_type::~value_type();
    }


    constexpr value_type const* value_ptr() const noexcept {
        return reinterpret_cast<value_type const*>(&_data[0]);
    }

    constexpr value_type* value_ptr() noexcept {
        return reinterpret_cast<value_type*>(&_data[0]);
    }


    char _data[sizeof(T)];
    bool _has_data;
};


}



#endif
#endif
