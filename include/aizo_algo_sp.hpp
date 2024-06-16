#ifndef UNI_AIZO_P_Z2_AIZO_ALGO_SP_HPP
#define UNI_AIZO_P_Z2_AIZO_ALGO_SP_HPP

#include "aizo_ds_igraph.hpp"
#include <optional>
#include <vector>
#include <list>

namespace aizo::algo {

class SP {
public:
  struct Solution {
    std::vector< std::list< ds::IGraph::Vertex > > m_path;
    std::vector< ds::IGraph::Weight >              m_weight;

    void print() const;
  };

  SP(ds::IGraph::Vertex source);

  void loadGraph(ds::IGraph const& graph);

  std::optional< Solution > dijkstra();
  std::optional< Solution > bellmanFord();

private:
  ds::IGraph::Vertex m_source;
  ds::IGraph const*  m_graph;
};

} // namespace aizo::algo

#endif // UNI_AIZO_P_Z2_AIZO_ALGO_SP_HPP
