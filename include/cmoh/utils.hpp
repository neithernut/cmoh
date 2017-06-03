/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016, 2017 Julian Ganz
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

#include <type_traits>
#include <utility>


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
    enum : bool {value = Item0::value && conjunction<Items...>::value};
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
    enum : bool {value = Item0::value || disjunction<Items...>::value};
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
 * All `Types` that are identical to `void` are ignored. If all the remaining
 * `Types` are identical, the type will be exported as the member `type`.
 * If not all parameters are identical or `void`, a compile time error will be
 * issued.
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
        std::is_same<typename common_type<Types...>::type, type>::value ||
        std::is_void<typename common_type<Types...>::type>::value,
        "Types are not identical"
    );
};

// Specialization for the void type
template <
    typename ...Types
>
struct common_type<void, Types...> {
    typedef typename common_type<Types...>::type type;
};

// Specialization for exactly one parameter
template <
    typename Type
>
struct common_type<Type> {
    typedef Type type;
};

// Otherwise pointless specialization to avoid "ambogious specialization"
template <>
struct common_type<void> {
    typedef void type;
};


/**
 * Check whether a value is an object callable with a number of arguments
 *
 */
template <
    typename Type,
    typename ...Args
>
struct is_callable_with {
private:
    template <
        typename T,
        typename = void
    >
    struct helper : std::false_type {};

    template <typename T>
    struct helper<T, void_t<decltype(std::declval<T>()(std::declval<Args>()...))>> :
        std::true_type {};
public:
    enum : bool {value = helper<Type>::value};
};


/**
 * Partial predeclaration of C++17 std::invoke()
 *
 * This overloaded function is a partial predeclaration of the C++17 STL
 * function `std::invoke()`. Unlike in the C++17 version,
 * `std::reference_wrapper<>` types are not supported as first arguments.
 */
template <
    typename Func,
    typename ...Args
>
typename std::enable_if<
    is_callable_with<Func, Args...>::value,
    decltype(std::declval<Func>()(std::declval<Args>()...))
>::type
invoke(
    Func&& func,
    Args&&... args
) {
    return std::forward<Func>(func)(std::forward<Args>(args)...);
}

// Specialization for methods invoked on a regular object
template <
    typename Func,
    typename Class,
    typename Arg0,
    typename ...Args
>
typename std::enable_if<
    std::is_member_function_pointer<Func Class::*>::value &&
    std::is_base_of<Class, typename std::decay<Arg0>::type>::value,
    decltype((std::declval<Arg0>().*std::declval<Func Class::*>())(std::declval<Args>()...))
>::type
invoke(
    Func Class::* func,
    Arg0&& arg0,
    Args&&... args
) {
    return (std::forward<Arg0>(arg0).*func)(std::forward<Args>(args)...);
}

// Specialization for methods invoked on an intermediate referring to the object
template <
    typename Func,
    typename Class,
    typename Arg0,
    typename ...Args
>
typename std::enable_if<
    std::is_member_function_pointer<Func Class::*>::value,
    decltype(((*std::declval<Arg0>()).*std::declval<Func Class::*>())(std::declval<Args>()...))
>::type
invoke(
    Func Class::* func,
    Arg0&& arg0,
    Args&&... args
) {
    return ((*std::forward<Arg0>(arg0)).*func)(std::forward<Args>(args)...);
}

// Specialization for attributes invoked on a regular object
template <
    typename Member,
    typename Class,
    typename Arg
>
typename std::enable_if<
    std::is_member_object_pointer<Member Class::*>::value &&
    std::is_base_of<Class, Arg>::value,
    decltype(std::declval<Arg>().*std::declval<Member>())
>::type
invoke(
    Member Class::* member,
    Arg&& arg
) {
    return std::forward<Arg>(arg).*member;
}

// Specialization for attributes invoked on an intermediate referring to the object
template <
    typename Member,
    typename Class,
    typename Arg
>
typename std::enable_if<
    std::is_member_object_pointer<Member Class::*>::value,
    decltype((*std::declval<Arg>()).*std::declval<Member>())
>::type
invoke(
    Member Class::* member,
    Arg&& arg
) {
    return (*std::forward<Arg>(arg)).*member;
}


}
}


#endif
