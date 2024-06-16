#include <fmt/core.h>
#include "aizo_ds_graphmatrix.hpp"
#include <iostream>

namespace aizo::ds {

GraphMatrix::GraphMatrix(GraphType type):
  m_isDirected{ type == GraphType::Directed }, m_vertexCount{ 0 } {
}

size_t GraphMatrix::addEdge(Edge edge) {
  std::vector< Weight > newEdge(getNumVertices(), 0);

  if (m_isDirected) {
    for (const auto& currentEdge : m_matrix) {
      if (currentEdge.at(edge.from) > 0 && currentEdge.at(edge.to) < 0) {
        return getNumEdges();
      }
    }

    newEdge.at(edge.from) = edge.weight;
    newEdge.at(edge.to)   = -edge.weight;
  } else {
    for (const auto& currentEdge : m_matrix) {
      if (currentEdge.at(edge.from) > 0 && currentEdge.at(edge.to) > 0) {
        return getNumEdges();
      }
    }

    newEdge.at(edge.from) = edge.weight;
    newEdge.at(edge.to)   = edge.weight;
  }

  m_matrix.emplace_back(newEdge);
  m_negativeEdge.emplace_back(edge.weight < 0);

  return getNumEdges();
}

size_t GraphMatrix::addVertex() {
  for (auto& edge : m_matrix) { edge.emplace_back(0); }

  ++m_vertexCount;

  return getNumVertices();
}

std::vector< GraphMatrix::Vertex > GraphMatrix::getVertices() const {
  std::vector< Vertex > vertices;
  for (size_t i = 0; i < m_vertexCount; ++i) { vertices.push_back(i); }
  return vertices;
}

std::vector< GraphMatrix::Edge > GraphMatrix::getEdges() const {
  std::vector< Edge > edges;

  auto src    = std::numeric_limits< Vertex >::max();
  auto dest   = std::numeric_limits< Vertex >::max();
  auto weight = std::numeric_limits< Weight >::max();

  auto vertIdx = 0U;
  auto edgeIdx = 0U;

  for (const auto& edge : m_matrix) {
    if (m_isDirected) {
      for (const auto cost : edge) {
        if (cost == 0) {
          ++vertIdx;
          continue;
        }

        if (cost < 0) {
          dest = vertIdx;
        } else {
          src    = vertIdx;
          weight = m_negativeEdge.at(edgeIdx) ? -cost : cost;
        }

        ++vertIdx;

        if ((src != std::numeric_limits< Vertex >::max()) &&
            (dest != std::numeric_limits< Vertex >::max())) {
          break;
        }
      }
    } else {
      for (const auto cost : edge) {
        if (cost == 0) {
          ++vertIdx;
          continue;
        }

        if (src == std::numeric_limits< Vertex >::max()) {
          src    = vertIdx;
          weight = m_negativeEdge.at(edgeIdx) ? -cost : cost;
        } else if (dest == std::numeric_limits< Vertex >::max()) {
          dest = vertIdx;
        }

        ++vertIdx;

        if ((src != std::numeric_limits< Vertex >::max()) &&
            (dest != std::numeric_limits< Vertex >::max())) {
          break;
        }
      }
    }

    edges.emplace_back(src, dest, weight);

    src    = std::numeric_limits< Vertex >::max();
    dest   = std::numeric_limits< Vertex >::max();
    weight = std::numeric_limits< Weight >::max();

    vertIdx = 0U;
    ++edgeIdx;
  }

  return edges;
}

std::vector< GraphMatrix::Vertex > GraphMatrix::getAdjacentVertices(
  Vertex vertex) const {
  std::vector< Vertex > vertices;

  const auto edges = getOutgoingEdges(vertex);

  for (const auto& edge : edges) { vertices.emplace_back(edge.to); }

  return vertices;
}

std::vector< GraphMatrix::Edge > GraphMatrix::getOutgoingEdges(
  Vertex vertex) const {
  std::vector< Edge > edges;

  for (const auto& edge : m_matrix) {
    if (m_isDirected) {
      if (edge.at(vertex) > 0) {
        for (size_t i = 0; i < edge.size(); ++i) {
          if (edge.at(i) < 0) {
            const auto weight = m_negativeEdge.at(&edge - &m_matrix.front()) ?
                                  edge.at(i) :
                                  -edge.at(i);
            edges.emplace_back(vertex, i, weight);
            break;
          }
        }
      }
    } else {
      if (edge.at(vertex) > 0) {
        for (size_t i = 0; i < edge.size(); ++i) {
          if (edge.at(i) > 0 && i != vertex) {
            const auto weight = m_negativeEdge.at(&edge - &m_matrix.front()) ?
                                  -edge.at(i) :
                                  edge.at(i);
            edges.emplace_back(vertex, i, weight);
            break;
          }
        }
      }
    }
  }

  return edges;
}

bool GraphMatrix::isDirected() const {
  return m_isDirected;
}

size_t GraphMatrix::getNumVertices() const {
  return m_vertexCount;
}

size_t GraphMatrix::getNumEdges() const {
  return m_matrix.size();
}

void GraphMatrix::clear() {
  m_matrix.clear();
  m_negativeEdge.clear();
}

void GraphMatrix::print() const {
  std::cout << "         |";

  size_t edgeIdx = 0;

  for (auto i = 0; i < getNumVertices(); ++i) {
    std::cout << fmt::format("v{:<5}|", i);
  }
  for (const auto& edge : m_matrix) {
    if (m_negativeEdge.at(edgeIdx)) {
      std::cout << fmt::format("\ne{:<5}(-)|", &edge - &m_matrix.front());
    } else {
      std::cout << fmt::format("\ne{:<5}(+)|", &edge - &m_matrix.front());
    }
    for (const auto& cost : edge) { std::cout << fmt::format("{:<6}|", cost); }
    ++edgeIdx;
  }
  std::cout << '\n';
}

} // namespace aizo::ds
