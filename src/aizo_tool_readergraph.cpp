#include "aizo_tool_readergraph.hpp"
#include <string>
#include <sstream>

namespace aizo::tool {

void ReaderGraph::openFile(std::string_view                 filename,
                           ReaderGraph::GraphRepresentation representation) {
  m_file.open(filename.data());
  m_representation = representation;
}

void ReaderGraph::closeFile() {
  if (m_file.is_open()) { m_file.close(); }
  m_representation = GraphRepresentation::None;
}

std::optional< std::variant< ds::GraphMatrix, ds::GraphList > >
ReaderGraph::readGraph() {
  switch (m_representation) {
  case GraphRepresentation::Matrix:
    return readMatrix();
  case GraphRepresentation::List:
    return readList();
  default:
    return std::nullopt;
  }
}

std::optional< std::variant< ds::GraphMatrix, ds::GraphList > >
ReaderGraph::readHeader() {
  size_t                vertexCount{};
  ds::IGraph::GraphType type{};

  m_file >> vertexCount;
  if (m_file.fail()) { return std::nullopt; }

  char typeChar{};
  m_file >> typeChar;
  if (m_file.fail()) { return std::nullopt; }

  if (typeChar == 'D') {
    type = ds::IGraph::GraphType::Directed;
  } else if (typeChar == 'U') {
    type = ds::IGraph::GraphType::Undirected;
  } else {
    return std::nullopt;
  }

  if (m_representation == GraphRepresentation::Matrix) {
    auto matrix = ds::GraphMatrix{ type };
    for (size_t i = 0; i < vertexCount; ++i) { matrix.addVertex(); }
    return matrix;
  } else if (m_representation == GraphRepresentation::List) {
    auto list = ds::GraphList{ type };
    for (size_t i = 0; i < vertexCount; ++i) { list.addVertex(); }
    return list;
  }

  return std::nullopt;
}

std::optional< ds::GraphMatrix > ReaderGraph::readMatrix() {
  auto header = readHeader();
  if (!header.has_value()) { return std::nullopt; }

  auto matrix = std::get< ds::GraphMatrix >(header.value());

  std::string line;
  std::getline(m_file, line); // skip the first line (vertices count)
  while (std::getline(m_file, line)) {
    std::istringstream edgeText(line);

    ds::IGraph::Edge edge{ std::numeric_limits< ds::IGraph::Vertex >::max(),
                           std::numeric_limits< ds::IGraph::Vertex >::max(),
                           0 };

    char edgeChar{};
    bool negativeEdge{ false };
    edgeText >> edgeChar;
    if (edgeChar == '-') { negativeEdge = true; }

    ds::IGraph::Weight weight{};

    for (size_t vertex = 0; !edgeText.eof(); ++vertex) {
      edgeText >> weight;

      if (matrix.isDirected()) {
        if (weight > 0) {
          edge.from   = vertex;
          edge.weight = negativeEdge ? -weight : weight;
        } else if (weight < 0) {
          edge.to = vertex;
        }
      } else {
        if (weight > 0) {
          if (edge.from == std::numeric_limits< ds::IGraph::Vertex >::max()) {
            edge.from   = vertex;
            edge.weight = negativeEdge ? -weight : weight;
          } else if (edge.to ==
                     std::numeric_limits< ds::IGraph::Vertex >::max()) {
            edge.to = vertex;
          }
        }
      }
    }

    matrix.addEdge(edge);
  }

  return matrix;
}

std::optional< ds::GraphList > ReaderGraph::readList() {
  auto header = readHeader();
  if (!header.has_value()) { return std::nullopt; }

  auto list = std::get< ds::GraphList >(header.value());

  std::string line;
  std::getline(m_file, line); // skip the first line (vertices count
  for (size_t vertex = 0; std::getline(m_file, line); ++vertex) {
    std::istringstream edgeText(line);

    ds::IGraph::Edge edge{ std::numeric_limits< ds::IGraph::Vertex >::max(),
                           std::numeric_limits< ds::IGraph::Vertex >::max(),
                           0 };

    std::string edgeStr;

    while (!edgeText.eof()) {
      edgeText >> edgeStr;

      size_t posLeft  = edgeStr.find_first_of('(');
      size_t posRight = edgeStr.find_first_of(')');

      auto destStr   = edgeStr.substr(0, posLeft);
      auto weightStr = edgeStr.substr(posLeft + 1, posRight - posLeft - 1);

      edge.to     = std::stoul(destStr);
      edge.weight = std::stol(weightStr);
      edge.from   = vertex;

      list.addEdge(edge);
    }
  }

  return list;
}

} // namespace aizo::tool
