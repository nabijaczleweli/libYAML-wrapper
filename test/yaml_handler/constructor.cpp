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
#include <utility>


using namespace std;
using namespace bandit;
using namespace libyaml;


#define ASSERT_TOKEN_HANDLER_NULL_(hndlr, token) AssertThat(bool((hndlr).do_on_##token), Is().EqualTo(false))
#define ASSERT_TOKEN_HANDLER_NONNULL_(hndlr, token) AssertThat(bool((hndlr).do_on_##token), Is().EqualTo(true))

#define ASSERT_ALL_TOKEN_HANDLERS(hndlr, what)                        \
	do {                                                                \
		ASSERT_TOKEN_HANDLER_##what((hndlr), no_token);                   \
		ASSERT_TOKEN_HANDLER_##what((hndlr), stream_start_token);         \
		ASSERT_TOKEN_HANDLER_##what((hndlr), stream_end_token);           \
		ASSERT_TOKEN_HANDLER_##what((hndlr), version_directive_token);    \
		ASSERT_TOKEN_HANDLER_##what((hndlr), tag_directive_token);        \
		ASSERT_TOKEN_HANDLER_##what((hndlr), document_start_token);       \
		ASSERT_TOKEN_HANDLER_##what((hndlr), document_end_token);         \
		ASSERT_TOKEN_HANDLER_##what((hndlr), block_sequence_start_token); \
		ASSERT_TOKEN_HANDLER_##what((hndlr), block_mapping_start_token);  \
		ASSERT_TOKEN_HANDLER_##what((hndlr), block_end_token);            \
		ASSERT_TOKEN_HANDLER_##what((hndlr), flow_sequence_start_token);  \
		ASSERT_TOKEN_HANDLER_##what((hndlr), flow_sequence_end_token);    \
		ASSERT_TOKEN_HANDLER_##what((hndlr), flow_mapping_start_token);   \
		ASSERT_TOKEN_HANDLER_##what((hndlr), flow_mapping_end_token);     \
		ASSERT_TOKEN_HANDLER_##what((hndlr), block_entry_token);          \
		ASSERT_TOKEN_HANDLER_##what((hndlr), flow_entry_token);           \
		ASSERT_TOKEN_HANDLER_##what((hndlr), key_token);                  \
		ASSERT_TOKEN_HANDLER_##what((hndlr), value_token);                \
		ASSERT_TOKEN_HANDLER_##what((hndlr), alias_token);                \
		ASSERT_TOKEN_HANDLER_##what((hndlr), anchor_token);               \
		ASSERT_TOKEN_HANDLER_##what((hndlr), tag_token);                  \
		ASSERT_TOKEN_HANDLER_##what((hndlr), scalar_token);               \
	} while(false)

#define MAKE_FULL_HANDLER(name)                                \
	yaml_handler(name);                                          \
	(name).do_on_no_token = [&]() {};                            \
	(name).do_on_stream_start_token = [&](const auto &) {};      \
	(name).do_on_stream_end_token = [&]() {};                    \
	(name).do_on_version_directive_token = [&](const auto &) {}; \
	(name).do_on_tag_directive_token = [&](const auto &) {};     \
	(name).do_on_document_start_token = [&]() {};                \
	(name).do_on_document_end_token = [&]() {};                  \
	(name).do_on_block_sequence_start_token = [&]() {};          \
	(name).do_on_block_mapping_start_token = [&]() {};           \
	(name).do_on_block_end_token = [&]() {};                     \
	(name).do_on_flow_sequence_start_token = [&]() {};           \
	(name).do_on_flow_sequence_end_token = [&]() {};             \
	(name).do_on_flow_mapping_start_token = [&]() {};            \
	(name).do_on_flow_mapping_end_token = [&]() {};              \
	(name).do_on_block_entry_token = [&]() {};                   \
	(name).do_on_flow_entry_token = [&]() {};                    \
	(name).do_on_key_token = [&]() {};                           \
	(name).do_on_value_token = [&]() {};                         \
	(name).do_on_alias_token = [&](const auto &) {};             \
	(name).do_on_anchor_token = [&](const auto &) {};            \
	(name).do_on_tag_token = [&](const auto &) {};               \
	(name).do_on_scalar_token = [&](const auto &) {}


go_bandit([&] {
	describe("handler", [&] {
		describe("constructors", [&] {
			it("default-constructs emptily", [&] {
				yaml_handler handler;
				ASSERT_ALL_TOKEN_HANDLERS(handler, NULL_);
			});

			it("copies everything", [&] {
				MAKE_FULL_HANDLER(handler0);
				yaml_handler handler1(handler0);
				ASSERT_ALL_TOKEN_HANDLERS(handler1, NONNULL_);
			});

			it("moves everything", [&] {
				MAKE_FULL_HANDLER(handler0);
				yaml_handler handler1(move(handler0));
				ASSERT_ALL_TOKEN_HANDLERS(handler1, NONNULL_);
			});
		});

		it("clones", [&] {
			MAKE_FULL_HANDLER(handler);
			const auto clone = handler.clone();
			ASSERT_ALL_TOKEN_HANDLERS(*clone, NONNULL_);
		});
	});
});
