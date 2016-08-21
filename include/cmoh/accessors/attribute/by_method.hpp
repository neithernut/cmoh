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
#ifndef CMOH_ATTRIBUTE_BY_METHOD_HPP__
#define CMOH_ATTRIBUTE_BY_METHOD_HPP__


// std includes
#include <type_traits>
#include <utility>


namespace cmoh {
namespace accessors {
namespace attribute {


/**
 * Attribute accessor using methods
 *
 * This accessor provides access to the attribute via methods of the target
 * C++ struct/class. It is constructed from an appropriate getter and,
 * optionally, a setter.
 *
 * Users are discouraged from constructing method accessors directly. Use
 * one of the `accessor()` overloads provided by the attribute instead.
 */
template <
    typename Attribute, ///< attribute type being accessed
    typename ObjType, ///< type of the class or struct with the attribute
    typename GetterVal, ///< type of the value returned from the getter
    typename SetterArg ///< effective type of the setter argument
>
struct by_method {
    typedef Attribute attribute; ///< type of attribute being accessed
    typedef ObjType object_type; ///< object being accessed

    typedef GetterVal(object_type::* getter)() const;
    typedef void(object_type::* setter)(SetterArg);


    static_assert(
        std::is_convertible<GetterVal, typename attribute::type>::value,
        "Value returned by getter is not convertible to attribute type"
    );
    static_assert(
        std::is_convertible<typename attribute::type, SetterArg>::value,
        "Attribute's type is not convertible to type required by setter"
    );


    by_method(getter getter, setter setter = nullptr)
        : _getter(getter), _setter(setter) {};
    by_method(by_method const&) = default;
    by_method(by_method&&) = default;

    /**
     * Get the attribute from an object
     *
     * \returns the attribute's value
     */
    typename attribute::type
    get(
        object_type const& obj ///< object from which to get the value
    ) const {
        return (obj.*_getter)();
    }

    /**
     * Set the attribute on an object
     */
    void
    set(
        object_type& obj, ///< object on which to set the attribute
        typename attribute::type&& value ///< value to set
    ) const {
        (obj.*_setter)(std::forward<typename attribute::type>(value));
    }
private:
    getter _getter;
    setter _setter;
};


// accessor factory overlaods for the `cmoh::accessor::attribute::by_method`
template <
    typename Attribute, ///< attribute being accessed
    typename ObjType, ///< type of the class or struct with the attribute
    typename Value ///< type of the value in the concrete C++ type
>
constexpr
typename std::enable_if<
    Attribute::is_const,
    by_method<Attribute, ObjType, Value, Value>
>::type
make_accessor(
    typename by_method<Attribute, ObjType, Value, Value>::getter getter
) {
    return by_method<Attribute, ObjType, Value, Value>(getter, nullptr);
}

template <
    typename Attribute, ///< attribute being accessed
    typename ObjType, ///< type of the class or struct with the attribute
    typename Value ///< type of the value in the concrete C++ type
>
constexpr
typename std::enable_if<
    !Attribute::is_const,
    by_method<Attribute, ObjType, Value, Value>
>::type
make_accessor(
    typename by_method<Attribute, ObjType, Value, Value>::getter getter,
    typename by_method<Attribute, ObjType, Value, Value>::setter setter
) {
    return by_method<Attribute, ObjType, Value, Value>(getter, setter);
}

template <
    typename Attribute, ///< attribute being accessed
    typename ObjType, ///< type of the class or struct with the attribute
    typename Value ///< type of the value in the concrete C++ type
>
constexpr
typename std::enable_if<
    !Attribute::is_const,
    by_method<Attribute, ObjType, Value, Value const&>
>::type
make_accessor(
    typename by_method<Attribute, ObjType, Value, Value const&>::getter getter,
    typename by_method<Attribute, ObjType, Value, Value const&>::setter setter
) {
    return by_method<Attribute, ObjType, Value, Value const&>(getter, setter);
}

template <
    typename Attribute, ///< attribute being accessed
    typename ObjType, ///< type of the class or struct with the attribute
    typename Value ///< type of the value in the concrete C++ type
>
constexpr
typename std::enable_if<
    !Attribute::is_const,
    by_method<Attribute, ObjType, Value, Value&&>
>::type
make_accessor(
    typename by_method<Attribute, ObjType, Value, Value&&>::getter getter,
    typename by_method<Attribute, ObjType, Value, Value&&>::setter setter
) {
    return by_method<Attribute, ObjType, Value, Value&&>(getter, setter);
}


}
}
}



#endif
