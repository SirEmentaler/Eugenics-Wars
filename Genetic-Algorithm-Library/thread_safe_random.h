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

#ifndef GENETIC_ALGORITHM_LIBRARY_THREAD_SAFE_RANDOM_H
#define GENETIC_ALGORITHM_LIBRARY_THREAD_SAFE_RANDOM_H

#include <chrono>
#include <random>
#include <gsl/gsl_util>

struct default_random_initialization_policy {
	static std::seed_seq& get();
};

inline std::seed_seq& default_random_initialization_policy::get() {
	static thread_local std::seed_seq sequence {
		gsl::narrow_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count()),
		std::random_device()(),
	};
	return sequence;
}

template<class UniformRandomBitGenerator, class InitializationPolicy = default_random_initialization_policy>
struct thread_safe_random_bit_generator {
	using underlying_generator_type = UniformRandomBitGenerator;
	using result_type = typename underlying_generator_type::result_type;
	static constexpr result_type min() noexcept(noexcept(underlying_generator_type::min()));
	static constexpr result_type max() noexcept(noexcept(underlying_generator_type::max()));
	result_type operator()();
};

template<class UniformRandomBitGenerator, class InitializationPolicy>
constexpr auto thread_safe_random_bit_generator<UniformRandomBitGenerator, InitializationPolicy>::min() noexcept(noexcept(underlying_generator_type::min())) -> result_type {
	return underlying_generator_type::min();
}

template<class UniformRandomBitGenerator, class InitializationPolicy>
constexpr auto thread_safe_random_bit_generator<UniformRandomBitGenerator, InitializationPolicy>::max() noexcept(noexcept(underlying_generator_type::max())) -> result_type {
	return underlying_generator_type::max();
}

template<class UniformRandomBitGenerator, class InitializationPolicy>
inline auto thread_safe_random_bit_generator<UniformRandomBitGenerator, InitializationPolicy>::operator()() -> result_type {
	static thread_local underlying_generator_type generator(InitializationPolicy::get());
	return generator();
}

#endif
