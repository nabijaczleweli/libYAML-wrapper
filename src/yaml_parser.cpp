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


#include "yaml_parser.hpp"
#include "detail/file.hpp"
#include <ios>


using namespace std;
using namespace libyaml;


yaml_parser::yaml_parser() noexcept {
	yaml_parser_initialize(this);
}

yaml_parser::~yaml_parser() noexcept {
	fill(input_buffer.begin(), input_buffer.end(), 0);
	yaml_parser_delete(this);
}

void yaml_parser::read_from_file(const string & path) {
	static detail::file_deleter deleter;

	input_file.reset(fopen(path.c_str(), "r"), deleter);
	if(!input_file) {
		input_file.reset(static_cast<FILE *>(nullptr), deleter);
		throw ios_base::failure("Cannot open \"" + path + "\" for reading");
	}

	yaml_parser_set_input_file(this, input_file.get());
}

void yaml_parser::read_from_data(const string & data) {
	input_buffer.assign(data.begin(), data.end());
	yaml_parser_set_input_string(this, input_buffer.c_str(), input_buffer.size());
}
