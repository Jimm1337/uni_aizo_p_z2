#include "aizo_ds_graphlist.hpp"
#include <fmt/core.h>
#include <unordered_map>
#include <iostream>

namespace aizo::ds {

GraphList::GraphList(GraphType type):
  m_isDirected{ type == IGraph::GraphType::Directed }, m_numEdges{ 0 } {
}

size_t GraphList::addEdge(Edge edge) {
  for (const auto& node : m_list.at(edge.from)) {
    if (node.vertex == edge.to) { return getNumEdges(); }
  }

  m_list.at(edge.from).emplace_back(edge.to, edge.weight);

  if (!m_isDirected) {
    m_list.at(edge.to).emplace_back(edge.from, edge.weight);
  }

  return getNumEdges();
}

size_t GraphList::addVertex() {
  m_list.emplace_back();

  return getNumVertices();
}

std::vector< IGraph::Vertex > GraphList::getVertices() const {
  std::vector< Vertex > vertices;

  for (size_t i = 0; i < m_list.size(); ++i) { vertices.emplace_back(i); }

  return vertices;
}

std::vector< IGraph::Edge > GraphList::getEdges() const {
  std::vector< Edge > edges;

  if (m_isDirected) {
    for (size_t i = 0; i < m_list.size(); ++i) {
      for (const auto& node : m_list.at(i)) {
        edges.emplace_back(static_cast< Vertex >(i), node.vertex, node.weight);
      }
    }
  } else {
    std::vector< std::vector< bool > > included(
      getNumVertices(), std::vector< bool >(getNumVertices(), false));

    for (size_t i = 0; i < m_list.size(); ++i) {
      for (const auto& node : m_list.at(i)) {
        if (!included.at(node.vertex).at(i)) {
          edges.emplace_back(
            static_cast< Vertex >(i), node.vertex, node.weight);
          included.at(i).at(node.vertex) = true;
          included.at(node.vertex).at(i) = true;
        }
      }
    }
  }

  return edges;
}

std::vector< IGraph::Vertex > GraphList::getAdjacentVertices(
  Vertex vertex) const {
  std::vector< Vertex > vertices;

  for (const auto& node : m_list.at(vertex)) {
    vertices.emplace_back(node.vertex);
  }

  return vertices;
}

std::vector< IGraph::Edge > GraphList::getOutgoingEdges(Vertex vertex) const {
  std::vector< Edge > edges;

  for (const auto& node : m_list.at(vertex)) {
    edges.emplace_back(vertex, node.vertex, node.weight);
  }

  return edges;
}

bool GraphList::isDirected() const {
  return m_isDirected;
}

size_t GraphList::getNumVertices() const {
  return m_list.size();
}

size_t GraphList::getNumEdges() const {
  return m_numEdges;
}

void GraphList::clear() {
  m_list.clear();
}

void GraphList::print() const {
  for (size_t i = 0; i < getNumVertices(); ++i) {
    std::cout << fmt::format("v{:<5}|", i);

    for (const auto& node : m_list.at(i)) {
      std::cout << fmt::format("{}({}) -> ", node.vertex, node.weight);
    }

    std::cout << '\n';
  }
}

} // namespace aizo::ds
