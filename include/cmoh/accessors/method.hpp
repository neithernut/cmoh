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
#ifndef CMOH_METHOD_METHOD_HPP__
#define CMOH_METHOD_METHOD_HPP__


// std includes
#include <type_traits>
#include <utility>


// local includes
#include <cmoh/utils.hpp>


namespace cmoh {
namespace accessors {
namespace method {


/**
 * Method accessor
 *
 * This accessor provides access to methods.
 *
 * Users are discouraged from constructing accessors directly. Use
 * one of the `accessor()` overloads provided by the method instead.
 */
template <
    typename Method, ///< method being accessed
    typename ObjType, ///< type of the class or struct affected
    typename Func ///< type of the invoable implementing the method
>
struct invocable {
    typedef Method property; ///< type of property being accessed
    typedef ObjType object_type; ///< object being accessed

    typedef Func func; ///< the actual invocable
    typedef typename std::conditional<
        property::mutates,
        object_type&,
        object_type const&
    >::type object_ref_type; ///< the type of reference to be passed for the obj


    invocable(func&& f) : _helper(std::forward<func>(f)) {};
    invocable(invocable const&) = default;
    invocable(invocable&&) = default;


    /**
     * Call the method
     *
     */
    template <
        typename ...Args
    >
    typename property::return_type
    call(
        object_ref_type obj,
        Args&&... args
    ) const {
        return _helper(obj, std::forward<Args>(args)...);
    }

private:
    /**
     * Wrapper for the actual call
     *
     * We use this wrapper in order to generate a static argument list from the
     * property, rather from arguments passed at the call site.
     */
    template <
        typename ...Args
    >
    struct helper {
        helper(func&& f) : _func(std::forward<func>(f)) {};
        helper(helper const&) = default;
        helper(helper&&) = default;

        typename property::return_type
        operator ()(
            object_ref_type obj,
            Args&&... args
        ) const {
            return util::invoke(_func, obj, std::forward<Args>(args)...);
        }

        func _func;
    };

    // specialization for interpreting an encapsulated type list
    template <
        typename ...Args
    >
    struct helper<util::types<Args...>> : helper<Args...> {};

    helper<typename property::arguments> _helper;
};


// accessor factory overlaods for the `cmoh::accessor::method::invocable`
template <
    typename Method, ///< method being accessed
    typename ObjType, ///< type of the class or struct affected
    typename Func ///< type of the invoable implementing the method
>
constexpr
auto
make_accessor(
    Func&& func
) {
    return invocable<Method, ObjType, Func>(
        std::forward<Func>(func)
    );
}


}
}
}



#endif
