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
#ifndef YAML_HANDLER_HPP
#define YAML_HANDLER_HPP


#define YAML_DECLARE_STATIC


#include <yaml.h>
#include <memory>
#include <utility>
#include <functional>


namespace libyaml {
	/** Override `on_*` functions **and** the `copy()` function
	  * OR
	  * assign to the `do_on_*` variables
	  */
	class yaml_handler {
		friend class yaml_reader;

		public:
			using stream_start_t      = decltype(std::declval<decltype(std::declval<yaml_token_t>().data)>().stream_start);
			using version_directive_t = decltype(std::declval<decltype(std::declval<yaml_token_t>().data)>().version_directive);
			using tag_directive_t     = decltype(std::declval<decltype(std::declval<yaml_token_t>().data)>().tag_directive);
			using alias_t             = decltype(std::declval<decltype(std::declval<yaml_token_t>().data)>().alias);
			using anchor_t            = decltype(std::declval<decltype(std::declval<yaml_token_t>().data)>().anchor);
			using tag_t               = decltype(std::declval<decltype(std::declval<yaml_token_t>().data)>().tag);
			using scalar_t            = decltype(std::declval<decltype(std::declval<yaml_token_t>().data)>().scalar);


			std::function<void(                           )> do_on_no_token;
			std::function<void(const stream_start_t      &)> do_on_stream_start_token;
			std::function<void(                           )> do_on_stream_end_token;
			std::function<void(const version_directive_t &)> do_on_version_directive_token;
			std::function<void(const tag_directive_t     &)> do_on_tag_directive_token;
			std::function<void(                           )> do_on_document_start_token;
			std::function<void(                           )> do_on_document_end_token;
			std::function<void(                           )> do_on_block_sequence_start_token;
			std::function<void(                           )> do_on_block_mapping_start_token;
			std::function<void(                           )> do_on_block_end_token;
			std::function<void(                           )> do_on_flow_sequence_start_token;
			std::function<void(                           )> do_on_flow_sequence_end_token;
			std::function<void(                           )> do_on_flow_mapping_start_token;
			std::function<void(                           )> do_on_flow_mapping_end_token;
			std::function<void(                           )> do_on_block_entry_token;
			std::function<void(                           )> do_on_flow_entry_token;
			std::function<void(                           )> do_on_key_token;
			std::function<void(                           )> do_on_value_token;
			std::function<void(const alias_t             &)> do_on_alias_token;
			std::function<void(const anchor_t            &)> do_on_anchor_token;
			std::function<void(const tag_t               &)> do_on_tag_token;
			std::function<void(const scalar_t            &)> do_on_scalar_token;


			inline yaml_handler();
			inline yaml_handler(const yaml_handler &);
			inline yaml_handler(yaml_handler &&);
			virtual ~yaml_handler();

			virtual std::unique_ptr<yaml_handler> clone() const;

			virtual void on_no_token                  (                           );
			virtual void on_stream_start_token        (const stream_start_t      &);
			virtual void on_stream_end_token          (                           );
			virtual void on_version_directive_token   (const version_directive_t &);
			virtual void on_tag_directive_token       (const tag_directive_t     &);
			virtual void on_document_start_token      (                           );
			virtual void on_document_end_token        (                           );
			virtual void on_block_sequence_start_token(                           );
			virtual void on_block_mapping_start_token (                           );
			virtual void on_block_end_token           (                           );
			virtual void on_flow_sequence_start_token (                           );
			virtual void on_flow_sequence_end_token   (                           );
			virtual void on_flow_mapping_start_token  (                           );
			virtual void on_flow_mapping_end_token    (                           );
			virtual void on_block_entry_token         (                           );
			virtual void on_flow_entry_token          (                           );
			virtual void on_key_token                 (                           );
			virtual void on_value_token               (                           );
			virtual void on_alias_token               (const alias_t             &);
			virtual void on_anchor_token              (const anchor_t            &);
			virtual void on_tag_token                 (const tag_t               &);
			virtual void on_scalar_token              (const scalar_t            &);
	};
}


libyaml::yaml_handler::yaml_handler() = default;
libyaml::yaml_handler::yaml_handler(const yaml_handler &) = default;
libyaml::yaml_handler::yaml_handler(yaml_handler &&) = default;

#endif // YAML_HANDLER_HPP
