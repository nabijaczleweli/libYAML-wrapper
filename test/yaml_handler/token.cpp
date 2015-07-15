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
#define private public
#include <yaml_handler.hpp>
#undef private
#include <algorithm>


using namespace std;
using namespace bandit;
using namespace libyaml;


#define MAKE_PREFILLED_TOKEN_BASE(name, fillwith, token_value) \
	yaml_token_t name;                                           \
	memset(&name, 'A', sizeof(name));                            \
	name.type = token_value

#define MAKE_PREFILLED_TOKEN(token_value) MAKE_PREFILLED_TOKEN_BASE(token, 'A', token_value)


bool operator==(const yaml_token_t & lhs, const yaml_token_t & rhs) {
	return equal(static_cast<const char *>(static_cast<const void *>(&lhs)), static_cast<const char *>(static_cast<const void *>(&lhs)) + sizeof(lhs),
	             static_cast<const char *>(static_cast<const void *>(&rhs)), static_cast<const char *>(static_cast<const void *>(&rhs)) + sizeof(rhs));
}


go_bandit([&] {
	describe("handler", [&] {
		const yaml_token_t zeroed_token{};

		it("deletes tokens", [&] {
			MAKE_PREFILLED_TOKEN(YAML_TAG_DIRECTIVE_TOKEN);  // doesn't need deletion, so no UB
			AssertThat(yaml_handler::delete_token(token), Is().EqualTo(YAML_TAG_DIRECTIVE_TOKEN));
			AssertThat(token, Is().EqualTo(zeroed_token));
		});

		it("deletes tokens after forwarding", [&] {
			MAKE_PREFILLED_TOKEN(YAML_STREAM_END_TOKEN);  // no associated data
			AssertThat(yaml_handler().handle(token), Is().EqualTo(YAML_STREAM_END_TOKEN));
			AssertThat(token, Is().EqualTo(zeroed_token));
		});
	});
});
