#include "aizo_tool_gengraph.hpp"

namespace aizo::tool {

void GenGraph::generate25DensityGraph(size_t                numVertices,
                                      WeightRange           weightRange,
                                      ds::IGraph::GraphType type) {
  if (numVertices < 2) { return; }

  std::random_device device;
  Generator          gen(device());
  Distribution       weightDist(weightRange.min, weightRange.max);

  addVertices(numVertices, type);
  genSpanningTree(numVertices, weightDist, gen);

  auto numEdges = m_graphMatrix->getNumEdges();

  double density = getDensity(type, numVertices, numEdges);

  while (density < 0.25) {
    const auto vertex1 = static_cast< ds::IGraph::Vertex >(gen() % numVertices);
    const auto vertex2 = static_cast< ds::IGraph::Vertex >(gen() % numVertices);

    if (vertex1 == vertex2) { continue; }

    const auto weight = weightDist(gen);

    const auto edge = ds::IGraph::Edge{ vertex1, vertex2, weight };

    m_graphMatrix->addEdge(edge);
    m_graphList->addEdge(edge);

    numEdges = m_graphMatrix->getNumEdges();

    density = getDensity(type, numVertices, numEdges);
  }
}

void GenGraph::generate50DensityGraph(size_t                numVertices,
                                      WeightRange           weightRange,
                                      ds::IGraph::GraphType type) {
  if (numVertices < 2) { return; }

  std::random_device device;
  Generator          gen(device());
  Distribution       weightDist(weightRange.min, weightRange.max);

  addVertices(numVertices, type);
  genSpanningTree(numVertices, weightDist, gen);

  auto numEdges = m_graphMatrix->getNumEdges();

  double density = getDensity(type, numVertices, numEdges);

  while (density < 0.50) {
    const auto vertex1 = static_cast< ds::IGraph::Vertex >(gen() % numVertices);
    const auto vertex2 = static_cast< ds::IGraph::Vertex >(gen() % numVertices);

    if (vertex1 == vertex2) { continue; }

    const auto weight = weightDist(gen);

    const auto edge = ds::IGraph::Edge{ vertex1, vertex2, weight };

    m_graphMatrix->addEdge(edge);
    m_graphList->addEdge(edge);

    numEdges = m_graphMatrix->getNumEdges();

    density = getDensity(type, numVertices, numEdges);
  }
}

void GenGraph::generate99DensityGraph(size_t                numVertices,
                                      WeightRange           weightRange,
                                      ds::IGraph::GraphType type) {
  if (numVertices < 2) { return; }

  std::random_device device;
  Generator          gen(device());
  Distribution       weightDist(weightRange.min, weightRange.max);

  addVertices(numVertices, type);
  genSpanningTree(numVertices, weightDist, gen);

  auto numEdges = m_graphMatrix->getNumEdges();

  double density = getDensity(type, numVertices, numEdges);

  while (density < 0.99) {
    const auto vertex1 = static_cast< ds::IGraph::Vertex >(gen() % numVertices);
    const auto vertex2 = static_cast< ds::IGraph::Vertex >(gen() % numVertices);

    if (vertex1 == vertex2) { continue; }

    const auto weight = weightDist(gen);

    const auto edge = ds::IGraph::Edge{ vertex1, vertex2, weight };

    m_graphMatrix->addEdge(edge);
    m_graphList->addEdge(edge);

    numEdges = m_graphMatrix->getNumEdges();

    density = getDensity(type, numVertices, numEdges);
  }
}

std::optional< ds::GraphMatrix > GenGraph::lastAsMatrix() const {
  return m_graphMatrix;
}

std::optional< ds::GraphList > GenGraph::lastAsList() const {
  return m_graphList;
}

void GenGraph::addVertices(size_t numVertices, ds::IGraph::GraphType type) {
  m_graphMatrix = ds::GraphMatrix(type);
  m_graphList   = ds::GraphList(type);

  for (size_t i = 0; i < numVertices; ++i) {
    m_graphMatrix->addVertex();
    m_graphList->addVertex();
  }
}

void GenGraph::genSpanningTree(size_t        numVertices,
                               Distribution& weightDist,
                               Generator&    gen) {
  for (size_t i = 1; i < numVertices; ++i) {
    const auto edge =
      ds::IGraph::Edge{ static_cast< ds::IGraph::Vertex >(i - 1),
                        static_cast< ds::IGraph::Vertex >(i),
                        weightDist(gen) };

    m_graphMatrix->addEdge(edge);
    m_graphList->addEdge(edge);
  }
}

} // namespace aizo::tool
