#include "aizo_algo_sp.hpp"
#include "fmt/core.h"
#include <queue>

namespace aizo::algo {

void SP::Solution::print() const {
  if (m_negativeCycle) {
    fmt::println("Negative cycle detected.");
    return;
  }

  fmt::println("Shortest paths:");
  for (size_t i = 0; i < m_path.size(); ++i) {
    for (const auto& vert : m_path[i]) { fmt::print("{} ", vert); }
    fmt::println("|Cost: {}", m_weight[i]);
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
    std::list< ds::IGraph::Edge > path;
    ds::IGraph::Vertex            current = i;

    solution.m_path.emplace_back();

    while (current != -1) {
      solution.m_path.at(i).push_front(current);
      current = prev[current];
    }

    solution.m_weight.push_back(dist[i]);
  }

  solution.m_negativeCycle = false;

  return solution;
}

std::optional< SP::Solution > SP::bellmanFord() {
  Solution solution;

  std::vector< ds::IGraph::Weight > dist(
    m_graph->getNumVertices(),
    std::numeric_limits< ds::IGraph::Weight >::max());
  std::vector< ds::IGraph::Vertex > prev(m_graph->getNumVertices(), -1);

  dist[m_source] = 0;

  bool done = false;
  for (size_t i = 0; i < m_graph->getNumVertices() - 1; ++i) {
    bool relaxed = false;
    for (const auto& edge : m_graph->getVertices()) {
      for (const auto& nextEdge : m_graph->getOutgoingEdges(edge)) {
        if (dist[nextEdge.to] > dist[nextEdge.from] + nextEdge.weight) {
          dist[nextEdge.to] = dist[nextEdge.from] + nextEdge.weight;
          prev[nextEdge.to] = nextEdge.from;
          relaxed           = true;
        }
      }
      if (!relaxed) {
        done = true;
        break;
      }
    }
  }

  if (!done) {
    for (const auto& edge : m_graph->getVertices()) {
      for (const auto& nextEdge : m_graph->getOutgoingEdges(edge)) {
        if (dist[nextEdge.to] > dist[nextEdge.from] + nextEdge.weight) {
          solution.m_negativeCycle = true;
          return solution;
        }
      }
    }
  }

  for (size_t i = 0; i < m_graph->getNumVertices(); ++i) {
    std::list< ds::IGraph::Vertex > path;
    ds::IGraph::Vertex              current = i;

    solution.m_path.emplace_back();

    while (current != -1) {
      solution.m_path.at(i).push_front(current);
      current = prev[current];
    }

    solution.m_weight.push_back(dist[i]);
  }

  return solution;
}

} // namespace aizo::algo
