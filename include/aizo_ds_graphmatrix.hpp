#ifndef UNI_AIZO_P_Z2_AIZO_DS_GRAPHMATRIX_HPP
#define UNI_AIZO_P_Z2_AIZO_DS_GRAPHMATRIX_HPP

#include "aizo_ds_igraph.hpp"

namespace aizo::ds {

class GraphMatrix : public IGraph {
public:
  explicit GraphMatrix(GraphType type);
  GraphMatrix(GraphMatrix const&)            = default;
  GraphMatrix(GraphMatrix&&)                 = default;
  GraphMatrix& operator=(GraphMatrix const&) = default;
  GraphMatrix& operator=(GraphMatrix&&)      = default;
  ~GraphMatrix() override                    = default;

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
  std::vector< std::vector< Weight > >
                      m_matrix; // incidence matrix 1st vector e, 2nd vector v
  std::vector< bool > m_negativeEdge;
  size_t              m_vertexCount;
  bool                m_isDirected;
};

} // namespace aizo::ds

#endif // UNI_AIZO_P_Z2_AIZO_DS_GRAPHMATRIX_HPP
