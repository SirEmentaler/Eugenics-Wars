#include <iostream>
#include <random>
#include "elitist_selection.h"
#include "genetic_algorithm.h"

struct point {
	double x;
	double y;
};

class point_generator {
public:
	point operator()() {
		return point {range_x(rand), range_y(rand)};
	}
private:
	std::mt19937 rand;
	std::uniform_real_distribution<> range_x {0.0, 18.0};
	std::uniform_real_distribution<> range_y {0.0, 15.0};
};

int main() {
	using algorithm_type = genetic_algorithm<point, double>;
	algorithm_type::context_type context;
	context.initial_population_size = 1000;
	context.breeding_population_size = 50;
	context.max_iterations = 1000;
	context.generator = point_generator();
	context.evaluator = [](const point& point) {
		const double a = point.x * point.x - point.y;
		const double b = 1 - point.x;
		return a * a * 100.0 + b * b + 10.0;
	};
	context.selector = elitist_selection();
	context.breeder = [](const point& lhs, const point& rhs) {
		return point {(lhs.x + rhs.x) / 2.0, (lhs.y + rhs.y) / 2.0};
	};
	algorithm_type algorithm(context);
	const auto result = algorithm();
	std::cout << result.value().x << ", " << result.value().y << std::endl;
	return 0;
}
