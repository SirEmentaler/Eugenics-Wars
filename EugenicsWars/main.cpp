#include <chrono>
#include <cmath>
#include <functional>
#include <iostream>
#include <random>
#include "elitist_selection.h"
#include "genetic_algorithm.h"
#include "roulette_wheel_selection.h"

struct point {
	double x;
	double y;
};

template<class UniformRandomBitGenerator>
class point_generator {
public:
	point_generator(const UniformRandomBitGenerator& g)
		: rand(g) {}
	point operator()() {
		return point {range_x(rand), range_y(rand)};
	}
private:
	UniformRandomBitGenerator rand;
	std::uniform_real_distribution<> range_x {0.0, 18.0};
	std::uniform_real_distribution<> range_y {0.0, 15.0};
};

double f(const point& point) noexcept {
	const double a = point.x * point.x - point.y;
	const double b = 1 - point.x;
	return a * a * 100.0 + b * b + 10.0;
}

point average(const point& lhs, const point& rhs) noexcept {
	return {(lhs.x + rhs.x) / 2.0, (lhs.y + rhs.y) / 2.0};
}

double negative_exponent(double x) noexcept {
	return std::exp(-x);
}

int main() {
	const std::mt19937_64 rand(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	using algorithm_type = genetic_algorithm<point, double>;
	algorithm_type::context_type context;
	context.initial_population_size = 10000;
	context.breeding_population_size = 200;
	context.max_iterations = 10;
	context.generator = point_generator(rand);
	context.evaluator = f;
	context.selector = roulette_wheel_selection(rand, &negative_exponent);
	context.breeder = average;
	context.comparator = std::greater<>();
	algorithm_type algorithm(context);
	repeat(10, [&] {
		const auto result = algorithm();
		std::cout << "Minimum found has value " << result.rating() << " at point ";
		std::cout << result.value().x << ", " << result.value().y << std::endl;
	});
	return 0;
}
