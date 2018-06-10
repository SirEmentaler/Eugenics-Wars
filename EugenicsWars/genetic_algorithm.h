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
#include "evaluated_specimen.h"
#include "repeat.h"

template<class Specimen, class Rating>
class genetic_algorithm {
public:
	using specimen_type = Specimen;
	using rating_type = Rating;
	using evaluated_specimen_type = evaluated_specimen<Specimen, Rating>;
	struct context_type;
	enum struct stage_type { generated, selected, bred };
	explicit genetic_algorithm(const context_type& context);
	explicit genetic_algorithm(context_type&& context);
	template<class... Functions>
	evaluated_specimen_type operator()(Functions&&... observers) const;
private:
	template<class... Functions>
	void communicate_stage(stage_type stage, const std::vector<evaluated_specimen_type>& specimens, Functions&&... observers) const;
	void evaluate(std::vector<evaluated_specimen_type>& specimens) const;
	std::vector<evaluated_specimen_type> breed(const std::vector<evaluated_specimen_type>& specimens) const;
	context_type context;
};

template<class Specimen, class Rating>
struct genetic_algorithm<Specimen, Rating>::context_type {
	std::size_t initial_population_size = 1;
	std::size_t breeding_population_size = 1;
	std::size_t max_iterations = 0;
	std::function<specimen_type()> generator;
	std::function<rating_type(const specimen_type&)> evaluator;
	std::function<void(std::vector<evaluated_specimen_type>&, std::size_t)> selector;
	std::function<specimen_type(const specimen_type&, const specimen_type&)> breeder;
	std::function<bool(rating_type, rating_type)> comparator;
};

template<class Specimen, class Rating>
inline genetic_algorithm<Specimen, Rating>::genetic_algorithm(const context_type& context)
	: context(context) {}

template<class Specimen, class Rating>
inline genetic_algorithm<Specimen, Rating>::genetic_algorithm(context_type&& context)
	: context(std::move(context)) {}

template<class Specimen, class Rating>
template<class... Functions>
inline auto genetic_algorithm<Specimen, Rating>::operator()(Functions&&... observers) const -> evaluated_specimen_type {
	std::vector<evaluated_specimen_type> specimens(context.initial_population_size);
	for (auto&& specimen : specimens) {
		specimen.value() = context.generator();
	}
	evaluate(specimens);
	communicate_stage(stage_type::generated, specimens, std::forward<Functions>(observers)...);
	repeat(context.max_iterations, [&] {
		context.selector(specimens, context.breeding_population_size);
		communicate_stage(stage_type::selected, specimens, std::forward<Functions>(observers)...);
		specimens = breed(specimens);
		evaluate(specimens);
		communicate_stage(stage_type::bred, specimens, std::forward<Functions>(observers)...);
	});
	return std::move(*std::max_element(specimens.begin(), specimens.end(), [this](const evaluated_specimen_type& lhs, const evaluated_specimen_type& rhs) {
		return context.comparator(lhs.rating(), rhs.rating());
	}));
}

template<class Specimen, class Rating>
template<class... Functions>
inline void genetic_algorithm<Specimen, Rating>::communicate_stage(stage_type stage, const std::vector<evaluated_specimen_type>& specimens, Functions&&... observers) const {
	(void)stage;
	(void)specimens;
	((void)(observers(*this, stage, specimens)), ...);
}

template<class Specimen, class Rating>
inline void genetic_algorithm<Specimen, Rating>::evaluate(std::vector<evaluated_specimen_type>& specimens) const {
	for (auto&& specimen : specimens) {
		specimen.evaluate(context.evaluator);
	}
}

template<class Specimen, class Rating>
inline auto genetic_algorithm<Specimen, Rating>::breed(const std::vector<evaluated_specimen_type>& specimens) const -> std::vector<evaluated_specimen_type> {
	const std::size_t specimen_count = specimens.size();
	std::vector<evaluated_specimen_type> result;
	result.reserve(specimen_count * (specimen_count - 1) / 2);
	for (auto it = specimens.begin(); it != specimens.end(); ++it) {
		const specimen_type& father = it->value();
		std::for_each(std::next(it), specimens.end(), [&](const evaluated_specimen_type& mother) {
			result.emplace_back(evaluated_specimen_type {context.breeder(father, mother.value())});
		});
	}
	return result;
}

#endif
