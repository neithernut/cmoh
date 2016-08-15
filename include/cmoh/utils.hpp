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


/**
 * Test whether a type occurs in a template parameter pack
 *
 * Instantiations of this struct contain a static member `value`, which will
 * be `true` if `Compare` occurs in `Types` and false otherwise.
 */
template <
    typename Compare, ///< Type to compare against items in `Types`
    typename ...Types ///< Types against which to compare `Compare` against
>
using contains = disjunction<std::is_same<Compare, Types>...>;


/**
 * Predeclaration of the C++17 std::void_t
 *
 * Used for SFINAE.
 */
template <
    typename...
>
using void_t = void;


/**
 * Meta type extracting the common type of a parameter pack
 *
 * If all `Types` are identical, the type will be exported as the member `type`.
 * If not all parameters are identical, a compile time error will be issued.
 */
template <
    typename ...Types ///< types to unify
>
struct common_type {
    static_assert(count<Types...>::value > 0, "Parameter pack is empty");
};

// Specialization for more than one parameter
template <
    typename Type0,
    typename ...Types
>
struct common_type<Type0, Types...> {
    typedef Type0 type;
    static_assert(
        std::is_same<type, typename common_type<Types...>::type>::value,
        "Types are not identical"
    );
};

// Specialization for exactly one parameter
template <
    typename Type
>
struct common_type<Type> {
    typedef Type type;
};


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
    template <typename...> void get() {}
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


public:
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
        const value&
    >::type
    get() const {
        return _value;
    }

    template <
        typename BoolType0,
        typename ...BoolTypes
    >
    typename std::enable_if<
        !BoolType0::value,
        decltype(_next.template get<BoolTypes...>())
    >::type
    get() const {
        return _next.template get<BoolTypes...>();
    }
};


}
}


#endif
