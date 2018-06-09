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

#ifndef EUGENICS_WARS_ROULETTE_WHEEL_SELECTION_H
#define EUGENICS_WARS_ROULETTE_WHEEL_SELECTION_H

#include <algorithm>
#include <cstddef>
#include <random>
#include <utility>
#include <type_traits>
#include <vector>
#include "identity.h"

template<class UniformRandomBitGenerator, class Function = identity<>>
class roulette_wheel_selection {
public:
	explicit roulette_wheel_selection(const UniformRandomBitGenerator& g, Function f = Function());
	template<class Specimen>
	void operator()(std::vector<Specimen>& specimens, std::size_t n);
private:
	UniformRandomBitGenerator rand;
	Function probability_function;
};

template<class UniformRandomBitGenerator, class Function>
inline roulette_wheel_selection<UniformRandomBitGenerator, Function>::roulette_wheel_selection(const UniformRandomBitGenerator& g, Function f)
	: rand(g), probability_function(f) {}

template<class UniformRandomBitGenerator, class Function>
template<class Specimen>
inline void roulette_wheel_selection<UniformRandomBitGenerator, Function>::operator()(std::vector<Specimen>& specimens, std::size_t n) {
	using sample_type = std::common_type_t<double, decltype(probability_function(std::declval<typename Specimen::rating_type>()))>;
	using iterator_type = typename std::vector<Specimen>::iterator;
	using pair_type = std::pair<sample_type, iterator_type>;
	std::vector<pair_type> samples;
	samples.reserve(specimens.size());
	for (iterator_type it = specimens.begin(); it != specimens.end(); ++it) {
		const sample_type probability = probability_function(it->rating());
		const std::exponential_distribution<sample_type> distribution(probability);
		samples.emplace_back(distribution(rand), it);
	}
	std::nth_element(samples.begin(), samples.begin() + n, samples.end());
	samples.resize(n);
	iterator_type it = specimens.begin();
	for (const auto& sample : samples) {
		std::iter_swap(it++, sample.second);
	}
	specimens.resize(n);
}

#endif
