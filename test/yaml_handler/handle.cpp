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
#include <yaml_handler.hpp>


using namespace bandit;
using namespace libyaml;


#define IT_FORWARDS_TOKEN_BASE(token_upper, token_lower, token_name, arg)                     \
	it("forwards " token_name, [&] {                                                            \
		yaml_handler hndlr;                                                                       \
		hndlr.do_on_##token_lower##_token = [&](arg) { throw YAML_##token_upper##_TOKEN; };       \
		yaml_token_t token{};                                                                     \
		token.type = YAML_##token_upper##_TOKEN;                                                  \
		AssertThrows(yaml_token_type_t, hndlr.handle(token, false));                              \
		AssertThat(LastException<yaml_token_type_t>(), Is().EqualTo(YAML_##token_upper##_TOKEN)); \
	})

#define IT_FORWARDS_TOKEN(token_upper, token_lower, token_name) IT_FORWARDS_TOKEN_BASE(token_upper, token_lower, token_name, )
#define IT_FORWARDS_TOKEN_ARG(token_upper, token_lower, token_name) IT_FORWARDS_TOKEN_BASE(token_upper, token_lower, token_name, const auto &)

go_bandit([] {
	describe("handler", [&] {
		describe("token handling", [&] {
			IT_FORWARDS_TOKEN(NO, no, "lack of token");
			IT_FORWARDS_TOKEN_ARG(STREAM_START, stream_start, "stream start");
			IT_FORWARDS_TOKEN(STREAM_END, stream_end, "stream end");
			IT_FORWARDS_TOKEN_ARG(VERSION_DIRECTIVE, version_directive, "version directive");
			IT_FORWARDS_TOKEN_ARG(TAG_DIRECTIVE, tag_directive, "tag directive");
			IT_FORWARDS_TOKEN(DOCUMENT_START, document_start, "document start");
			IT_FORWARDS_TOKEN(DOCUMENT_END, document_end, "document end");
			IT_FORWARDS_TOKEN(BLOCK_SEQUENCE_START, block_sequence_start, "block sequence start");
			IT_FORWARDS_TOKEN(BLOCK_MAPPING_START, block_mapping_start, "block mapping start");
			IT_FORWARDS_TOKEN(BLOCK_END, block_end, "block end");
			IT_FORWARDS_TOKEN(FLOW_SEQUENCE_START, flow_sequence_start, "flow sequence start");
			IT_FORWARDS_TOKEN(FLOW_SEQUENCE_END, flow_sequence_end, "flow sequence end");
			IT_FORWARDS_TOKEN(FLOW_MAPPING_START, flow_mapping_start, "flow mapping start");
			IT_FORWARDS_TOKEN(FLOW_MAPPING_END, flow_mapping_end, "flow mapping end");
			IT_FORWARDS_TOKEN(BLOCK_ENTRY, block_entry, "block entry");
			IT_FORWARDS_TOKEN(FLOW_ENTRY, flow_entry, "flow entry");
			IT_FORWARDS_TOKEN(KEY, key, "key");
			IT_FORWARDS_TOKEN(VALUE, value, "value");
			IT_FORWARDS_TOKEN_ARG(ALIAS, alias, "alias");
			IT_FORWARDS_TOKEN_ARG(ANCHOR, anchor, "anchor");
			IT_FORWARDS_TOKEN_ARG(TAG, tag, "tag");
			IT_FORWARDS_TOKEN_ARG(SCALAR, scalar, "scalar");
		});
	});
});
