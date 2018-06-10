#include "path_merger.h"
#include <algorithm>
#include <functional>
#include <iterator>
#include <stack>
#include <utility>
#include <vector>
#include <gsl/span>
#include <repeat.h>
#include "disjoint_set_data_structure.h"

permutation path_merger::operator()(const permutation& lhs, const permutation& rhs) const {
	const std::size_t size = lhs.size();
	edge_vector lhs_edges = to_edges(lhs);
	edge_vector rhs_edges = to_edges(rhs);
	edge_vector result_edges;
	result_edges.reserve(size);
	std::set_intersection(lhs_edges.begin(), lhs_edges.end(), rhs_edges.begin(), rhs_edges.end(), std::back_inserter(result_edges));
	disjoint_set_data_structure components(size);
	std::vector<unsigned> missing_edges(size, 2);
	for (const auto& [left, right] : result_edges) {
		components.merge(left, right);
		missing_edges[left]--;
		missing_edges[right]--;
	}
	gsl::span<edge_type> spans[2] {lhs_edges, rhs_edges};
	while (!std::all_of(std::begin(spans), std::end(spans), std::mem_fn(&gsl::span<edge_type>::empty))) {
		for (auto&& span : spans) {
			auto it = std::find_if(span.begin(), span.end(), [&](const edge_type& edge) {
				const auto& [lhs, rhs] = edge;
				return missing_edges[lhs] && missing_edges[rhs] && components.find(lhs) != components.find(rhs);
			});
			if (it != span.end()) {
				const auto& [left, right] = *it;
				result_edges.emplace_back(left, right);
				components.merge(left, right);
				missing_edges[left]--;
				missing_edges[right]--;
			}
			span = span.subspan(it - span.begin());
		}
	}
	std::stack<unsigned, std::vector<unsigned>> s;
	for (unsigned i = 0; i < size; i++) {
		repeat(missing_edges[i], [&] {
			if (!s.empty() && components.find(s.top()) != components.find(i)) {
				const unsigned& left = s.top();
				const unsigned& right = i;
				result_edges.emplace_back(left, right);
				components.merge(left, right);
				missing_edges[left]--;
				missing_edges[right]--;
				s.pop();
			} else {
				s.push(i);
			}
		});
	}
	if (!s.empty()) {
		const unsigned left = s.top();
		s.pop();
		const unsigned& right = s.top();
		result_edges.emplace_back(left, right);
	}
	return to_permutation(result_edges);
}

auto path_merger::to_edges(const permutation& perm) const -> edge_vector {
	edge_vector result {std::minmax({perm.front(), perm.back()})};
	std::transform(std::next(perm.begin()), perm.end(), perm.begin(), std::inserter(result, result.end()), [](unsigned lhs, unsigned rhs) {
		return std::minmax({lhs, rhs});
	});
	std::sort(result.begin(), result.end());
	return result;
}

permutation path_merger::to_permutation(const edge_vector& edges) const {
	const std::size_t size = edges.size();
	std::vector<std::vector<unsigned>> graph(size);
	for (auto&& node : graph) {
		node.reserve(2);
	}
	for (const auto& [lhs, rhs] : edges) {
		graph[lhs].push_back(rhs);
		graph[rhs].push_back(lhs);
	}
	permutation result(size);
	unsigned prev = 0;
	unsigned cur = graph.front().front();
	std::for_each(std::next(result.begin()), result.end(), [&](unsigned& element) {
		element = cur;
		const auto& node = graph[cur];
		unsigned next = node.front() == prev ? node.back() : node.front();
		prev = cur;
		cur = next;
	});
	return result;
}
