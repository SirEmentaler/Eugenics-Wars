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

#include "identity.h"
#include <type_traits>
#include <utility>
#include <gtest/gtest.h>

TEST(Identity, GivenIntLiteral_ReturnsEqualValue) {
	identity id;
	static_assert(std::is_same_v<int&&, decltype(id(123))>);
	ASSERT_EQ(123, id(123));
}

TEST(Identity, GivenIntLvalue_ReturnsLvalueReferenceToIt) {
	identity id;
	int value = 123;
	id(value)++;
	static_assert(std::is_same_v<int&, decltype(id(value))>);
	ASSERT_EQ(124, value);
}

TEST(Identity, GivenIntConstLvalue_ReturnsConstLvalueReferenceToIt) {
	identity id;
	const int value = 123;
	static_assert(std::is_same_v<const int&, decltype(id(value))>);
	ASSERT_EQ(&value, &id(value));
}

TEST(Identity, GivenIntRvalue_ReturnsRvalueReferenceToIt) {
	identity id;
	int value = 123;
	static_assert(std::is_same_v<int&&, decltype(id(std::move(value)))>);
	ASSERT_EQ(123, id(std::move(value)));
}
