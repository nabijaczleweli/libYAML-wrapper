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


#ifndef REFERENCE_WRAPPER_HPP
#define REFERENCE_WRAPPER_HPP


#include <utility>


namespace libyaml {
	namespace util {
		template <class T>
		class all_reference_wrapper {
		private:
			T * val;

		public:
			inline all_reference_wrapper(T && t) : val(const_cast<T *>(std::addressof(t))) {}
			inline all_reference_wrapper(const T & t) : val(const_cast<T *>(std::addressof(t))) {}
			inline constexpr all_reference_wrapper(all_reference_wrapper &&) = default;
			inline constexpr all_reference_wrapper(const all_reference_wrapper &) = default;

			inline all_reference_wrapper & operator=(const all_reference_wrapper &) = default;
			inline all_reference_wrapper & operator=(all_reference_wrapper &&) = default;

			inline operator T &() {
				return *val;
			}

			inline T & get() {
				return *val;
			}

			inline operator const T &() const {
				return *val;
			}

			inline const T & get() const {
				return *val;
			}
		};
	}
}


#endif  // REFERENCE_WRAPPER_HPP
