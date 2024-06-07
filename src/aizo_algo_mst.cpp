#include "aizo_algo_mst.hpp"
#include "aizo_ds_graphmatrix.hpp"
#include "aizo_ds_graphlist.hpp"
#include <fmt/core.h>
#include <queue>

namespace aizo::algo {

void MST::Solution::print() const {
  fmt::println("Weight: {}", m_weight);
  fmt::println("Tree edges:");
  for (const auto& edge : m_edges) {
    fmt::println("({} - {}): {}", edge.from, edge.to, edge.weight);
  }
}

MST::DisjointSet::DisjointSet(size_t size): parent(size), rank(size) {
  for (size_t i = 0; i < size; ++i) {
    parent[i] = i;
    rank[i]   = 0;
  }
}

ds::IGraph::Vertex MST::DisjointSet::find(ds::IGraph::Vertex v) {
  if (v != parent[v]) { parent[v] = find(parent[v]); }
  return parent[v];
}

void MST::DisjointSet::merge(ds::IGraph::Vertex u, ds::IGraph::Vertex v) {
  const auto rootU = find(u);
  const auto rootV = find(v);

  if (rootU == rootV) { return; }

  if (rank[rootU] > rank[rootV]) {
    parent[rootV] = rootU;
  } else {
    parent[rootU] = rootV;
    if (rank[rootU] == rank[rootV]) { ++rank[rootV]; }
  }
}

void MST::loadGraph(const ds::IGraph& graph) {
  m_graph = &graph;
}

std::optional< MST::Solution > MST::kruskal() {
  if (m_graph == nullptr) { return std::nullopt; }

  std::priority_queue< ds::IGraph::Edge,
                       std::vector< ds::IGraph::Edge >,
                       std::greater<> >
    left;

  const auto edges = m_graph->getEdges();

  for (const auto& edge : edges) { left.push(edge); }

  DisjointSet sets(m_graph->getNumVertices());

  Solution solution;

  for (size_t i = 0; i < m_graph->getNumVertices() - 1; ++i) {
    auto edge = left.top();
    left.pop();
    while (sets.find(edge.to) == sets.find(edge.from)) {
      edge = left.top();
      left.pop();
    }

    solution.m_edges.push_back(edge);
    solution.m_weight += edge.weight;
    sets.merge(edge.from, edge.to);
  }

  return solution;
}

std::optional< MST::Solution > MST::prim() {
  if (m_graph == nullptr) { return std::nullopt; }

  Solution solution;

  std::vector< bool > visited(m_graph->getNumVertices(), false);

  std::priority_queue< ds::IGraph::Edge,
                       std::vector< ds::IGraph::Edge >,
                       std::greater<> >
    left;

  auto vertices = m_graph->getVertices();

  visited[vertices[0]] = true;

  for (const auto& edge : m_graph->getOutgoingEdges(vertices[0])) {
    left.push(edge);
  }

  auto               source = vertices[0];
  ds::IGraph::Vertex dest{};

  while (solution.m_edges.size() < m_graph->getNumVertices() - 1) {
    auto edge = left.top();
    left.pop();

    dest = edge.to == source ? edge.from : edge.to;

    if (visited[dest]) { continue; }

    visited[dest] = true;
    solution.m_edges.push_back(edge);
    solution.m_weight += edge.weight;

    source = dest;

    for (const auto& nextEdge : m_graph->getOutgoingEdges(source)) {
      dest = nextEdge.to == source ? nextEdge.from : nextEdge.to;
      if (!visited[dest]) { left.push(nextEdge); }
    }
  }

  return solution;
}
} // namespace aizo::algo
