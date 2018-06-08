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
	std::uniform_real_distribution<> range_x {-10.0, 10.0};
	std::uniform_real_distribution<> range_y {-10.0, 10.0};
};

int main() {
	using algorithm_type = genetic_algorithm<point, double>;
	algorithm_type::context_type context;
	context.initial_population_size = 1000;
	context.breeding_population_size = 10;
	context.max_iterations = 100;
	context.generator = point_generator();
	context.evaluator = [](const point& point) {
		return (point.x - 2) * (point.x - 2) + (point.y + 3) * (point.y + 3);
	};
	context.selector = elitist_selection<algorithm_type::evaluated_specimen_type>();
	context.breeder = [](const point& lhs, const point& rhs) {
		return point {(lhs.x + rhs.x) / 2.0, (lhs.y + rhs.y) / 2.0};
	};
	algorithm_type algorithm(context);
	const point result = algorithm().value();
	std::cout << result.x << ", " << result.y << std::endl;
	return 0;
}
