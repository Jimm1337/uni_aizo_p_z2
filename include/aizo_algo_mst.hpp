#ifndef UNI_AIZO_P_Z2_AIZO_ALGO_MST_HPP
#define UNI_AIZO_P_Z2_AIZO_ALGO_MST_HPP

#include "aizo_ds_igraph.hpp"
#include <vector>
#include <optional>

namespace aizo::algo {

class MST {
private:
  struct Solution {
    std::vector< ds::IGraph::Edge > m_edges;
    ds::IGraph::Weight              m_weight{};

    void print() const;
  };

  struct DisjointSet {
    std::vector< ds::IGraph::Vertex > parent;
    std::vector< ds::IGraph::Vertex > rank;

    explicit DisjointSet(size_t size);

    ds::IGraph::Vertex find(ds::IGraph::Vertex v);
    void merge(ds::IGraph::Vertex u, ds::IGraph::Vertex v) ;
  };

public:
  void loadGraph(ds::IGraph const& graph);

  std::optional< Solution > kruskal();
  std::optional< Solution > prim();

private:
  ds::IGraph const* m_graph;
};

} // namespace aizo::algo

#endif // UNI_AIZO_P_Z2_AIZO_ALGO_MST_HPP
