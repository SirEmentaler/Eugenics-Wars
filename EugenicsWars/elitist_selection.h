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

#ifndef EUGENICS_WARS_ELITIST_SELECTION_H
#define EUGENICS_WARS_ELITIST_SELECTION_H

#include <algorithm>
#include <cstddef>
#include <functional>
#include <vector>
#include <gsl/gsl_assert>

template<class Compare = std::less<>>
class elitist_selection {
public:
	explicit elitist_selection(const Compare& comp = Compare());
	template<class Specimen>
	void operator()(std::vector<Specimen>& specimens, std::size_t n) const;
private:
	Compare comparator;
};

template<class Compare>
inline elitist_selection<Compare>::elitist_selection(const Compare& comp)
	: comparator(comp) {}

template<class Compare>
template<class Specimen>
inline void elitist_selection<Compare>::operator()(std::vector<Specimen>& specimens, std::size_t n) const {
	Expects(specimens.size() >= n);
	std::nth_element(specimens.begin(), specimens.begin() + n, specimens.end(), [this](const Specimen& lhs, const Specimen& rhs) {
		return comparator(rhs.rating(), lhs.rating());
	});
	specimens.resize(n);
}

#endif
