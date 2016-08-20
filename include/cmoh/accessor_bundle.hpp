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
#ifndef CMOH_ACCESSOR_BUNDLE_HPP__
#define CMOH_ACCESSOR_BUNDLE_HPP__


// std includes
#include <type_traits>
#include <utility>

// local includes
#include <cmoh/selectable_items.hpp>
#include <cmoh/utils.hpp>


namespace cmoh {


/**
 * Check whether a type can construct an object from some attributes
 *
 * This checks whether the `SupposedConstructor` features a template with the
 * name `is_initializable_from`. If it does, it evaluates that template with
 * the `PassedAttributes` and presents that result via the static member
 * `value`. Otherwise, `value` is `false`.
 */
template <
    typename SupposedConstructor, ///< type which _may_ be a constructor type
    typename ...PassedAttributes ///< attributes available for construction
>
struct is_initializable_from {
private:
    /**
     * Check whether a type can be used to initialize anything
     *
     * This metafunction effectively checks whether a type features a template with
     * the name `is_initializable_from` for which an instantiation with zero
     * parameters exist.
     */
    template <
        typename T, ///< type which may feature the template
        typename = void
    >
    struct has_is_initializable_from : std::false_type {};

    // specialization for types featuring the name
    template <
        typename T
    >
    struct has_is_initializable_from<
        T,
        util::void_t<typename T::template is_initializable_from<>>
    > : std::true_type {};


    /**
     * Internal helper for selective evaluation of is_initializable_from
     *
     * Evaluates `T::is_initializable_from<Attributes...>` only if `flag` is
     * `true`.
     */
    template <
        typename T, ///< type featuring `is_initializable_from`
        bool flag, ///< flag controlling the evaluation
        typename ...Attributes ///< attributes to pass as template parameters
    >
    struct helper : std::false_type {};

    // specialization for evaluating the template
    template <
        typename T,
        typename ...Attributes
    >
    struct helper<T, true, Attributes...> :
        T::template is_initializable_from<Attributes...> {};

public:
    static constexpr bool value = helper<
        SupposedConstructor,
        has_is_initializable_from<SupposedConstructor>::value,
        PassedAttributes...
    >::value;
};


/**
 * Check whether an accessor accesses a specific attribute
 *
 * This checks whether the accessor provided features a type `attribute`
 * identical to the attribute provided.
 */
template <
    typename Accessor, ///< accessor to test for the attribute
    typename Attribute, ///< attribute to be accessed
    typename = void
>
struct accesses_attribute : std::false_type {};

// Specialization for accessors featuring a type `attr`
template <
    typename Accessor,
    typename Attribute
>
struct accesses_attribute<
    Accessor,
    Attribute,
    util::void_t<typename Accessor::attribute>
> : std::is_same<Attribute, typename Accessor::attribute> {};


/**
 * Accessor bundle
 *
 * Instantiations of this template bundle accessors and make them conveniently
 * accessible as a group, posing as an abstraction layer.
 *
 * TODO: further explanation
 *
 * Users are discouraged from constructing accessor bundles directly. Use
 * `bundle()` instread as a factory.
 */
template <
    typename ...Accessors
>
struct accessor_bundle {
    typedef
        typename util::common_type<typename Accessors::object_type...>::type
        object_type;


    accessor_bundle(Accessors... accessors) :
            _accessors(std::forward<Accessors>(accessors)...) {}
    accessor_bundle(accessor_bundle const&) = default;
    accessor_bundle(accessor_bundle&&) = default;
    accessor_bundle() = delete;


    /**
     * Set the value of a specific attribute on an object
     */
    template <
        typename ...Attributes ///< attributes from which to construct an object
    >
    object_type
    construct(
        typename Attributes::type&&... values ///< value to set
    ) const {
        auto constructor = _accessors.
            template get<is_initializable_from<Accessors, Attributes...>...>();

        // construct the object itself
        auto retval{constructor.template construct<Attributes...>(
            std::forward<typename Attributes::type>(values)...
        )};

        initialize_if_unused<decltype(constructor), Attributes...>(
            retval,
            std::forward<typename Attributes::type>(values)...
        );

        return retval;
    }


    /**
     * Get the value of a specific attribute from an object
     *
     * \returns the value of the attribute
     */
    template <
        typename Attribute ///< attribute to get
    >
    typename Attribute::type
    get(
        object_type const& obj ///< object from which to get the value
    ) const {
        return _accessors.
            template get<accesses_attribute<Accessors, Attribute>...>().
            get(obj);
    }

    /**
     * Set the value of a specific attribute on an object
     */
    template <
        typename Attribute ///< attribute to set
    >
    void
    set(
        object_type& obj, ///< object on which to set the attribute
        typename Attribute::type&& value ///< value to set
    ) const {
        _accessors.
            template get<accesses_attribute<Accessors, Attribute>...>().
            set(obj, std::forward<typename Attribute::type>(value));
    }


private:
    /**
     * Initialize attributes which are not used by a specific constructor
     */
    template <
        typename Constructor, ///< constructor to consider
        typename Attribute0, ///< first of the attributes
        typename ...Attributes ///< rest of the attributes
    >
    void
    initialize_if_unused(
        object_type& obj, ///< object on which to set the attributes' values
        typename Attribute0::type&& value0, ///< first of the values
        typename Attributes::type&&... values ///< rest of the values
    ) const {
        initialize_single_if_unused<Constructor, Attribute0>(
            obj,
            std::forward<typename Attribute0::type>(value0)
        );

        // recurse
        initialize_if_unused<Constructor, Attributes...>(
            obj,
            std::forward<typename Attributes::type>(values)...
        );
    }

    // overload for an empty list of attributes
    template <
        typename Constructor
    >
    void
    initialize_if_unused(
        object_type& obj
    ) const {}


    /**
     * Initialize a single attribute if it is not used by a specific constructor
     */
    template <
        typename Constructor, ///< constructor to consider
        typename Attribute ///< attribute to set
    >
    typename std::enable_if<!Constructor::template uses<Attribute>::value>::type
    initialize_single_if_unused(
        object_type& obj, ///< object on which to set the attribute's value
        typename Attribute::type&& value ///< value to set
    ) const {
        // TODO: static assertion for unsettable attributes
        set<Attribute>(obj, std::forward<typename Attribute::type>(value));
    }

    // overload for attributes which are used by the constructor specified
    template <
        typename Constructor,
        typename Attribute
    >
    typename std::enable_if<Constructor::template uses<Attribute>::value>::type
    initialize_single_if_unused(
        object_type& obj,
        typename Attribute::type&& value
    ) const {}


    selectable_items<Accessors...> _accessors;
};


/**
 * Construct an accessor bundle from a bunch of accessors
 *
 * \returns an accessor bundle holding all the accessors supplied
 */
template <
    typename ...Accessors
>
constexpr
const
accessor_bundle<Accessors...>
bundle(
    Accessors&&... accessors ///< accessors to bundle
) {
    return accessor_bundle<Accessors...>(std::forward<Accessors>(accessors)...);
}


}


#endif
