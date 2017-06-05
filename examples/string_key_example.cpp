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


// we _always_ want assertions
#undef NDEBUG

// std includes
#include <iostream>
#include <cassert>

// CMOH includes
#include <cmoh/string_view.hpp>
#include <cmoh/string_utils.hpp>

#include <cmoh/accessor_bundle.hpp>
#include <cmoh/attribute.hpp>
#include <cmoh/factory.hpp>

// local includes
#include "person.hpp"




// The `cmoh::string_view` can be used for string keys. It's an adapted
// string view with compile time capabilities lacking in the STL version.
extern constexpr const cmoh::string_view birthday{"birthday"};
extern constexpr const cmoh::string_view first_name{"first_name"};
extern constexpr const cmoh::string_view last_name{"last_name"};
extern constexpr const cmoh::string_view age{"age"};

// As the attribute type, use `cmoh::string_view const&`. Otherwise, the string
// views can be used just like enumeration and integral keys.
using birthday_attr = cmoh::attribute<cmoh::string_view const&, birthday,
    const std::chrono::system_clock::time_point>;
using first_name_attr = cmoh::attribute<cmoh::string_view const&, first_name, std::string>;
using last_name_attr = cmoh::attribute<cmoh::string_view const&, last_name, std::string>;
using age_attr = cmoh::attribute<cmoh::string_view const&, age, const std::chrono::hours>;


// work-around for durations not being printable
std::ostream&
operator << (std::ostream& stream, std::chrono::hours const& value) {
    return stream << value.count() << " hours";
}

int main(int argc, char* argv[]) {
    // We create an accessor bundle.
    auto accessors = bundle(
        cmoh::factory<person, birthday_attr>(),
        first_name_attr::accessor<person>(&person::first_name, &person::set_first_name),
        last_name_attr::accessor<person>(&person::last_name, &person::set_last_name),
        age_attr::accessor<person>(&person::age)
    );

    // We can use the constants just like other key types, e.g. for object
    // construction.
    person p = accessors.create<birthday, first_name, last_name>(
        std::chrono::system_clock::now() - std::chrono::hours(24),
        "Hans",
        "Wurst"
    );

    // Having strings, we can pretty-print the object using a visitor.
    auto printer = [&] (auto accessor) {
        std::cout << cmoh::accessors::key(accessor) << ": " << accessor.get(p);
        std::cout << std::endl;
    };
    accessors.visit_properties(printer);

    std::cout << std::endl;

    // we can also address properties using strings at run time
    assert(accessors.set<std::string>(p, std::string("first_name"), "Henrick"));
    accessors.visit_properties(printer);

    return 0;
}


