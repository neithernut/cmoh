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
#ifndef CMOH_SELECTABLE_ITEMS_HPP__
#define CMOH_SELECTABLE_ITEMS_HPP__

#include <type_traits>
#include <utility>


namespace cmoh {


/**
 * Container holding various items, which can be retrieved via criteria
 *
 * Use this container if you have a bunch of items of which you want to select
 * one using some criteria.  The container is constructible from values of
 * arbitrary types.
 *
 * At compile time, those items can be accessed through overloads of the variadic
 * method template `get()`, which takes as template parameters types featuring a
 * member `value` which is either `true` or `false`. The use case this template
 * was designed for is when you want to select one of the items based on some
 * meta function. Example:
 *
 *     template <typename ...Types> struct foo {
 *         selectable_items<Types...> bar;
 *         // ...
 *         void meth() {
 *             bar.get<has_some_feature<Types>...>();
 *         }
 *     };
 *
 * Currently, only the retrieval of const references is supported.
 */
template <
    typename ...Types ///< types of the items held by the container
>
struct selectable_items {
    template <typename...> using type_of = void;
    template <typename...> void get() {}

    /**
     * Visit all items, optionally matching some compile time critirea
     */
    template <
        typename Function,
        typename ...BoolTypes
    >
    void
    visit(
        Function&& func ///< function to apply
    ) const {}
};

// Specialization for at least one parameter
template <
    typename Type0,
    typename ...Types
>
struct selectable_items<Type0, Types...> {
    typedef Type0 value;
    typedef selectable_items<Types...> next;


private:
    // members have to be declares before get() because of decltype usage
    value _value;
    next _next;


    // helper for type_of
    template <typename ...BoolTypes>
    struct type_of_helper;

    template <typename BoolType0, typename ...BoolTypes>
    struct type_of_helper<BoolType0, BoolTypes...> : std::conditional<
        BoolType0::value,
        value,
        typename next::template type_of<BoolTypes...>
    > {};

public:
    template <
        typename ...BoolTypes
    >
    using type_of = typename type_of_helper<BoolTypes...>::type;


    selectable_items(value&& current, Types... values) :
            _value(std::forward<value>(current)),
            _next(std::forward<Types>(values)...) {};
    selectable_items(selectable_items const&) = default;
    selectable_items(selectable_items&&) = default;
    selectable_items() = delete;


    template <
        typename BoolType0,
        typename ...BoolTypes
    >
    typename std::enable_if<
        BoolType0::value,
        value const&
    >::type
    get() const noexcept {
        return _value;
    }

    template <
        typename BoolType0,
        typename ...BoolTypes
    >
    typename std::enable_if<
        !BoolType0::value,
        typename next::template type_of<BoolTypes...>
    >::type
    get() const noexcept {
        return _next.template get<BoolTypes...>();
    }


    template <
        typename Function,
        typename BoolType0 = std::true_type,
        typename ...BoolTypes
    >
    typename std::enable_if<BoolType0::value>::type
    visit(
        Function&& func
    ) const {
        func(_value);
        _next.template visit<Function, BoolTypes...>(
            std::forward<Function>(func)
        );
    }

    template <
        typename Function,
        typename BoolType0 = std::true_type,
        typename ...BoolTypes
    >
    typename std::enable_if<!BoolType0::value>::type
    visit(
        Function&& func
    ) const {
        _next.template visit<Function, BoolTypes...>(
            std::forward<Function>(func)
        );
    }


    template <
        typename Function,
        typename BoolType0 = std::true_type,
        typename ...BoolTypes
    >
    typename std::enable_if<BoolType0::value>::type
    visit(
        Function&& func
    ) {
        func(_value);
        _next.template visit<Function, BoolTypes...>(
            std::forward<Function>(func)
        );
    }

    template <
        typename Function,
        typename BoolType0 = std::true_type,
        typename ...BoolTypes
    >
    typename std::enable_if<!BoolType0::value>::type
    visit(
        Function&& func
    ) {
        _next.template visit<Function, BoolTypes...>(
            std::forward<Function>(func)
        );
    }
};


}


#endif
