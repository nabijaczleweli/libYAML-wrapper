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


#include "bandit/bandit.h"
#include "../util/throw.hpp"
#define private public
#include <yaml_parser.hpp>
#undef private
#include <cstdio>
#include <fstream>
#include <iostream>


using namespace std;
using namespace std::experimental;
using namespace bandit;
using namespace libyaml;


go_bandit([] {
	describe("parser", [&] {
		const decltype(declval<yaml_parser>().input_file) no_file;

		describe("read()", [&] {
			it("throws on nonexistant file", [&] {
				yaml_parser parser;
				AssertThrows(ios_base::failure, parser.read_from_file(tmpnam(nullptr)));
				AssertThat(parser.input_buffer, Is().EqualTo(nullopt));
				AssertThat(parser.input_file, Is().EqualTo(no_file));
				AssertThat(parser.has_input(), Is().EqualTo(false));
			});

			it("opens existing files", [&] {
				const auto name = tmpnam(nullptr);
				ofstream{name};
				yaml_parser parser;
				AssertNothrow(parser.read_from_file(name));
				AssertThat(parser.input_buffer, Is().EqualTo(nullopt));
				AssertThat(parser.input_file, Is().Not().EqualTo(no_file));
				AssertThat(parser.has_input(), Is().EqualTo(true));
				remove(name);
			});

			it("opens data of length 0", [&] {
				yaml_parser parser;
				parser.read_from_data("");
				AssertThat(parser.input_buffer, Is().Not().EqualTo(nullopt));
				AssertThat(*parser.input_buffer, Is().EqualToContainer(""s));
				AssertThat(parser.input_file, Is().EqualTo(no_file));
				AssertThat(parser.has_input(), Is().EqualTo(true));
			});

			it("opens arbitrary data", [&] {
				yaml_parser parser;
				parser.read_from_data("Sample data");
				AssertThat(parser.input_buffer, Is().Not().EqualTo(nullopt));
				AssertThat(*parser.input_buffer, Is().EqualToContainer("Sample data"s));
				AssertThat(parser.input_file, Is().EqualTo(no_file));
				AssertThat(parser.has_input(), Is().EqualTo(true));
			});

			describe("reopening", [&] {
				it("throws with data already open", [&] {
					yaml_parser parser;
					parser.read_from_data("Sample data #1");
					AssertThrows(yaml_parser_exception, parser.read_from_data("Sample data #2"));
					AssertThrows(yaml_parser_exception, parser.read_from_file(tmpnam(nullptr)));
				});

				it("throws with file already open", [&] {
					const auto name = tmpnam(nullptr);
					ofstream{name};
					yaml_parser parser;
					parser.read_from_file(name);
					AssertThrows(yaml_parser_exception, parser.read_from_data("Sample data"));
					AssertThrows(yaml_parser_exception, parser.read_from_file(tmpnam(nullptr)));
					remove(name);
				});
			});
		});
	});
});
