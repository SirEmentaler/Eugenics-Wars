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

#ifndef SALESMAN_EXAMPLE_PATH_MUTATOR_H
#define SALESMAN_EXAMPLE_PATH_MUTATOR_H

#include <algorithm>
#include <functional>
#include <iterator>
#include <random>
#include <vector>
#include "permutation.h"

template<class UniformRandomBitGenerator>
class path_node_swapper {
public:
	explicit path_node_swapper(UniformRandomBitGenerator& g);
	void operator()(permutation& perm);
private:
	UniformRandomBitGenerator& rand;
};

template<class UniformRandomBitGenerator>
inline path_node_swapper<UniformRandomBitGenerator>::path_node_swapper(UniformRandomBitGenerator& g)
	: rand(g) {}

template<class UniformRandomBitGenerator>
inline void path_node_swapper<UniformRandomBitGenerator>::operator()(permutation& perm) {
	Expects(perm.size() > 0);
	std::vector<std::reference_wrapper<permutation::value_type>> sample;
	sample.reserve(2);
	std::sample(perm.begin(), perm.end(), std::back_inserter(sample), 2, rand);
	std::swap(sample.front().get(), sample.back().get());
}

template<class UniformRandomBitGenerator>
class path_node_relocator {
public:
	explicit path_node_relocator(UniformRandomBitGenerator& g);
	void operator()(permutation& perm) const;
private:
	UniformRandomBitGenerator& rand;
};

template<class UniformRandomBitGenerator>
inline path_node_relocator<UniformRandomBitGenerator>::path_node_relocator(UniformRandomBitGenerator& g)
	: rand(g) {}

template<class UniformRandomBitGenerator>
inline void path_node_relocator<UniformRandomBitGenerator>::operator()(permutation& perm) const {
	Expects(perm.size() > 0);
	std::uniform_int_distribution<std::size_t> distribution(0, perm.size());
	auto left = perm.begin() + distribution(rand);
	auto right = perm.begin() + distribution(rand);
	if (left < right)
		std::rotate(left, std::next(left), right);
	else if (left > right)
		std::rotate(right, std::prev(left), left);
}

#endif
