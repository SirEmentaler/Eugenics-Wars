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

#ifndef GENETIC_ALGORITHM_LIBRARY_EVALUATED_SPECIMEN_H
#define GENETIC_ALGORITHM_LIBRARY_EVALUATED_SPECIMEN_H

#include <optional>
#include <utility>
#include <tuple>
#include <type_traits>
#include <gsl/gsl_assert>

template<class Specimen, class Rating>
class evaluated_specimen {
public:
	using value_type = Specimen;
	using rating_type = Rating;
	template<class... Args, class = std::enable_if_t<!std::is_same_v<std::tuple<std::decay_t<Args>...>, std::tuple<evaluated_specimen>>>>
	constexpr evaluated_specimen(Args&&... args) noexcept(noexcept(value_type(std::forward<Args>(args)...)));
	constexpr value_type& value() & noexcept;
	constexpr const value_type& value() const& noexcept;
	constexpr bool has_rating() const noexcept;
	constexpr rating_type rating() const;
	template<class Function>
	void evaluate(Function&& evaluator);
private:
	value_type specimen;
	std::optional<rating_type> grade;
};

template<class Specimen, class Rating>
template<class... Args, class>
constexpr evaluated_specimen<Specimen, Rating>::evaluated_specimen(Args&&... args) noexcept(noexcept(value_type(std::forward<Args>(args)...)))
	: specimen(std::forward<Args>(args)...) {}

template<class Specimen, class Rating>
constexpr auto evaluated_specimen<Specimen, Rating>::value() & noexcept -> value_type& {
	return specimen;
}

template<class Specimen, class Rating>
constexpr auto evaluated_specimen<Specimen, Rating>::value() const& noexcept -> const value_type& {
	return specimen;
}

template<class Specimen, class Rating>
constexpr bool evaluated_specimen<Specimen, Rating>::has_rating() const noexcept {
	return grade.has_value();
}

template<class Specimen, class Rating>
constexpr auto evaluated_specimen<Specimen, Rating>::rating() const -> rating_type {
	return grade.value();
}

template<class Specimen, class Rating>
template<class Function>
inline void evaluated_specimen<Specimen, Rating>::evaluate(Function&& evaluator) {
	grade.emplace(evaluator(std::as_const(specimen)));
	Ensures(has_rating());
}

#endif
