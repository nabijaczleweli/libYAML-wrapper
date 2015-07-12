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
#define private public  // Hack into yaml_reader::handlers
#include <yaml_reader.hpp>
#undef private
#include <memory>


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

go_bandit([&] {
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

			// TODO: add test for all tokens in sequence
		});
	});
});
