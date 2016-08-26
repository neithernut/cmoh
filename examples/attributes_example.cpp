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


// std includes
#include <iostream>
#include <cassert>

// CMOH includes
#include <cmoh/accessor_bundle.hpp>
#include <cmoh/attribute.hpp>
#include <cmoh/factory.hpp>

// local includes
#include "person.hpp"

// we _always_ want assertions
#undef NDEBUG




// We declare attributes we want to access. This may as well be done in a
// separate header.

// We need to specify some datatype we use for addressing objects' properties.
// In this example, we use an enumeration.
enum attribute {birthday, name, age};

// We now declare the attributes. Each attribute has a key and a type. The
// attributes "age" and "birthday" are const, which means they cannot be set.
// They may, however, still be used in construction.
using birthday_attr = cmoh::attribute<attribute, birthday,
    const std::chrono::system_clock::time_point>;
using name_attr = cmoh::attribute<attribute, name, std::string>;
using age_attr = cmoh::attribute<attribute, age, const std::chrono::hours>;




int main(int argc, char* argv[]) {
    // From the attributes we pull accessors for a concrete C++ type and bundle
    // them in an accessor bundle. Note: "age" is actually not an attribute in
    // `person` but a dependent value. However, as the attribute specified above
    // is const anyways, we can just supply the calculation method as the
    // getter.
    auto accessors = bundle(
        cmoh::factory<person, birthday_attr>(),
        name_attr::accessor<person>(&person::name, &person::set_name),
        age_attr::accessor<person>(&person::age)
    );

    // Using the bundle, we can create objects. In this case, we use the factory
    // we supplied to the bundle, which creates an object from a birthday using
    // the regular constructor. The name is set using the setter.
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

