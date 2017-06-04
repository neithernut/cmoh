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
#include "person.hpp"


person::person(std::chrono::system_clock::time_point const& birthday) :
    _birthday(birthday) {}

std::string person::first_name() const { return _first_name; }
void person::set_first_name(std::string const& name) { _first_name = name; }

std::string person::last_name() const { return _last_name; }
void person::set_last_name(std::string const& name) { _last_name = name; }

std::chrono::hours person::age() const {
    auto const diff = decltype(_birthday)::clock::now() - _birthday;
    return std::chrono::duration_cast<std::chrono::hours>(diff);
}

