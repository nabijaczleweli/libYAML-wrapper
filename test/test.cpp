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


#define private public  // Hack into yaml_reader::parser
#include <yaml_reader.hpp>
#undef private
#include <experimental/string_view>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>


using namespace std;
using namespace std::experimental;
using namespace libyaml;


static string operator*(const string & str, unsigned int amt) {
	string temp;
	temp.reserve(str.size() * amt);
	while(amt--)
		temp += str;
	return temp;
}


class priniting_handler : public yaml_handler {
public:
	using yaml_handler::yaml_handler;

	priniting_handler() {
		static const constexpr auto defprint = [](auto str) { cout << str << '\n'; };

		do_on_no_token                   = bind(defprint, "No token");
		do_on_stream_end_token           = bind(defprint, "Stream end");
		do_on_document_start_token       = bind(defprint, "Document start");
		do_on_document_end_token         = bind(defprint, "Document end");
		do_on_block_sequence_start_token = bind(defprint, "Block sequence start");
		do_on_block_mapping_start_token  = bind(defprint, "Block mapping start");
		do_on_block_end_token            = bind(defprint, "Block end");
		do_on_flow_sequence_start_token  = bind(defprint, "Flow sequence start");
		do_on_flow_sequence_end_token    = bind(defprint, "Flow sequence end");
		do_on_flow_mapping_start_token   = bind(defprint, "Flow mapping start");
		do_on_flow_mapping_end_token     = bind(defprint, "Flow mapping end");
		do_on_block_entry_token          = bind(defprint, "Block entry");
		do_on_flow_entry_token           = bind(defprint, "Block entry");
		do_on_key_token                  = bind(defprint, "Key");
		do_on_value_token                = bind(defprint, "Value");
	}

	virtual unique_ptr<yaml_handler> clone() const {
		return make_unique<priniting_handler>(*this);
	}

	virtual void on_stream_start_token(const stream_start_t & stream_start) override {
		static const map<yaml_encoding_t, string_view> encodings({{YAML_ANY_ENCODING, "whatever"},
		                                                          {YAML_UTF8_ENCODING, "UTF-8"},
		                                                          {YAML_UTF16LE_ENCODING, "UTF-16-LE with BOM"},
		                                                          {YAML_UTF16BE_ENCODING, "UTF-16-BE with BOM"}});

		cout << "Start stream encoded as " << encodings.at(stream_start.encoding) << '\n';
	}

	virtual void on_version_directive_token(const version_directive_t & version_directive) override {
		cout << "Version: " << version_directive.major << '.' << version_directive.minor << '\n';
	}

	virtual void on_tag_directive_token(const tag_directive_t & tag_directive) override {
		cout << tag_directive.prefix << " tag => " << tag_directive.handle << '\n';
	}

	virtual void on_alias_token(const alias_t & alias) override {
		cout << "Aliased to " << alias.value << '\n';
	}

	virtual void on_anchor_token(const anchor_t & anchor) override {
		cout << "Anchor " << anchor.value << '\n';
	}

	virtual void on_tag_token(const tag_t & tag) override {
		cout << "Tag " << tag.suffix << " => " << tag.handle << '\n';
	}

	virtual void on_scalar_token(const scalar_t & scalar) override {
		static const map<yaml_scalar_style_t, string_view> styles({{YAML_ANY_SCALAR_STYLE, "however"},
		                                                           {YAML_PLAIN_SCALAR_STYLE, "plainly"},
		                                                           {YAML_SINGLE_QUOTED_SCALAR_STYLE, "single-quotedly"},
		                                                           {YAML_DOUBLE_QUOTED_SCALAR_STYLE, "double-quotedly"},
		                                                           {YAML_LITERAL_SCALAR_STYLE, "literally"},
		                                                           {YAML_FOLDED_SCALAR_STYLE, "via folding"}});

		cout << "Scalar \"" << scalar.value << "\" & size() == " << scalar.length << " styled " << styles.at(scalar.style) << '\n';
	}
};


int main() {
	string curkey;
	bool await_key = false;
	map<string, vector<string>> themap;

	yaml_handler handler;
	handler.do_on_scalar_token = [&](const auto & scalar) {
		string value(reinterpret_cast<char *>(scalar.value), scalar.length);
		if(await_key) {
			curkey = move(value);
			themap.insert({curkey, {}});
			await_key = false;
		} else
			themap[curkey].emplace_back(move(value));
	};
	handler.do_on_key_token = [&]() { await_key = true; };
	yaml_reader("../test/test.yaml", handler, priniting_handler());

	cout << '\n';
	for(const auto & kv : themap) {
		const string spacer = " "s * (kv.first.size() + 1);
		cout << kv.first << ":\n";
		for(const auto & v : kv.second)
			cout << spacer << v << '\n';
	}
}
