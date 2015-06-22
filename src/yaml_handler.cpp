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
