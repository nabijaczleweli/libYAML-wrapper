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


#include "yaml_reader.hpp"
#include "util/file.hpp"
#include <algorithm>
#include <exception>


using namespace std;
using namespace std::experimental;
using namespace libyaml;
using namespace libyaml::util;


optional<unsigned int> yaml_reader::consecutive_notoken_threshold = 10;


void yaml_reader::open(const string & from) {
	if(exists(from))
		parser.read_from_file(from);
	else
		parser.read_from_data(from);
}

void yaml_reader::read() {
	if(!parser.has_input())
		return;

	auto notokens_tolerated_left = consecutive_notoken_threshold ? consecutive_notoken_threshold.value() : 0u;
	exception_ptr thrown         = nullptr;
	yaml_token_t token;
	do {
		yaml_parser_scan(parser, &token);
		auto type = token.type;

		for(const auto & handler : all_handlers)
			try {
				type = handler->handle(token, false);
			} catch(...) {
				thrown = current_exception();
			}

		if(consecutive_notoken_threshold) {
			if(type == YAML_NO_TOKEN)
				--notokens_tolerated_left;
			else
				notokens_tolerated_left = consecutive_notoken_threshold.value();
		}
	} while(yaml_handler::delete_token(token) != YAML_STREAM_END_TOKEN && !thrown && (!consecutive_notoken_threshold || notokens_tolerated_left));

	if(thrown)
		rethrow_exception(thrown);
}

void yaml_reader::read(const string & from) {
	open(from);
	read();
}

bool yaml_reader::has_input() const {
	return parser.has_input();
}

auto yaml_reader::handlers() -> decltype(all_handlers.size()) const {
	return all_handlers.size();
}

void yaml_reader::append_handler(all_reference_wrapper<yaml_handler> ref) {
	all_handlers.emplace_back(ref.get().clone());
}
