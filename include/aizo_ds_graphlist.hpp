#ifndef UNI_AIZO_P_Z2_AIZO_DS_GRAPHLIST_HPP
#define UNI_AIZO_P_Z2_AIZO_DS_GRAPHLIST_HPP

#include "aizo_ds_igraph.hpp"
#include <list>
#include <vector>

namespace aizo::ds {

class GraphList : public IGraph {
private:
  struct Node {
    Vertex vertex;
    Weight weight;
  };

public:
  explicit GraphList(GraphType type);
  GraphList(GraphList const&)            = default;
  GraphList(GraphList&&)                 = default;
  GraphList& operator=(GraphList const&) = default;
  GraphList& operator=(GraphList&&)      = default;
  ~GraphList() override                  = default;

  size_t addEdge(Edge edge) override;
  size_t addVertex() override;

  [[nodiscard]] std::vector< Vertex > getVertices() const override;
  [[nodiscard]] std::vector< Edge >   getEdges() const override;

  [[nodiscard]] std::vector< Vertex > getAdjacentVertices(
    Vertex vertex) const override;
  [[nodiscard]] std::vector< Edge > getOutgoingEdges(
    Vertex vertex) const override;

  [[nodiscard]] bool isDirected() const override;

  [[nodiscard]] size_t getNumVertices() const override;
  [[nodiscard]] size_t getNumEdges() const override;

  void clear() override;

  void print() const override;

private:
  std::vector< std::list< Node > > m_list;
  size_t                                   m_numEdges;
  bool                                     m_isDirected;
};

} // namespace aizo::ds

#endif // UNI_AIZO_P_Z2_AIZO_DS_GRAPHLIST_HPP
