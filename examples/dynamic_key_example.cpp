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

// CMOH includes
#include <cmoh/accessor_bundle.hpp>
#include <cmoh/attribute.hpp>
#include <cmoh/factory.hpp>

// local includes
#include "person.hpp"




// Like in the attribute example, we declare an attribute
using name_attr = cmoh::attribute<int, 1, std::string>;




int main(int argc, char* argv[]) {
    // We specify how to access the attribute
    auto accessors = bundle(
        name_attr::accessor<person>(&person::first_name, &person::set_first_name)
    );

    // We create a new person to play with
    person p(std::chrono::system_clock::now());

    // We give the person a name
    assert(accessors.set<std::string>(p, 1, std::string("Lisa")));

    {
        // Now we try to retrieve it
        auto name = accessors.get<std::string>(p, 1);
        std::cout << "Name: " << name.value_or("unknown") << std::endl;
        assert(name.has_value());
    }

    {
        // We want to retrieve the name again, but this time with another key
        auto name = accessors.get<std::string>(p, 2);
        std::cout << "Name: " << name.value_or("unknown") << std::endl;
        assert(!name.has_value());
    }

    return 0;
}

