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

#include <chrono>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <random>
#include <genetics.h>
#include "point.h"
#include "point_breeder.h"
#include "point_generator.h"
#include "point_mutator.h"

double f(const point& p) noexcept {
	if (p.x < 0.0 || p.x > 18.0 || p.y < 0.0 || p.y > 15.0 || p.x * p.y < 10.0 || p.x * p.y + p.x - p.y > 3.5)
		return std::numeric_limits<double>::max();
	const double a = p.x * p.x - p.y;
	const double b = 1 - p.x;
	return a * a * 100.0 + b * b + 10.0;
}

int main() {
	thread_safe_random_bit_generator<std::mt19937_64> rand(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	using algorithm_type = genetic_algorithm<point, double>;
	algorithm_type::context_type context;
	context.initial_population_size = 100;
	context.breeding_population_size = 10;
	context.max_iterations = 1000;
	context.generator = point_generator(rand);
	context.evaluator = f;
	context.selector = roulette_wheel_selection(rand, [](double x) noexcept { return 1.0 / (x * x); });
	// Also try:
	// context.selector = elitist_selection<std::greater<>>();
	context.breeder = mutating_breeder(point_merge_coordinates(rand), point_mutator(0.1, rand));
	context.comparator = std::greater<>();
	algorithm_type algorithm(context);
#ifdef LOGGING
	std::ofstream out_log("point.log");
	default_logger logger(out_log);
#endif
	repeat(10, [&] {
		const auto result = algorithm(
#ifdef LOGGING
			logger
#endif
		);
		std::cout << "Minimum found: f" << result.value() << " = " << result.rating() << std::endl;
	});
	return 0;
}
