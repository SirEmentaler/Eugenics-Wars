#include "path_merger.h"
#include <algorithm>
#include <iterator>
#include <utility>
#include <vector>
#include "disjoint_set_data_structure.h"

permutation path_merger::operator()(const permutation& lhs, const permutation& rhs) const {
	const std::size_t size = lhs.size();
	edge_set lhs_edges = to_edges(lhs);
	edge_set rhs_edges = to_edges(rhs);
	edge_set common_edges;
	std::set_intersection(lhs_edges.begin(), lhs_edges.end(), rhs_edges.begin(), rhs_edges.end(), std::inserter(common_edges, common_edges.end()));
	disjoint_set_data_structure connected_components(size);
	for (const auto& edge : common_edges) {
		connected_components.merge(edge.first, edge.second);
	}
	// TODO
	return lhs;
}

auto path_merger::to_edges(const permutation& perm) const -> edge_set {
	edge_set result {std::minmax({perm.front(), perm.back()})};
	std::transform(std::next(perm.begin()), perm.end(), perm.begin(), std::inserter(result, result.end()), [](unsigned lhs, unsigned rhs) {
		return std::minmax({lhs, rhs});
	});
	return result;
}
