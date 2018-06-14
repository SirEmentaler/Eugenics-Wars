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

#ifndef SALESMAN_EXAMPLE_PERMUTATION_GENERATOR_H
#define SALESMAN_EXAMPLE_PERMUTATION_GENERATOR_H

#include <algorithm>
#include <cstddef>
#include <functional>
#include <numeric>
#include <random>
#include "permutation.h"

template<class UniformRandomBitGenerator>
class permutation_generator {
public:
	permutation_generator(std::size_t n, UniformRandomBitGenerator& g) noexcept;
	permutation operator()();
private:
	std::size_t size;
	UniformRandomBitGenerator& rand;
};

template<class UniformRandomBitGenerator>
inline permutation_generator<UniformRandomBitGenerator>::permutation_generator(std::size_t n, UniformRandomBitGenerator& g) noexcept
	: size(n), rand(g) {}

template<class UniformRandomBitGenerator>
inline permutation permutation_generator<UniformRandomBitGenerator>::operator()() {
	permutation result(size);
	std::iota(result.begin(), result.end(), 0);
	std::shuffle(result.begin(), result.end(), rand);
	return result;
}

#endif
