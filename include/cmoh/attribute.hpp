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

    /**
     * Binding of an attribute to a concrete object type
     *
     * Each instantiation of this template represent one bindings between an
     * attribute and an existing (real) C++ type. The binding itself does
     * nothing but feature functionality for defining accessors for the
     * attributes.
     *
     * These accessors must provide a method for getting the attribute's value
     * from the object and may provide a method for setting the attribute:
     *
     *     type get(object_type const& obj) const
     *     void set(object_type&, type&& value) const
     *
     * Where `object_type` is the C++ type for which the binding was created
     * and `type` is the type of the attribute (e.g. `int`).
     */
    template <
        typename ObjType ///< type of the class or struct with the attribute
    >
    struct binding {
        typedef ObjType object_type;

        typedef type(object_type::* getter)() const;

        /**
         * Attribute accessor using methods
         */
        template <
            getter Getter,
            void(ObjType::* Setter)(type const&) = nullptr
        >
        struct methods {
            typedef attribute attr; ///< attribute being accessed
            typedef ObjType object_type; ///< object being accessed

            /**
             * Get the attribute from an object
             *
             * \returns the attribute's value
             */
            type
            get(
                object_type const& obj ///< object from which to get the value
            ) const {
                return (obj.*Getter)();
            }

            /**
             * Set the attribute on an object
             */
            void
            set(
                object_type& obj, ///< object on which to set the attribute
                type&& value ///< value to set
            ) const {
                static_assert(Setter != nullptr, "No setter defined for this attribute");
                (obj.*Setter)(std::forward(value));
            }
        };
    };
};
}


#endif
