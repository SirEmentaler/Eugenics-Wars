#include "disjoint_set_data_structure.h"
#include <cstddef>
#include <numeric>

disjoint_set_data_structure::disjoint_set_data_structure(std::size_t n)
	: parents(n), ranks(n) {
	std::iota(parents.begin(), parents.end(), std::size_t());
}

std::size_t disjoint_set_data_structure::find(std::size_t index) const {
	std::size_t& parent = parents[index];
	if (parent != index)
		parent = find(parent);
	return parent;
}

bool disjoint_set_data_structure::merge(std::size_t lhs, std::size_t rhs) {
	const std::size_t lhs_root = find(lhs);
	const std::size_t rhs_root = find(rhs);
	if (lhs_root == rhs_root)
		return false;
	std::size_t& lhs_rank = ranks[lhs_root];
	const std::size_t rhs_rank = ranks[rhs_root];
	if (lhs_rank < rhs_rank)
		parents[lhs_root] = rhs_root;
	else {
		parents[rhs_root] = lhs_root;
		if (lhs_rank == rhs_rank)
			lhs_rank++;
	}
	return true;
}
