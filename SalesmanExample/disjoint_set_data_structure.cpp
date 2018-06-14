#include "disjoint_set_data_structure.h"
#include <cstddef>
#include <numeric>
#include <gsl/gsl_assert>
#include <gsl/gsl_util>

disjoint_set_data_structure::disjoint_set_data_structure(std::size_t n)
	: parents(n), ranks(n) {
	std::iota(parents.begin(), parents.end(), std::size_t());
}

std::size_t disjoint_set_data_structure::find(std::size_t index) const {
	Expects(index < parents.size());
	std::size_t& parent = gsl::at(parents, index);
	if (parent != index)
		parent = find(parent);
	return parent;
}

bool disjoint_set_data_structure::merge(std::size_t lhs, std::size_t rhs) {
	Expects(lhs < parents.size());
	Expects(rhs < parents.size());
	const std::size_t lhs_root = find(lhs);
	const std::size_t rhs_root = find(rhs);
	if (lhs_root == rhs_root)
		return false;
	std::size_t& lhs_rank = gsl::at(ranks, lhs_root);
	const std::size_t rhs_rank = gsl::at(ranks, rhs_root);
	if (lhs_rank < rhs_rank)
		gsl::at(parents, lhs_root) = rhs_root;
	else {
		gsl::at(parents, rhs_root) = lhs_root;
		if (lhs_rank == rhs_rank)
			lhs_rank++;
	}
	return true;
}
