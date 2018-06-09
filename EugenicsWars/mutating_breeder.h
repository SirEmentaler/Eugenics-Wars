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

#ifndef EUGENICS_WARS_MUTATING_BREEDER_H
#define EUGENICS_WARS_MUTATING_BREEDER_H

template<class Breeder, class Mutator>
class mutating_breeder {
public:
	mutating_breeder(const Breeder& breeder, const Mutator& mutator);
	template<class Specimen>
	Specimen operator()(const Specimen& lhs, const Specimen& rhs);
private:
	Breeder underlying_breeder;
	Mutator mutator;
};

template<class Breeder, class Mutator>
inline mutating_breeder<Breeder, Mutator>::mutating_breeder(const Breeder& breeder, const Mutator& mutator)
	: underlying_breeder(breeder), mutator(mutator) {}

template<class Breeder, class Mutator>
template<class Specimen>
inline Specimen mutating_breeder<Breeder, Mutator>::operator()(const Specimen& lhs, const Specimen& rhs) {
	return mutator(underlying_breeder(lhs, rhs));
}

#endif
