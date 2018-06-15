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

#ifndef EUGENICS_WARS_THREAD_SAFE_RANDOM_H
#define EUGENICS_WARS_THREAD_SAFE_RANDOM_H

#include <random>
#include <utility>

template<class UniformRandomBitGenerator = std::default_random_engine, class SeedSequence = std::seed_seq>
class thread_safe_random_bit_generator {
public:
	using underlying_generator_type = UniformRandomBitGenerator;
	using seed_sequence_type = SeedSequence;
	using result_type = typename underlying_generator_type::result_type;
	template<class... Args>
	thread_safe_random_bit_generator(Args&&... args) noexcept(noexcept(seed_sequence_type {std::forward<Args>(args)...}));
	static constexpr result_type min() noexcept(noexcept(underlying_generator_type::min()));
	static constexpr result_type max() noexcept(noexcept(underlying_generator_type::max()));
	result_type operator()();
private:
	seed_sequence_type seed_sequence;
};

template<class UniformRandomBitGenerator, class SeedSequence>
template<class... Args>
inline thread_safe_random_bit_generator<UniformRandomBitGenerator, SeedSequence>::thread_safe_random_bit_generator(Args&&... args) noexcept(noexcept(seed_sequence_type {std::forward<Args>(args)...}))
	: seed_sequence {std::forward<Args>(args)...} {}

template<class UniformRandomBitGenerator, class SeedSequence>
constexpr auto thread_safe_random_bit_generator<UniformRandomBitGenerator, SeedSequence>::min() noexcept(noexcept(underlying_generator_type::min())) -> result_type {
	return underlying_generator_type::min();
}

template<class UniformRandomBitGenerator, class SeedSequence>
constexpr auto thread_safe_random_bit_generator<UniformRandomBitGenerator, SeedSequence>::max() noexcept(noexcept(underlying_generator_type::max())) -> result_type {
	return underlying_generator_type::max();
}

template<class UniformRandomBitGenerator, class SeedSequence>
inline auto thread_safe_random_bit_generator<UniformRandomBitGenerator, SeedSequence>::operator()() -> result_type {
	static thread_local underlying_generator_type generator(seed_sequence);
	return generator();
}

#endif
