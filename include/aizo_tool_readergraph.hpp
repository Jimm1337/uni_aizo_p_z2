#ifndef UNI_AIZO_P_Z2_AIZO_TOOL_READERGRAPH_HPP
#define UNI_AIZO_P_Z2_AIZO_TOOL_READERGRAPH_HPP

#include "aizo_ds_graphmatrix.hpp"
#include "aizo_ds_graphlist.hpp"
#include <string_view>
#include <fstream>
#include <variant>
#include <optional>

namespace aizo::tool {

class ReaderGraph {
public:
  enum class GraphRepresentation : uint8_t {
    Matrix,
    List,
    None
  };

  void openFile(std::string_view filename, GraphRepresentation representation);
  void closeFile();

  std::optional< std::variant< ds::GraphMatrix, ds::GraphList > > readGraph();

private:
  std::optional< std::variant< ds::GraphMatrix, ds::GraphList > > readHeader();
  std::optional<ds::GraphMatrix> readMatrix();
  std::optional<ds::GraphList> readList();

  std::ifstream       m_file;
  GraphRepresentation m_representation{ GraphRepresentation::None };
};

} // namespace aizo::tool

#endif // UNI_AIZO_P_Z2_AIZO_TOOL_READERGRAPH_HPP
