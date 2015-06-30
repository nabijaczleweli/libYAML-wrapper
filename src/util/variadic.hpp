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


#ifndef VARIADIC_HPP
#define VARIADIC_HPP


#include <utility>


namespace libyaml {
	namespace util {
		template <class T, class... TT>
		struct over_all {
			using next  = over_all<TT...>;
			enum { size = 1 + next::size };

			template <class C>
			inline constexpr static C for_each(C cbk, T && tval, TT &&... ttval) {
				cbk(std::forward<T>(tval));
				next::for_each(cbk, std::forward<TT>(ttval)...);
				return cbk;
			}

			template <class C>
			inline constexpr C operator()(C cbk, T && tval, TT &&... ttval) const {
				return for_each(cbk, std::forward<T>(tval), std::forward<TT>(ttval)...);
			}
		};

		template <class T>
		struct over_all<T> {
			enum { size = 1 };

			template <class C>
			inline constexpr static C for_each(C cbk, T && tval) {
				cbk(std::forward<T>(tval));
				return cbk;
			}

			template <class C>
			inline constexpr C operator()(C cbk, T && tval) const {
				return for_each(cbk, std::forward<T>(tval));
			}
		};
	}
}


#endif  // VARIADIC_HPP
