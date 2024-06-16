#ifndef UNI_AIZO_P_Z2_AIZO_DS_IGRAPH_HPP
#define UNI_AIZO_P_Z2_AIZO_DS_IGRAPH_HPP

#include <cstdint>
#include <vector>

namespace aizo::ds {

class IGraph {
public:
  using Vertex = uint32_t;
  using Weight = int32_t;

  struct Edge {
    Vertex from;
    Vertex to;
    Weight weight;

    [[nodiscard]] auto operator<=>(Edge const& other) const {
      return weight <=> other.weight;
    }
  };

  enum class GraphType : uint8_t {
    Directed,
    Undirected
  };

  enum class GraphClass : uint8_t {
    List,
    Matrix
  };

  IGraph()                         = default;
  IGraph(IGraph const&)            = default;
  IGraph(IGraph&&)                 = default;
  IGraph& operator=(IGraph const&) = default;
  IGraph& operator=(IGraph&&)      = default;
  virtual ~IGraph()                = default;

  virtual size_t addEdge(Edge edge) = 0;
  virtual size_t addVertex()        = 0;

  [[nodiscard]] virtual std::vector< Vertex > getVertices() const = 0;
  [[nodiscard]] virtual std::vector< Edge >   getEdges() const    = 0;

  [[nodiscard]] virtual std::vector< Vertex > getAdjacentVertices(
    Vertex vertex) const = 0;
  [[nodiscard]] virtual std::vector< Edge > getOutgoingEdges(
    Vertex vertex) const = 0;

  [[nodiscard]] virtual bool isDirected() const = 0;

  [[nodiscard]] virtual size_t getNumVertices() const = 0;
  [[nodiscard]] virtual size_t getNumEdges() const    = 0;

  virtual void clear() = 0;

  virtual void print() const = 0;
};

} // namespace aizo::ds

#endif // UNI_AIZO_P_Z2_AIZO_DS_IGRAPH_HPP
