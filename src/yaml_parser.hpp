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


#pragma once
#ifndef YAML_PARSER_HPP
#define YAML_PARSER_HPP


#define YAML_DECLARE_STATIC

#include <yaml.h>
#include <string>
#include <cstdlib>


namespace libyaml {
	/** A wrapper around yaml_parser_t, for OO-ness */
	class yaml_parser : public yaml_parser_t {
		private:
			std::basic_string<unsigned char> input_buffer;
			std::FILE * input_file;

			void try_close_file();

		public:
			yaml_parser() noexcept;
			~yaml_parser() noexcept;

			void read_from_file(const std::string & path);
			void read_from_data(const std::string & data);
	};
}


#endif // YAML_PARSER_HPP
