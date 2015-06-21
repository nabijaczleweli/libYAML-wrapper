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


#include <yaml_parser.hpp>
#include <map>
#include <vector>
#include <iostream>
#include <memory>
#include <sstream>


using namespace libyaml;
using namespace std;


static const string toindent("  ");


static string operator*(const string & str, unsigned int amt) {
	string temp;
	temp.reserve(str.size() * amt);
	while(amt--)
		temp += str;
	return temp;
}


int main()  {
	yaml_parser parser;
	parser.read_from_file("test.yaml");
	auto token = make_shared<yaml_token_t>();
	decltype(token->type) type;

	unsigned int indent = 0;

	do {
		yaml_parser_scan(&parser, token.get());

		switch(token->type) {
			case YAML_NO_TOKEN :
				cout << toindent * indent << "YAML_NO_TOKEN? Curious. Curious, indeed.";
				break;
			case YAML_STREAM_START_TOKEN :
				cout << toindent * indent << "YAML_STREAM_START_TOKEN w/ ";
				switch(token->data.stream_start.encoding) {
					case YAML_ANY_ENCODING :
						cout << "YAML_ANY_ENCODING (the parser chooses the encoding)";
						break;
					case YAML_UTF8_ENCODING :
						cout << "YAML_UTF8_ENCODING (default UTF-8 encoding)";
						break;
					case YAML_UTF16LE_ENCODING :
						cout << "YAML_UTF16LE_ENCODING (UTF-16-LE encoding with BOM)";
						break;
					case YAML_UTF16BE_ENCODING :
						cout << "YAML_UTF16BE_ENCODING (UTF-16-BE encoding with BOM)";
						break;
				}
				++indent;
				break;
			case YAML_STREAM_END_TOKEN :
				indent = 0;
				cout << toindent * indent << "YAML_STREAM_END_TOKEN";
				break;
			case YAML_VERSION_DIRECTIVE_TOKEN :
				cout << toindent * indent << "YAML_VERSION_DIRECTIVE_TOKEN @ " << token->data.version_directive.major << '.' << token->data.version_directive.minor;
				break;
			case YAML_TAG_DIRECTIVE_TOKEN :
				cout << toindent * indent << "YAML_TAG_DIRECTIVE_TOKEN prefixed by " << token->data.tag_directive.prefix << " handled by " <<
				                                                                        token->data.tag_directive.handle;
				break;
			case YAML_DOCUMENT_START_TOKEN :
				cout << toindent * indent++ << "YAML_DOCUMENT_START_TOKEN";
				break;
			case YAML_DOCUMENT_END_TOKEN :
				cout << toindent * --indent << "YAML_DOCUMENT_END_TOKEN";
				break;
			case YAML_BLOCK_SEQUENCE_START_TOKEN :
				cout << toindent * indent++ << "YAML_BLOCK_SEQUENCE_START_TOKEN";
				break;
			case YAML_BLOCK_MAPPING_START_TOKEN :
				cout << toindent * --indent << "YAML_BLOCK_MAPPING_START_TOKEN (BLOCK-SEQUENCE-END)";
				break;
			case YAML_BLOCK_END_TOKEN :
				cout << toindent * --indent << "YAML_BLOCK_END_TOKEN";
				break;
			case YAML_FLOW_SEQUENCE_START_TOKEN :
				cout << toindent * indent++ << "YAML_FLOW_SEQUENCE_START_TOKEN";
				break;
			case YAML_FLOW_SEQUENCE_END_TOKEN :
				cout << toindent * --indent << "YAML_FLOW_SEQUENCE_END_TOKEN";
				break;
			case YAML_FLOW_MAPPING_START_TOKEN :
				cout << toindent * indent++ << "YAML_FLOW_MAPPING_START_TOKEN";
				break;
			case YAML_FLOW_MAPPING_END_TOKEN :
				cout << toindent * --indent << "YAML_FLOW_MAPPING_END_TOKEN";
				break;
			case YAML_BLOCK_ENTRY_TOKEN :
				cout << toindent * indent++ << "YAML_BLOCK_ENTRY_TOKEN";
				break;
			case YAML_FLOW_ENTRY_TOKEN :
				cout << toindent * indent++ << "YAML_FLOW_ENTRY_TOKEN";
				break;
			case YAML_KEY_TOKEN :
				cout << toindent * indent << "YAML_KEY_TOKEN";
				break;
			case YAML_VALUE_TOKEN :
				cout << toindent * indent << "YAML_VALUE_TOKEN";
				break;
			case YAML_ALIAS_TOKEN :
				cout << toindent * indent << "YAML_ALIAS_TOKEN -> " << token->data.alias.value;
				break;
			case YAML_ANCHOR_TOKEN :
				cout << toindent * indent << "YAML_ANCHOR_TOKEN -> " << token->data.anchor.value;
				break;
			case YAML_TAG_TOKEN :
				cout << toindent * indent << "YAML_TAG_TOKEN #" << token->data.tag.handle << " + " << token->data.tag.suffix;
				break;
			case YAML_SCALAR_TOKEN :
				cout << toindent * indent << "YAML_SCALAR_TOKEN = \"" << token->data.scalar.value << "\".size() == " << token->data.scalar.length << " as ";
				switch(token->data.scalar.style) {
					case YAML_ANY_SCALAR_STYLE :
						cout << "YAML_ANY_SCALAR_STYLE (the emitter chooses the style)";
						break;
					case YAML_PLAIN_SCALAR_STYLE :
						cout << "YAML_PLAIN_SCALAR_STYLE (plain scalar style)";
						break;
					case YAML_SINGLE_QUOTED_SCALAR_STYLE :
						cout << "YAML_SINGLE_QUOTED_SCALAR_STYLE (single-quoted scalar style)";
						break;
					case YAML_DOUBLE_QUOTED_SCALAR_STYLE :
						cout << "YAML_DOUBLE_QUOTED_SCALAR_STYLE (double-quoted scalar style)";
						break;
					case YAML_LITERAL_SCALAR_STYLE :
						cout << "YAML_LITERAL_SCALAR_STYLE (literal scalar style)";
						break;
					case YAML_FOLDED_SCALAR_STYLE :
						cout << "YAML_FOLDED_SCALAR_STYLE (folded scalar style)";
						break;
				}
				break;
		}
		cout << '\n';

		type = token->type;
		yaml_token_delete(token.get());
	} while(type != YAML_STREAM_END_TOKEN && type != YAML_NO_TOKEN);
}
