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
#define private public  // Hack into yaml_reader::handlers
#include <yaml_reader.hpp>
#undef private
#include <memory>


using namespace std;
using namespace libyaml;


class yaml_sequence_counting_handler : public yaml_handler {
private:
	shared_ptr<int> stream_counter;
	shared_ptr<int> version_counter;

public:
	inline yaml_sequence_counting_handler() : stream_counter(make_shared<int>(0)), version_counter(make_shared<int>(0)) {}
	inline yaml_sequence_counting_handler(yaml_sequence_counting_handler &&) = default;
	inline yaml_sequence_counting_handler(const yaml_sequence_counting_handler &) = default;

	virtual ~yaml_sequence_counting_handler() noexcept = default;

	inline yaml_sequence_counting_handler & operator=(yaml_sequence_counting_handler &&) = default;
	inline yaml_sequence_counting_handler & operator=(const yaml_sequence_counting_handler &) = default;

	virtual unique_ptr<yaml_handler> clone() const override {
		return make_unique<yaml_sequence_counting_handler>(*this);
	}

	virtual void on_stream_start_token(const stream_start_t &) override {
		++*stream_counter;
	}

	virtual void on_version_directive_token(const version_directive_t &) override {
		++*version_counter;
	}


	int stream_count() const {
		return *stream_counter;
	}

	int version_count() const {
		return *version_counter;
	}
};

TEST_CASE("Basic read() tests", "[reader]") {
	REQUIRE_NOTHROW(yaml_reader().read());
	REQUIRE_NOTHROW(yaml_reader().read(""));

	{
		yaml_sequence_counting_handler handler;
		vector<yaml_sequence_counting_handler> handlers;
		handlers.reserve(10);
		fill_n(back_inserter(handlers), 10, handler);
		yaml_reader(handlers).read("%YAML 2.2");
		REQUIRE(handler.stream_count() == handlers.size());
		REQUIRE(handler.version_count() == handlers.size());
	}

	{
		yaml_handler handler;
		handler.do_on_stream_start_token = [&](const auto &) {
			throw nullptr;
		};
		REQUIRE_THROWS_AS(yaml_reader({handler}).read(""), nullptr_t);
		REQUIRE_NOTHROW(yaml_reader({handler}).read());
	}
}
