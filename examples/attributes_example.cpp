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
#include <cassert>
#include <iostream>
#include <sstream>

// CMOH includes
#include <cmoh/accessor_bundle.hpp>
#include <cmoh/attribute.hpp>
#include <cmoh/factory.hpp>

// local includes
#include "person.hpp"




// We declare attributes we want to access. This may as well be done in a
// separate header.

// We need to specify some datatype we use for addressing objects' properties.
// In this example, we use an enumeration.
enum attribute {birthday, first_name, last_name, full_name, capitals, age};

// We now declare the attributes. Each attribute has a key and a type. The
// attributes "age" and "birthday" are const, which means they cannot be set.
// They may, however, still be used in construction.
using birthday_attr = cmoh::attribute<attribute, birthday,
    const std::chrono::system_clock::time_point>;
using first_name_attr = cmoh::attribute<attribute, first_name, std::string>;
using last_name_attr = cmoh::attribute<attribute, last_name, std::string>;
using full_name_attr = cmoh::attribute<attribute, full_name, const std::string>;
using capitals_attr = cmoh::attribute<attribute, capitals, const std::string>;
using age_attr = cmoh::attribute<attribute, age, const std::chrono::hours>;


static
auto
get_full_name(person const& p) {
    std::ostringstream buf;
    buf << p.first_name() << " " << p.last_name();
    return buf.str();
}




int main(int argc, char* argv[]) {
    // From the attributes we pull accessors for a concrete C++ type and bundle
    // them in an accessor bundle. Note: "age" is actually not an attribute in
    // `person` but a dependent value. However, as the attribute specified above
    // is const anyways, we can just supply the calculation method as the
    // getter.
    auto accessors = bundle(
        cmoh::factory<person, birthday_attr>(),
        first_name_attr::accessor<person>(&person::first_name, &person::set_first_name),
        last_name_attr::accessor<person>(&person::last_name, &person::set_last_name),
        full_name_attr::accessor<person>(&get_full_name),
        capitals_attr::accessor<person>([] (person const& p) {
            std::ostringstream buf;
            buf << p.first_name()[0] << "." << p.last_name()[0] << ".";
            return buf.str();
        }),
        age_attr::accessor<person>(&person::age)
    );

    // Using the bundle, we can create objects. In this case, we use the factory
    // we supplied to the bundle, which creates an object from a birthday using
    // the regular constructor. The name is set using the setter.
    person p = accessors.create<birthday, first_name, last_name>(
        std::chrono::system_clock::now() - std::chrono::hours(24),
        "Hans",
        "Wurst"
    );

    cmoh::accessors::attribute::make_accessor<full_name_attr, person, std::string>(&get_full_name);

    // We can read attributes from a real class via the accessor bundle
    std::cout << "name: " << accessors.get<first_name>(p) << " " << accessors.get<last_name>(p) << std::endl;
    std::cout << "Age: " << accessors.get<age>(p).count() << " hours" << std::endl;
    assert((accessors.get<first_name>(p) == "Hans") && (accessors.get<last_name>(p) == "Wurst"));

    // We can also set attributes via the bundle
    std::cout << "Setting name..." << std::endl;
    accessors.set<first_name>(p, "Henrick");

    // We can also query atrificial attributes retrieved by other functions
    std::cout << "Name: " << accessors.get<full_name>(p) << std::endl;
    std::cout << "Age: " << accessors.get<age>(p).count() << " hours" << std::endl;
    assert(accessors.get<full_name>(p) == "Henrick Wurst");

    // ... or lambdas
    std::cout << "Capitals: " << accessors.get<capitals>(p) << std::endl;

    return 0;
}

