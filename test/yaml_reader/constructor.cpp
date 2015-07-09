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


#include "catch/catch.hpp"
#define private public  // Hack into yaml_reader::handlers
#include <yaml_reader.hpp>
#undef private


using namespace libyaml;


TEST_CASE("Reader constructors", "[reader][constructor]") {
	yaml_handler empty_handler;

	REQUIRE(yaml_reader().handlers.size() == 0);
	REQUIRE(yaml_reader({yaml_handler()}).handlers.size() == 1);
	REQUIRE(yaml_reader({empty_handler, yaml_handler()}).handlers.size() == 2);

	{
		yaml_reader rdr;
		rdr.append_handler(empty_handler);
		rdr.append_handler(yaml_handler());
		REQUIRE(rdr.handlers.size() == 2);
	}
}
