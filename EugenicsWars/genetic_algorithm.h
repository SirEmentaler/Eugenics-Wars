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

#ifndef EUGENICS_WARS_GENETIC_ALGORITHM_H
#define EUGENICS_WARS_GENETIC_ALGORITHM_H

#include <algorithm>
#include <functional>
#include <iterator>
#include <utility>
#include <vector>
#include "repeat.h"

template<class Specimen, class Rating>
class genetic_algorithm {
public:
	using specimen_type = Specimen;
	using rating_type = Rating;
	struct evaluated_specimen;
	struct context_type;
	genetic_algorithm(const context_type& context);
	genetic_algorithm(context_type&& context);
	evaluated_specimen operator()() const;
private:
	void evaluate(std::vector<evaluated_specimen>& specimens) const;
	std::vector<evaluated_specimen> breed(const std::vector<evaluated_specimen>& specimens) const;
	context_type context;
};

template<class Specimen, class Rating>
struct genetic_algorithm<Specimen, Rating>::evaluated_specimen {
	specimen_type value;
	rating_type rating;
	constexpr friend bool operator==(const evaluated_specimen& lhs, const evaluated_specimen& rhs) {
		return lhs.rating == rhs.rating;
	}
	constexpr friend bool operator!=(const evaluated_specimen& lhs, const evaluated_specimen& rhs) {
		return lhs.rating != rhs.rating;
	}
	constexpr friend bool operator<(const evaluated_specimen& lhs, const evaluated_specimen& rhs) {
		return lhs.rating < rhs.rating;
	}
	constexpr friend bool operator>(const evaluated_specimen& lhs, const evaluated_specimen& rhs) {
		return lhs.rating > rhs.rating;
	}
	constexpr friend bool operator<=(const evaluated_specimen& lhs, const evaluated_specimen& rhs) {
		return lhs.rating <= rhs.rating;
	}
	constexpr friend bool operator>=(const evaluated_specimen& lhs, const evaluated_specimen& rhs) {
		return lhs.rating >= rhs.rating;
	}
};

template<class Specimen, class Rating>
struct genetic_algorithm<Specimen, Rating>::context_type {
	std::size_t initial_population_size = 1;
	std::size_t breeding_population_size = 1;
	std::size_t max_iterations = 0;
	std::function<specimen_type()> generator;
	std::function<rating_type(const specimen_type&)> evaluator;
	std::function<void(std::vector<evaluated_specimen>&, std::size_t)> selector;
	std::function<specimen_type(const specimen_type&, const specimen_type&)> breeder;
};

template<class Specimen, class Rating>
inline genetic_algorithm<Specimen, Rating>::genetic_algorithm(const context_type& context)
	: context(context) {}

template<class Specimen, class Rating>
inline genetic_algorithm<Specimen, Rating>::genetic_algorithm(context_type&& context)
	: context(std::move(context)) {}

template<class Specimen, class Rating>
inline auto genetic_algorithm<Specimen, Rating>::operator()() const -> evaluated_specimen {
	std::vector<evaluated_specimen> specimens(context.initial_population_size);
	for (auto&& specimen : specimens) {
		specimen.value = context.generator();
	}
	repeat(context.max_iterations, [&] {
		evaluate(specimens);
		context.selector(specimens, context.breeding_population_size);
		specimens = breed(specimens);
	});
	evaluate(specimens);
	return std::move(*std::min_element(specimens.begin(), specimens.end()));
}

template<class Specimen, class Rating>
inline void genetic_algorithm<Specimen, Rating>::evaluate(std::vector<evaluated_specimen>& specimens) const {
	for (auto&& specimen : specimens) {
		specimen.rating = context.evaluator(specimen.value);
	}
}

template<class Specimen, class Rating>
inline auto genetic_algorithm<Specimen, Rating>::breed(const std::vector<evaluated_specimen>& specimens) const -> std::vector<evaluated_specimen> {
	const std::size_t specimen_count = specimens.size();
	std::vector<evaluated_specimen> result;
	result.reserve(specimen_count * (specimen_count - 1) / 2);
	for (auto it = specimens.begin(); it != specimens.end(); ++it) {
		const specimen_type& father = it->value;
		std::for_each(std::next(it), specimens.end(), [&](const evaluated_specimen& mother) {
			result.emplace_back(evaluated_specimen {context.breeder(father, mother.value)});
		});
	}
	return result;
}

#endif
