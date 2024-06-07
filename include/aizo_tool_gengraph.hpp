#ifndef UNI_AIZO_P_Z2_AIZO_TOOL_GENGRAPH_HPP
#define UNI_AIZO_P_Z2_AIZO_TOOL_GENGRAPH_HPP

#include "aizo_ds_graphmatrix.hpp"
#include "aizo_ds_graphlist.hpp"
#include <optional>
#include <random>

namespace aizo::tool {

class GenGraph {
public:
  using Distribution = std::uniform_int_distribution< ds::IGraph::Weight >;
  using Generator    = std::mt19937;

  struct WeightRange {
    ds::IGraph::Weight min;
    ds::IGraph::Weight max;
  };

  void generate25DensityGraph(size_t                numVertices,
                              WeightRange           weightRange,
                              ds::IGraph::GraphType type);
  void generate50DensityGraph(size_t                numVertices,
                              WeightRange           weightRange,
                              ds::IGraph::GraphType type);
  void generate99DensityGraph(size_t                numVertices,
                              WeightRange           weightRange,
                              ds::IGraph::GraphType type);

  [[nodiscard]] std::optional< ds::GraphMatrix > lastAsMatrix() const;
  [[nodiscard]] std::optional< ds::GraphList >   lastAsList() const;

private:
  void addVertices(size_t numVertices, ds::IGraph::GraphType type);
  void genSpanningTree(size_t        numVertices,
                       Distribution& weightDist,
                       Generator&    gen);
  static constexpr double getDensity(ds::IGraph::GraphType type,
                                     size_t                numVertices,
                                     size_t                numEdges) {
    return type == ds::IGraph::GraphType::Directed ?
             static_cast<double>(numEdges) / static_cast<double>(numVertices * (numVertices - 1)) :
             static_cast<double>(2 * numEdges) / static_cast<double>(numVertices * (numVertices - 1));
  }

  std::optional< ds::GraphMatrix > m_graphMatrix;
  std::optional< ds::GraphList >   m_graphList;
};

} // namespace aizo::tool

#endif // UNI_AIZO_P_Z2_AIZO_TOOL_GENGRAPH_HPP
