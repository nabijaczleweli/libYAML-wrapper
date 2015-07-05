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
#define private public
#include <yaml_parser.hpp>
#undef private
#include <fstream>
#include <cstdio>


using namespace std;
using namespace libyaml;


TEST_CASE("Parser read*() and constructors", "[parser]") {
	REQUIRE(yaml_parser().input_buffer.size() == 0);
	REQUIRE(yaml_parser().input_file == nullptr);

	{
		yaml_parser parser;
		REQUIRE_THROWS_AS(parser.read_from_file(tmpnam(nullptr)), ios_base::failure);
		REQUIRE(parser.input_buffer.size() == 0);
		REQUIRE(parser.input_file == nullptr);
	}

	{
		const auto name = tmpnam(nullptr);
		ofstream{name};
		yaml_parser parser;
		REQUIRE_NOTHROW(parser.read_from_file(name));
		REQUIRE(parser.input_buffer.size() == 0);
		REQUIRE(parser.input_file != nullptr);
		remove(name);
	}

	{
		yaml_parser parser;
		parser.read_from_data("");
		REQUIRE(parser.input_buffer.size() == 0);
		REQUIRE(parser.input_file == nullptr);
	}

	{
		yaml_parser parser;
		parser.read_from_data("Sample data");
		REQUIRE(parser.input_buffer.size() != 0);
		REQUIRE(parser.input_file == nullptr);
	}
}
