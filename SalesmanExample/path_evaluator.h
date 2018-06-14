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

#ifndef SALESMAN_EXAMPLE_PATH_EVALUATOR_H
#define SALESMAN_EXAMPLE_PATH_EVALUATOR_H

#include <functional>
#include <numeric>
#include <gsl/gsl_assert>
#include "permutation.h"

template<class Matrix>
class path_evaluator {
public:
	using matrix_type = Matrix;
	using row_type = typename matrix_type::value_type;
	using value_type = typename row_type::value_type;
	explicit path_evaluator(const matrix_type& matrix);
	value_type operator()(const permutation& perm) const;
private:
	matrix_type matrix;
};

template<class Matrix>
inline path_evaluator<Matrix>::path_evaluator(const matrix_type& matrix)
	: matrix(matrix) {}

template<class Matrix>
inline auto path_evaluator<Matrix>::operator()(const permutation& perm) const -> value_type {
	Expects(perm.size() == matrix.size());
	Expects(perm.size() > 0);
	const auto distance = [this](unsigned dest, unsigned src) {
		return gsl::at(gsl::at(matrix, src), dest);
	};
	return std::inner_product(std::next(perm.begin()), perm.end(), perm.begin(), distance(perm.front(), perm.back()), std::plus<>(), distance);
}

#endif
