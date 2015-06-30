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
#include "util/variadic.hpp"
#include <experimental/optional>
#include <memory>
#include <string>
#include <vector>
#include <yaml.h>


namespace libyaml {
	class yaml_reader {
	private:
		yaml_parser parser;

	public:
		static std::experimental::optional<unsigned int> consecutive_notoken_threshold;

		std::vector<std::shared_ptr<yaml_handler>> handlers;

		inline yaml_reader();
		inline yaml_reader(const yaml_reader &);
		inline yaml_reader(yaml_reader &&);
		template <class... Args>
		inline yaml_reader(Args &&... args);
		template <class S, class... Args>
		inline yaml_reader(const S & str, Args &&... args);

		void read(const std::string & from);
	};
}


inline libyaml::yaml_reader::yaml_reader() = default;
inline libyaml::yaml_reader::yaml_reader(const yaml_reader &) = default;
inline libyaml::yaml_reader::yaml_reader(yaml_reader &&) = default;

template <class... Args>
inline libyaml::yaml_reader::yaml_reader(Args &&... args) {
	handlers.reserve(sizeof...(Args));
	libyaml::util::over_all<Args...>::for_each([&](auto && val) { handlers.emplace_back(val.clone()); }, std::forward<Args>(args)...);
}

template <class S, class... Args>
inline libyaml::yaml_reader::yaml_reader(const S & str, Args &&... args) : yaml_reader(std::forward<Args>(args)...) {
	read(str);
}


#endif  // YAML_READER_HPP
