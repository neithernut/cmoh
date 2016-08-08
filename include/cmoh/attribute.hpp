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
#ifndef CMOH_ATTRIBUTE_HPP__
#define CMOH_ATTRIBUTE_HPP__


// std includes
#include <type_traits>
#include <utility>


namespace cmoh {


/**
 * Attribute declaration utility
 *
 * Using this template, a programmer may declare attributes she wants to make
 * accessible to the cmoh system. Use like:
 *
 *     using foo = cmoh::attribute<int>;
 *
 * Note that using `typedef` instead of `using` may produce unintended results,
 * since the cmoh system relies on each attribute declaration being another
 * type.
 *
 * The attributes themselves are not bound to a C++ type. Attributes may be
 * grouped together in namespaces or stucts, each representing an interface.
 */
template <
    typename Attr ///< type of the attribute itself
>
struct attribute {
    typedef typename std::remove_cv<Attr>::type type;
    static constexpr bool is_const = std::is_const<Attr>::value;

    template <
        typename ObjType, ///< type of the class or struct with the attribute
        typename SetterArg ///< effective type of the setter argument
    >
    struct method_accessor {
        typedef attribute attr; ///< attribute being accessed
        typedef ObjType object_type; ///< object being accessed

        typedef type(object_type::* getter)() const;
        typedef void(object_type::* setter)(SetterArg);

        method_accessor(getter getter, setter setter = nullptr)
            : _getter(getter), _setter(setter) {};
        method_accessor(method_accessor const&) = default;
        method_accessor(method_accessor&&) = default;

        /**
         * Get the attribute from an object
         *
         * \returns the attribute's value
         */
        type
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
            type&& value ///< value to set
        ) const {
            (obj.*_setter)(std::forward<type>(value));
        }
    private:
        const getter _getter;
        const setter _setter;
    };
};
}


#endif
