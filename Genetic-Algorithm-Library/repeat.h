////////////////////////////////////////////////////////////
//
// Copyright (c) 2018 Jan Filipowicz, Filip Turobos
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
////////////////////////////////////////////////////////////

#ifndef EUGENICS_WARS_REPEAT_H
#define EUGENICS_WARS_REPEAT_H

#include <cstddef>
#include <utility>

/// Calls provided function object \a f exactly \a n times.
/**
Equivalent to calling the function object in a \c for loop but expresses
intent more clearly and does not expose the loop index to the function.

@tparam NullaryFunction A type that satifies FunctionObject for an empty
	                    argument list

@param[in] n Number of times to call the function object
@param[in] f Function object to call
*/
template<class NullaryFunction>
constexpr void repeat(std::size_t n, NullaryFunction&& f)
noexcept(noexcept(std::forward<NullaryFunction>(f)())) {
	while (n--) {
		std::forward<NullaryFunction>(f)();
	}
}

/// Calls function object \a f up to \a n times until \c false is returned.
/**
Equivalent to calling the function object in a `for` loop but expresses
intent more clearly and does not expose the loop index to the function.

@tparam NullaryPredicate A type that satifies FunctionObject for an empty
                         argument list and whose return type is contextually
                         convertible to \c bool

@param[in] n Maximum number of times to call the function object
@param[in] f Function object to call
*/
template<class NullaryPredicate>
constexpr void repeat_while(std::size_t n, NullaryPredicate&& f)
noexcept(noexcept(std::forward<NullaryPredicate>(f)())) {
	while (n-- && static_cast<bool>(std::forward<NullaryPredicate>(f)()));
}

/// Calls function object \a f up to \a n times until \c true is returned.
/**
Equivalent to calling the function object in a `for` loop but expresses
intent more clearly and does not expose the loop index to the function.

@tparam NullaryPredicate A type that satifies FunctionObject for an empty
                         argument list and whose return type is contextually
                         convertible to \c bool

@param[in] n Maximum number of times to call the function object
@param[in] f Function object to call
*/
template<class NullaryPredicate>
constexpr void repeat_until(std::size_t n, NullaryPredicate&& f)
noexcept(noexcept(std::forward<NullaryPredicate>(f)())) {
	while (n--) {
		if (std::forward<NullaryPredicate>(f)())
			return;
	}
}

#endif
