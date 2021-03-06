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
#include <yaml_reader.hpp>
#include <vector>
#include <list>


using namespace std;
using namespace bandit;
using namespace libyaml;


#define COPIES_FROM_CONTAINER(container)                          \
	"copies handlers from " #container, [&] {                       \
		container<yaml_handler> handlers;                             \
		AssertThat(yaml_reader(handlers).handlers(), Is().EqualTo(0u));     \
                                                                  \
		handlers.emplace_back();                                      \
		AssertThat(yaml_reader(handlers).handlers(), Is().EqualTo(1u)); \
                                                                  \
		handlers.emplace_back(empty_handler);                         \
		AssertThat(yaml_reader(handlers).handlers(), Is().EqualTo(2u)); \
	}


go_bandit([] {
	describe("reader", [&] {
		yaml_handler empty_handler;

		describe("constructors", [&] {
			it("is empty by default", [&] {
				AssertThat(yaml_reader().handlers(), Is().EqualTo(0u));
				AssertThat(yaml_reader().has_input(), Is().EqualTo(false));
			});

			it("copies handlers from init-list", [&] {
				AssertThat(yaml_reader({}).handlers(), Is().EqualTo(0u));
				AssertThat(yaml_reader({yaml_handler()}).handlers(), Is().EqualTo(1u));
				AssertThat(yaml_reader({empty_handler, yaml_handler()}).handlers(), Is().EqualTo(2u));
			});

			it(COPIES_FROM_CONTAINER(vector));
			it(COPIES_FROM_CONTAINER(list));

			it("moves", [&] {
				AssertThat(yaml_reader(yaml_reader({})).handlers(), Is().EqualTo(0u));
				AssertThat(yaml_reader(yaml_reader({empty_handler})).handlers(), Is().EqualTo(1u));
				AssertThat(yaml_reader(yaml_reader({empty_handler, empty_handler})).handlers(), Is().EqualTo(2u));
			});
		});

		it("appends handlers", [&] {
			yaml_reader reader;
			AssertThat(reader.handlers(), Is().EqualTo(0u));

			reader.append_handler(empty_handler);
			AssertThat(reader.handlers(), Is().EqualTo(1u));

			reader.append_handler(yaml_handler());
			AssertThat(reader.handlers(), Is().EqualTo(2u));
		});
	});
});
