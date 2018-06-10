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

#ifndef EUGENICS_WARS_CHAIN_MUTATION_H
#define EUGENICS_WARS_CHAIN_MUTATION_H

#include <utility>
#include <tuple>

template<class... Mutations>
class chain_mutation {
public:
	explicit chain_mutation(Mutations&&... mutations);
	template<class T>
	void operator()(T& specimen);
private:
	using tuple_type = std::tuple<Mutations...>;
	template<std::size_t I, class T>
	void chain_call(T& specimen);
	tuple_type mutations;
};

template<class... Mutations>
inline chain_mutation<Mutations...>::chain_mutation(Mutations&&... mutations)
	: mutations(std::forward<Mutations>(mutations)...) {}

template<class... Mutations>
template<class T>
inline void chain_mutation<Mutations...>::operator()(T& specimen) {
	return chain_call<0>(specimen);
}

template<class... Mutations>
template<std::size_t I, class T>
inline void chain_mutation<Mutations...>::chain_call(T& specimen) {
	if constexpr (I < std::tuple_size_v<tuple_type>) {
		std::get<I>(mutations)(specimen);
		chain_call<I + 1>(specimen);
	}
}

#endif
