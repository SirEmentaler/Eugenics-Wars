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

#ifndef POINT_EXAMPLE_POINT_BREEDER_H
#define POINT_EXAMPLE_POINT_BREEDER_H

#include <random>
#include "point.h"

class point_average {
public:
	constexpr point operator()(const point& lhs, const point& rhs) const noexcept;
};

constexpr point point_average::operator()(const point & lhs, const point & rhs) const noexcept {
	return {(lhs.x + rhs.x) / 2.0, (lhs.y + rhs.y) / 2.0};
}

template<class UniformRandomBitGenerator>
class point_merge_coordinates {
public:
	explicit point_merge_coordinates(UniformRandomBitGenerator& g);
	point operator()(const point& lhs, const point& rhs);
private:
	UniformRandomBitGenerator& rand;
	std::bernoulli_distribution distribution {0.5};
};

template<class UniformRandomBitGenerator>
inline point_merge_coordinates<UniformRandomBitGenerator>::point_merge_coordinates(UniformRandomBitGenerator& g)
	: rand(g) {}

template<class UniformRandomBitGenerator>
inline point point_merge_coordinates<UniformRandomBitGenerator>::operator()(const point& lhs, const point& rhs) {
	return distribution(rand) ? point {lhs.x, rhs.y} : point {rhs.x, lhs.y};
}

#endif
