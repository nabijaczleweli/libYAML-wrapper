// The MIT License (MIT)

// Copyright (c) 2015 nabijaczleweli

//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.


#pragma once
#ifndef UTIL_FILE_HPP
#define UTIL_FILE_HPP


#include <type_traits>


namespace libyaml {
	namespace util {
		template <class NameT, class = std::enable_if_t<std::is_class<NameT>::value>>
		bool exists(const NameT & name) noexcept(noexcept(name.data()));
		bool exists(const char * name) noexcept;
	}
}


template <class NameT, class>
bool libyaml::util::exists(const NameT & name) noexcept(noexcept(name.data())) {
	if(FILE * file = fopen(name.data(), "r")) {
		fclose(file);
		return true;
	} else
		return false;
}

bool libyaml::util::exists(const char * name) noexcept {
	if(FILE * file = fopen(name, "r")) {
		fclose(file);
		return true;
	} else
		return false;
}


#endif  // UTIL_FILE_HPP
