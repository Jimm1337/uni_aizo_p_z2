#ifndef UNI_AIZO_P_Z2_AIZO_TOOL_MENU_HPP
#define UNI_AIZO_P_Z2_AIZO_TOOL_MENU_HPP

#include "aizo_ds_graphmatrix.hpp"
#include "aizo_ds_graphlist.hpp"
#include <cstdint>
#include <optional>

namespace aizo::tool {

class Menu {
public:
  enum class State : int8_t {
    Exit               = -2,
    Start              = -1,
    GenerateGraph      = 0,
    ReadGraph_Matrix   = 1,
    ReadGraph_List     = 2,
    DisplayGraphMatrix = 3,
    DisplayGraphList   = 4,
    RunMST_Kruskal     = 5,
    RunMST_Prim        = 6,
    RunSP_Dijkstra     = 7,
    RunSP_BellmanFord  = 8,
    Measure            = 9
  };

  void run();

private:
  static void printMenu();
  static State readInput();
  static void badChoice();

  void generateGraph();
  void readGraphMatrix();
  void readGraphList();
  void displayGraphMatrix();
  void displayGraphList();
  void runMSTKruskal();
  void runMSTPrim();
  void runSPDijkstra();
  void runSPBellmanFord();
  void measure();

  State                            m_state = State::Start;
  std::optional< ds::GraphMatrix > m_graphMatrix;
  std::optional< ds::GraphList >   m_graphList;
};

} // namespace aizo::tool

#endif // UNI_AIZO_P_Z2_AIZO_TOOL_MENU_HPP
