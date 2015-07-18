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
#include "../testutil/throw.hpp"
#include <yaml_reader.hpp>
#include <memory>
#include <iterator>
#include <algorithm>


using namespace std;
using namespace bandit;
using namespace libyaml;


class yaml_sequence_counting_handler : public yaml_handler {
private:
	int & stream_counter;
	int & version_counter;

public:
	inline yaml_sequence_counting_handler(int & strm, int & ver) : stream_counter(strm), version_counter(ver) {}
	inline yaml_sequence_counting_handler(yaml_sequence_counting_handler && ysch) = default;
	inline yaml_sequence_counting_handler(const yaml_sequence_counting_handler & ysch) = default;
	virtual ~yaml_sequence_counting_handler() noexcept = default;

	inline yaml_sequence_counting_handler & operator=(yaml_sequence_counting_handler &&) = default;
	inline yaml_sequence_counting_handler & operator=(const yaml_sequence_counting_handler &) = default;

	virtual unique_ptr<yaml_handler> clone() const override {
		return make_unique<yaml_sequence_counting_handler>(*this);
	}

	virtual void on_stream_start_token(const stream_start_t &) override {
		++stream_counter;
	}

	virtual void on_version_directive_token(const version_directive_t &) override {
		++version_counter;
	}
};

go_bandit([] {
	describe("reader", [&] {
		describe("read()", [&] {
			it("doesn't throw", [&] {
				AssertNothrow(yaml_reader().read());
				AssertNothrow(yaml_reader().read(""));
			});

			it("spreads tokens across all handlers", [&] {
				int streams{}, versions{};
				yaml_sequence_counting_handler handler(streams, versions);
				vector<yaml_sequence_counting_handler> handlers;
				handlers.reserve(10);
				fill_n(back_inserter(handlers), 10, handler);

				yaml_reader(handlers).read("%YAML 2.2");
				AssertThat(handlers, Is().OfLength(streams));
				AssertThat(handlers, Is().OfLength(versions));
			});

			it("handles exceptions", [&] {
				yaml_handler handler;
				handler.do_on_stream_start_token = [&](const auto &) { throw nullptr; };
				AssertThrows(nullptr_t, yaml_reader({handler}).read(""));
				AssertNothrow(yaml_reader({handler}).read());
			});

			it("generates all tokens in sequence", [&] {
				vector<yaml_token_type_t> sequence;
				yaml_handler handler;
				handler.do_on_no_token                   = [&]() { sequence.emplace_back(YAML_NO_TOKEN); };
				handler.do_on_stream_start_token         = [&](const auto &) { sequence.emplace_back(YAML_STREAM_START_TOKEN); };
				handler.do_on_stream_end_token           = [&]() { sequence.emplace_back(YAML_STREAM_END_TOKEN); };
				handler.do_on_version_directive_token    = [&](const auto &) { sequence.emplace_back(YAML_VERSION_DIRECTIVE_TOKEN); };
				handler.do_on_tag_directive_token        = [&](const auto &) { sequence.emplace_back(YAML_TAG_DIRECTIVE_TOKEN); };
				handler.do_on_document_start_token       = [&]() { sequence.emplace_back(YAML_DOCUMENT_START_TOKEN); };
				handler.do_on_document_end_token         = [&]() { sequence.emplace_back(YAML_DOCUMENT_END_TOKEN); };
				handler.do_on_block_sequence_start_token = [&]() { sequence.emplace_back(YAML_BLOCK_SEQUENCE_START_TOKEN); };
				handler.do_on_block_mapping_start_token  = [&]() { sequence.emplace_back(YAML_BLOCK_MAPPING_START_TOKEN); };
				handler.do_on_block_end_token            = [&]() { sequence.emplace_back(YAML_BLOCK_END_TOKEN); };
				handler.do_on_flow_sequence_start_token  = [&]() { sequence.emplace_back(YAML_FLOW_SEQUENCE_START_TOKEN); };
				handler.do_on_flow_sequence_end_token    = [&]() { sequence.emplace_back(YAML_FLOW_SEQUENCE_END_TOKEN); };
				handler.do_on_flow_mapping_start_token   = [&]() { sequence.emplace_back(YAML_FLOW_MAPPING_START_TOKEN); };
				handler.do_on_flow_mapping_end_token     = [&]() { sequence.emplace_back(YAML_FLOW_MAPPING_END_TOKEN); };
				handler.do_on_block_entry_token          = [&]() { sequence.emplace_back(YAML_BLOCK_ENTRY_TOKEN); };
				handler.do_on_flow_entry_token           = [&]() { sequence.emplace_back(YAML_FLOW_ENTRY_TOKEN); };
				handler.do_on_key_token                  = [&]() { sequence.emplace_back(YAML_KEY_TOKEN); };
				handler.do_on_value_token                = [&]() { sequence.emplace_back(YAML_VALUE_TOKEN); };
				handler.do_on_alias_token                = [&](const auto &) { sequence.emplace_back(YAML_ALIAS_TOKEN); };
				handler.do_on_anchor_token               = [&](const auto &) { sequence.emplace_back(YAML_ANCHOR_TOKEN); };
				handler.do_on_tag_token                  = [&](const auto &) { sequence.emplace_back(YAML_TAG_TOKEN); };
				handler.do_on_scalar_token               = [&](const auto &) { sequence.emplace_back(YAML_SCALAR_TOKEN); };

				yaml_reader({handler})
				    .read("%YAML 2.2\n"                       // stream start, version directive
				          "%TAG !yaml! tag:yaml.org,2002:\n"  // tag directive
				          "\n"
				          "---\n"      // document start
				          "m00:\n"     // block mapping start, key, scalar, value, block sequence start
				          "  - f00\n"  // block entry, scalar
				          "  - b00\n"  // block entry, scalar, block end, block end
				          "...\n"      // document end
				          "\n"
				          "[]\n"  // flow sequence start, flow sequence end
				          "\n"
				          "{f00: que, que: fooo}\n"  // flow mapping start, key, scalar, value scalar, flow entry, key, scalar, value, scalar, flow mapping end
				          "\n"
				          "f00: &blazeit 420\n"  // block mapping start, key, scalar, value, anchor, scalar
				          "b00: *blazeit\n");    // key, scalar, value, alias, block end, stream end

				AssertThat(sequence, Is().EqualToContainer(decltype(sequence)(
				                         {YAML_STREAM_START_TOKEN, YAML_VERSION_DIRECTIVE_TOKEN, YAML_TAG_DIRECTIVE_TOKEN, YAML_DOCUMENT_START_TOKEN,

				                          YAML_BLOCK_MAPPING_START_TOKEN, YAML_KEY_TOKEN, YAML_SCALAR_TOKEN,

				                          YAML_VALUE_TOKEN, YAML_BLOCK_SEQUENCE_START_TOKEN, YAML_BLOCK_ENTRY_TOKEN, YAML_SCALAR_TOKEN, YAML_BLOCK_ENTRY_TOKEN,
				                          YAML_SCALAR_TOKEN, YAML_BLOCK_END_TOKEN, YAML_BLOCK_END_TOKEN,

				                          YAML_DOCUMENT_END_TOKEN,

				                          YAML_FLOW_SEQUENCE_START_TOKEN, YAML_FLOW_SEQUENCE_END_TOKEN,

				                          YAML_FLOW_MAPPING_START_TOKEN, YAML_KEY_TOKEN, YAML_SCALAR_TOKEN, YAML_VALUE_TOKEN, YAML_SCALAR_TOKEN, YAML_FLOW_ENTRY_TOKEN,
				                          YAML_KEY_TOKEN, YAML_SCALAR_TOKEN, YAML_VALUE_TOKEN, YAML_SCALAR_TOKEN, YAML_FLOW_MAPPING_END_TOKEN,

				                          YAML_BLOCK_MAPPING_START_TOKEN, YAML_KEY_TOKEN, YAML_SCALAR_TOKEN, YAML_VALUE_TOKEN, YAML_ANCHOR_TOKEN, YAML_SCALAR_TOKEN,

				                          YAML_KEY_TOKEN, YAML_SCALAR_TOKEN, YAML_VALUE_TOKEN, YAML_ALIAS_TOKEN, YAML_BLOCK_END_TOKEN, YAML_STREAM_END_TOKEN})));
			});
		});
	});
});
