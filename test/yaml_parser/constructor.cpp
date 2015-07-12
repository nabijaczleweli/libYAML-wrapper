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


using namespace std;
using namespace std::experimental;
using namespace bandit;
using namespace libyaml;


go_bandit([&] {
	describe("parser", [&] {
		const decltype(declval<yaml_parser>().input_file) no_file;

		describe("constructors", [&] {
			describe("default", [&] {
				it("doesn't throw", [&] {
					AssertNothrow(yaml_parser().~yaml_parser());
				});

				it("is empty", [&] {
					AssertThat(yaml_parser().input_buffer, Is().EqualTo(nullopt));
					AssertThat(yaml_parser().input_file, Is().EqualTo(no_file));
					AssertThat(yaml_parser().has_input(), Is().EqualTo(false));
				});
			});

			describe("copy", [&] {
				const yaml_parser base;

				it("doesn't throw", [&] {
					AssertNothrow(yaml_parser(base).~yaml_parser());
				});

				it("is empty", [&] {
					AssertThat(yaml_parser(base).input_buffer, Is().EqualTo(nullopt));
					AssertThat(yaml_parser(base).input_file, Is().EqualTo(no_file));
					AssertThat(yaml_parser(base).has_input(), Is().EqualTo(false));
				});
			});

			describe("move", [&] {
				it("doesn't throw", [&] {
					AssertNothrow(yaml_parser(yaml_parser()).~yaml_parser());
				});

				it("is empty", [&] {
					AssertThat(yaml_parser(yaml_parser()).input_buffer, Is().EqualTo(nullopt));
					AssertThat(yaml_parser(yaml_parser()).input_file, Is().EqualTo(no_file));
					AssertThat(yaml_parser(yaml_parser()).has_input(), Is().EqualTo(false));
				});
			});
		});
	});
});
