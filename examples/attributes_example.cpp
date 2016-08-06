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

// CMOH includes
#include <cmoh/attribute.hpp>
#include <cmoh/accessor_bundle.hpp>

// local includes
#include "person.hpp"


// At first, we declare attributes we want to access. This may be done in a
// separate header. Note that the age of a person cannot be set.
using name = cmoh::attribute<std::string>;
using age = cmoh::attribute<const unsigned int>;


int main(int argc, char* argv[]) {
    // From the attributes we pull accessors for a concrete C++ type and bundle
    // them in an accessor bundle.
    auto accessors = bundle(
        name::binding<person>::methods<&person::name, &person::set_name>(),
        age::binding<person>::methods<&person::age>()
    );

    person p;
    p.set_name("Hans");
    p.set_age(54);

    // We can read attributes from a real class via the accessor bundle
    std::cout << "Name: " << accessors.get<name>(p) << std::endl;
    std::cout << "Age: " << accessors.get<age>(p) << std::endl;

    // We can also set the attributes via the bundle
    std::cout << "Setting name..." << std::endl;
    accessors.set<name>(p, "Hans Wurst");

    // Re-read the attributes to demonstrate they have indeed changed
    std::cout << "Name: " << accessors.get<name>(p) << std::endl;
    std::cout << "Age: " << accessors.get<age>(p) << std::endl;

    return 0;
}

