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
#ifndef CMOH_ACCESSORS_FACTORY_ABSTRACT_FACTORY_HPP__
#define CMOH_ACCESSORS_FACTORY_ABSTRACT_FACTORY_HPP__


// std includes
#include <utility>

// local includes
#include <cmoh/utils.hpp>
#include <cmoh/accessors/utils.hpp>


namespace cmoh {
namespace accessors {
namespace factory {


/**
 * Object factory wrapper base
 *
 * This template provides a few utilities and other members which are expected
 * from a factory.
 */
template <
    typename ...Attributes ///< attributes fed to the constructor
>
struct abstract_factory {
    /**
     * The common key type of all attributes used
     */
    typedef typename util::common_type<
        typename property<Attributes>::type::key_type...
    >::type key_type;

    /**
     * Check whether the constructor can be invoked using some attributes
     *
     * Instantiations provide a member `value` which is `true` if the attributes
     * are sufficient for constructing an object and false otherwise.
     */
    template <
        key_type ...keys ///< keys of attributes availible for construction
    >
    using is_initializable_from = util::conjunction<
        accesses<Attributes, key_type, keys...>...
    >;

    /**
     * Check whether a specific attribute is used by this constructor
     *
     * Instantiations provide a member `value` which is `true` if the attribute
     * is used for constructing an object and false otherwise.
     */
    template <
        key_type key ///< key of attribute to check
    >
    using uses = util::disjunction<accesses<Attributes, key_type, key>...>;

    /**
     * Get the one property having a specific key
     *
     * Provides either the attribute having the provided `key` or void via the
     * member `type`.
     */
    template <
        key_type key
    >
    using property_by_key = util::common_type<
        typename std::conditional<
            Attributes::key == static_cast<decltype(Attributes::key)>(key),
            Attributes,
            void
        >::type...
    >;
};


}
}
}


#endif
