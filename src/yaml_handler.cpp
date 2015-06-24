// The MIT License(MIT)

// Copyright(c) 2015 nabijaczleweli

//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files(the "Software"),
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


#include "yaml_handler.hpp"


using namespace std;
using namespace libyaml;

using type_t              = yaml_handler::type_t;
using stream_start_t      = yaml_handler::stream_start_t;
using version_directive_t = yaml_handler::version_directive_t;
using tag_directive_t     = yaml_handler::tag_directive_t;
using alias_t             = yaml_handler::alias_t;
using anchor_t            = yaml_handler::anchor_t;
using tag_t               = yaml_handler::tag_t;
using scalar_t            = yaml_handler::scalar_t;


yaml_handler::~yaml_handler() = default;

unique_ptr<yaml_handler> yaml_handler::clone() const {
	return make_unique<yaml_handler>(*this);
}

type_t yaml_handler::delete_token(yaml_token_t & token) {
	const auto type = token.type;
	yaml_token_delete(&token);
	return type;
}

type_t yaml_handler::handle(yaml_token_t & token, bool handle_deletion) {
	#define DEFAULT_CASE_ON_TOKEN(token_upper, token_lower) \
		case YAML_##token_upper##_TOKEN : \
			on_##token_lower##_token(); \
			break;

	#define ARG_CASE_ON_TOKEN(token_upper, token_lower) \
		case YAML_##token_upper##_TOKEN : \
			on_##token_lower##_token(token.data.token_lower); \
			break;


	switch(token.type) {
		DEFAULT_CASE_ON_TOKEN(NO, no)
		DEFAULT_CASE_ON_TOKEN(STREAM_END, stream_end)
		DEFAULT_CASE_ON_TOKEN(DOCUMENT_START, document_start)
		DEFAULT_CASE_ON_TOKEN(DOCUMENT_END, document_end)
		DEFAULT_CASE_ON_TOKEN(BLOCK_SEQUENCE_START, block_sequence_start)
		DEFAULT_CASE_ON_TOKEN(BLOCK_MAPPING_START, block_mapping_start)
		DEFAULT_CASE_ON_TOKEN(BLOCK_END, block_end)
		DEFAULT_CASE_ON_TOKEN(FLOW_SEQUENCE_START, flow_sequence_start)
		DEFAULT_CASE_ON_TOKEN(FLOW_SEQUENCE_END, flow_sequence_end)
		DEFAULT_CASE_ON_TOKEN(FLOW_MAPPING_START, flow_mapping_start)
		DEFAULT_CASE_ON_TOKEN(FLOW_MAPPING_END, flow_mapping_end)
		DEFAULT_CASE_ON_TOKEN(BLOCK_ENTRY, block_entry)
		DEFAULT_CASE_ON_TOKEN(FLOW_ENTRY, flow_entry)
		DEFAULT_CASE_ON_TOKEN(KEY, key)
		DEFAULT_CASE_ON_TOKEN(VALUE, value)
		ARG_CASE_ON_TOKEN(STREAM_START, stream_start)
		ARG_CASE_ON_TOKEN(VERSION_DIRECTIVE, version_directive)
		ARG_CASE_ON_TOKEN(TAG_DIRECTIVE, tag_directive)
		ARG_CASE_ON_TOKEN(ALIAS, alias)
		ARG_CASE_ON_TOKEN(ANCHOR, anchor)
		ARG_CASE_ON_TOKEN(TAG, tag)
		ARG_CASE_ON_TOKEN(SCALAR, scalar)
	}

	return handle_deletion ? delete_token(token) : token.type;
}


#define DEFAULT_ON_TOKEN(which_token) \
	void yaml_handler::on_##which_token##_token() { \
		if(do_on_##which_token##_token) \
			do_on_##which_token##_token(); \
	}

#define ARG_ON_TOKEN(which_token) \
	void yaml_handler::on_##which_token##_token(const which_token##_t & which_token) { \
		if(do_on_##which_token##_token) \
			do_on_##which_token##_token(which_token); \
	}


DEFAULT_ON_TOKEN(no)
DEFAULT_ON_TOKEN(stream_end)
DEFAULT_ON_TOKEN(document_start)
DEFAULT_ON_TOKEN(document_end)
DEFAULT_ON_TOKEN(block_sequence_start)
DEFAULT_ON_TOKEN(block_mapping_start)
DEFAULT_ON_TOKEN(block_end)
DEFAULT_ON_TOKEN(flow_sequence_start)
DEFAULT_ON_TOKEN(flow_sequence_end)
DEFAULT_ON_TOKEN(flow_mapping_start)
DEFAULT_ON_TOKEN(flow_mapping_end)
DEFAULT_ON_TOKEN(block_entry)
DEFAULT_ON_TOKEN(flow_entry)
DEFAULT_ON_TOKEN(key)
DEFAULT_ON_TOKEN(value)
ARG_ON_TOKEN(stream_start)
ARG_ON_TOKEN(version_directive)
ARG_ON_TOKEN(tag_directive)
ARG_ON_TOKEN(alias)
ARG_ON_TOKEN(anchor)
ARG_ON_TOKEN(tag)
ARG_ON_TOKEN(scalar)
