#include "aizo_algo_sp.hpp"
#include "fmt/core.h"
#include <queue>
#include <iostream>

namespace aizo::algo {

void SP::Solution::print() const {
  std::cout << "Shortest paths:\n";
  for (size_t i = 0; i < m_path.size(); ++i) {
    for (const auto& vert : m_path[i]) {
      std::cout << fmt::format("{} ", vert);
    }
    std::cout << fmt::format("|Cost: {}\n", m_weight[i]);
  }
}

SP::SP(ds::IGraph::Vertex source): m_source{ source }, m_graph{ nullptr } {
}

void SP::loadGraph(const ds::IGraph& graph) {
  m_graph = &graph;
}

std::optional< SP::Solution > SP::dijkstra() {
  Solution solution;

  std::vector< ds::IGraph::Vertex > vertices = m_graph->getVertices();
  std::vector< int64_t >            prev(vertices.size(), -1);
  std::vector< ds::IGraph::Weight > dist(
    vertices.size(), std::numeric_limits< ds::IGraph::Weight >::max());

  dist[m_source] = 0;

  std::priority_queue< ds::IGraph::Edge,
                       std::vector< ds::IGraph::Edge >,
                       std::greater<> >
    edges;

  std::vector< bool > visited(vertices.size(), false);

  for (const auto& edge : m_graph->getOutgoingEdges(m_source)) {
    edges.push(edge);
  }

  for (size_t i = 0; i < vertices.size() - 1; ++i) {
    while (!edges.empty()) {
      auto edge = edges.top();
      edges.pop();

      if (dist[edge.to] > dist[edge.from] + edge.weight) {
        dist[edge.to] = dist[edge.from] + edge.weight;
        prev[edge.to] = edge.from;

        visited[edge.from] = true;

        for (const auto& nextEdge : m_graph->getOutgoingEdges(edge.to)) {
          if (!visited[nextEdge.to]) { edges.push(nextEdge); }
        }
      }
    }
  }

  for (size_t i = 0; i < vertices.size(); ++i) {
    ds::IGraph::Vertex current = i;

    solution.m_path.emplace_back();

    while (current != -1) {
      solution.m_path.at(i).push_front(current);
      current = prev[current];
    }

    solution.m_weight.push_back(dist[i]);
  }

  return solution;
}

std::optional< SP::Solution > SP::bellmanFord() {
  Solution solution;

  std::vector< ds::IGraph::Weight > dist(
    m_graph->getNumVertices(),
    std::numeric_limits< ds::IGraph::Weight >::max());
  std::vector< ds::IGraph::Vertex > prev(m_graph->getNumVertices(), -1);

  dist[m_source] = 0;

  auto current = m_source;
  for (size_t i = 0; i < m_graph->getNumVertices(); ++i) {

    for (const auto& edge : m_graph->getOutgoingEdges(current)) {
      if (dist[edge.to] > dist[edge.from] + edge.weight) {
        dist[edge.to] = dist[edge.from] + edge.weight;
        prev[edge.to] = edge.from;
      }
    }

    current = (current + 1) % m_graph->getNumVertices();
  }

  for (size_t i = 0; i < m_graph->getNumVertices(); ++i) {
    ds::IGraph::Vertex cur = i;

    solution.m_path.emplace_back();

    while (cur != -1) {
      solution.m_path.at(i).push_front(cur);
      cur = prev[cur];
    }

    solution.m_weight.push_back(dist[i]);
  }

  return solution;
}

} // namespace aizo::algo
