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
#include "util/reference_wrapper.hpp"


using namespace std;
using namespace bandit;
using namespace libyaml::util;


go_bandit([] {
	describe("reference wrapper", [&] {
		describe("get()", [&] {
			it("returns identity reference", [&] {
				int f00 = 0;
				AssertThat(&all_reference_wrapper<int>(f00).get(), Is().EqualTo(&f00));
				AssertThat(all_reference_wrapper<int>(f00).get(), Is().EqualTo(f00));
			});

			it("returns temporary", [&] {
				AssertThat(all_reference_wrapper<int>(0).get(), Is().EqualTo(0));
				AssertThat(&all_reference_wrapper<int>(0).get(), Is().Not().EqualTo(nullptr));
			});

			it("handles const", [&] {
				int f00            = 0;
				const int constf00 = 0;
				const all_reference_wrapper<int> constconstwrapper(constf00), constwrapper(f00);
				all_reference_wrapper<int> mutableconstwrapper(constf00), mutablewrapper(f00);
				AssertThat(&constconstwrapper.get(), Is().EqualTo(&constf00));
				AssertThat(&constwrapper.get(), Is().EqualTo(&f00));
				AssertThat(&mutableconstwrapper.get(), Is().EqualTo(&constf00));
				AssertThat(&mutablewrapper.get(), Is().EqualTo(&f00));
			});
		});

		describe("operator T&()", [&] {
			it("returns identity reference", [&] {
				int f00 = 0;
				AssertThat(&static_cast<int &>(all_reference_wrapper<int>(f00)), Is().EqualTo(&f00));
				AssertThat(static_cast<int &>(all_reference_wrapper<int>(f00)), Is().EqualTo(f00));
				AssertThat(&static_cast<const int &>(all_reference_wrapper<int>(f00)), Is().EqualTo(&f00));
				AssertThat(static_cast<const int &>(all_reference_wrapper<int>(f00)), Is().EqualTo(f00));
			});

			it("returns temporary", [&] {
				AssertThat(static_cast<int &>(all_reference_wrapper<int>(0)), Is().EqualTo(0));
				AssertThat(&static_cast<int &>(all_reference_wrapper<int>(0)), Is().Not().EqualTo(nullptr));
				AssertThat(static_cast<const int &>(all_reference_wrapper<int>(0)), Is().EqualTo(0));
				AssertThat(&static_cast<const int &>(all_reference_wrapper<int>(0)), Is().Not().EqualTo(nullptr));
			});
		});

		describe("assignment", [&] {
			it("copies", [&] {
				int f00 = 0;
				int m00 = 1;
				all_reference_wrapper<int> wrapper(f00);
				all_reference_wrapper<int> wrapper2(m00);
				wrapper = wrapper2;
				AssertThat(&wrapper.get(), Is().EqualTo(&m00))
			});

			it("moves", [&] {
				int f00 = 0;
				int m00 = 1;
				all_reference_wrapper<int> wrapper(f00);
				wrapper = all_reference_wrapper<int>(m00);
				AssertThat(&wrapper.get(), Is().EqualTo(&m00))
			});

			it("implicitly moves", [&] {
				int f00 = 0;
				AssertThat((all_reference_wrapper<int>(f00) = 1).get(), Is().EqualTo(1))
			});
		});

		it("has equivalent operator T&() and get()", [&] {
			int f00 = 0;
			all_reference_wrapper<int> wrapper(f00);
			const all_reference_wrapper<int> constwrapper(f00);
			AssertThat(&static_cast<int &>(wrapper), Is().EqualTo(&wrapper.get()));
			AssertThat(&static_cast<const int &>(constwrapper), Is().EqualTo(&constwrapper.get()));
		});
	});
});
