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
#ifndef YAML_READER_HPP
#define YAML_READER_HPP


#define YAML_DECLARE_STATIC

#include "yaml_reader.hpp"
#include "yaml_parser.hpp"
#include "yaml_handler.hpp"
#include "util/reference_wrapper.hpp"
#include <experimental/optional>
#include <memory>
#include <string>
#include <vector>
#include <yaml.h>


namespace libyaml {
	class yaml_reader {
	private:
		yaml_parser parser;
		std::vector<std::shared_ptr<yaml_handler>> handlers;

	public:
		static std::experimental::optional<unsigned int> consecutive_notoken_threshold;

		inline yaml_reader();
		inline yaml_reader(const yaml_reader &);
		inline yaml_reader(yaml_reader &&);
		inline yaml_reader(std::initializer_list<libyaml::util::all_reference_wrapper<yaml_handler>> lst);
		template <class T>
		inline yaml_reader(const T & cont);

		inline ~yaml_reader();

		void open(const std::string & from);

		void read();
		void read(const std::string & from);

		void append_handler(libyaml::util::all_reference_wrapper<yaml_handler> ref);
	};
}


inline libyaml::yaml_reader::yaml_reader() = default;
inline libyaml::yaml_reader::yaml_reader(const yaml_reader &) = default;
inline libyaml::yaml_reader::yaml_reader(yaml_reader &&) = default;

inline libyaml::yaml_reader::~yaml_reader() = default;

inline libyaml::yaml_reader::yaml_reader(std::initializer_list<libyaml::util::all_reference_wrapper<yaml_handler>> lst) {
	handlers.reserve(lst.size());
	for(const auto & handler : lst)
		handlers.emplace_back(handler.get().clone());
}

template <class T>
inline libyaml::yaml_reader::yaml_reader(const T & cont) {
	handlers.reserve(cont.size());
	for(const auto & handler : cont)
		handlers.emplace_back(handler.clone());
}


#endif  // YAML_READER_HPP
