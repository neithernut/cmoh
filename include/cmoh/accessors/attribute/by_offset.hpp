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
#ifndef CMOH_ATTRIBUTE_BY_OFFSET_HPP__
#define CMOH_ATTRIBUTE_BY_OFFSET_HPP__


// std includes
#include <cstddef>
#include <type_traits>


namespace cmoh {
namespace accessors {
namespace attribute {


/**
 * Attribute accessor using methods
 *
 * Access an attribute by the offset within the C++ type targeted. Use is
 * generally not recommended. This accessor is primarily included for legacy
 * types and structs from C interfaces, which do not provide getters and
 * setters in constrained environments.
 *
 * Users are discouraged from constructing accessors directly. Use one of the
 * `accessor()` overloads provided by the attribute instead.
 */
template <
    typename Attribute, ///< attribute type being accessed
    typename ObjType, ///< type of the class or struct with the attribute
    typename Value ///< type of the attribute in the class
>
struct by_offset {
    typedef Attribute property; ///< type of property being accessed
    typedef ObjType object_type; ///< object being accessed
    typedef Value value_type; ///< actual type within the object


    static_assert(
        std::is_convertible<object_type, typename property::type>::value,
        "Attribute type and real type not compatible."
    );


    by_offset(std::size_t offset) : _offset(offset) {};
    by_offset(by_offset const&) = default;
    by_offset(by_offset&&) = default;

    /**
     * Get the attribute from an object
     *
     * \returns the attribute's value
     */
    typename property::type
    get(
        object_type const& obj ///< object from which to get the value
    ) const {
        return *reinterpret_cast<value_type const*>(
            reinterpret_cast<char const*>(&obj) + _offset
        );
    }

    /**
     * Set the attribute on an object
     */
    void
    set(
        object_type& obj, ///< object on which to set the attribute
        typename property::type&& value ///< value to set
    ) const {
        *reinterpret_cast<value_type*>(
            reinterpret_cast<char*>(&obj) + _offset
        ) = value;
    }


private:
    std::size_t _offset;
};


// accessor factory overlaod for the `cmoh::accessor::attribute::by_offset`
template <
    typename Attribute, ///< attribute being accessed
    typename ObjType, ///< type of the class or struct with the attribute
    typename Value ///< type of the value in the concrete C++ type
>
constexpr
by_offset<Attribute, ObjType, Value>
make_accessor(
    std::size_t offset
) {
    return by_offset<Attribute, ObjType, Value>(offset);
}


}
}
}




#endif
