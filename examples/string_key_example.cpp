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


// we _always_ want assertions
#undef NDEBUG

// std includes
#include <iostream>
#include <cassert>

// CMOH includes
#include <cmoh/string_view.hpp>

#include <cmoh/accessor_bundle.hpp>
#include <cmoh/attribute.hpp>
#include <cmoh/factory.hpp>

// local includes
#include "person.hpp"




// The `cmoh::string_view` can be used for string keys. It's an adapted
// string view with compile time capabilities lacking in the STL version.
extern constexpr const cmoh::string_view birthday{"birthday"};
extern constexpr const cmoh::string_view name{"name"};
extern constexpr const cmoh::string_view age{"age"};

// As the attribute type, use `cmoh::string_view const&`. Otherwise, the string
// views can be used just like enumeration and integral keys.
using birthday_attr = cmoh::attribute<cmoh::string_view const&, birthday,
    const std::chrono::system_clock::time_point>;
using name_attr = cmoh::attribute<cmoh::string_view const&, name, std::string>;
using age_attr = cmoh::attribute<cmoh::string_view const&, age, const std::chrono::hours>;


int main(int argc, char* argv[]) {
    auto accessors = bundle(
        cmoh::factory<person, birthday_attr>(),
        name_attr::accessor<person>(&person::name, &person::set_name),
        age_attr::accessor<person>(&person::age)
    );


    person p = accessors.create<birthday, name>(
        std::chrono::system_clock::now() - std::chrono::hours(24),
        "Hans"
    );

    // We can read attributes from a real class via the accessor bundle
    std::cout << "Name: " << accessors.get<name>(p) << std::endl;
    std::cout << "Age: " << accessors.get<age>(p).count() << " hours" << std::endl;
    assert(accessors.get<name>(p) == "Hans");

    // We can also set attributes via the bundle
    std::cout << "Setting name..." << std::endl;
    accessors.set<name>(p, "Hans Wurst");

    // ...
    std::cout << "Name: " << accessors.get<name>(p) << std::endl;
    std::cout << "Age: " << accessors.get<age>(p).count() << " hours" << std::endl;
    assert(accessors.get<name>(p) == "Hans Wurst");

    return 0;
}

