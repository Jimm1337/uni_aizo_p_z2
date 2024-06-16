#include "aizo_tool_menu.hpp"
#include "aizo_tool_gengraph.hpp"
#include "aizo_tool_readergraph.hpp"
#include "aizo_algo_mst.hpp"
#include "aizo_algo_sp.hpp"
#include "aizo_tool_measure.hpp"
#include <fmt/core.h>
#include <iostream>

namespace aizo::tool {

void Menu::run() {
  printMenu();

  for (;;) {
    const auto input = readInput();

    switch (input) {
    case State::GenerateGraph:
      generateGraph();
      break;
    case State::ReadGraph_Matrix:
      readGraphMatrix();
      break;
    case State::ReadGraph_List:
      readGraphList();
      break;
    case State::DisplayGraphMatrix:
      displayGraphMatrix();
      break;
    case State::DisplayGraphList:
      displayGraphList();
      break;
    case State::RunMST_Kruskal:
      runMSTKruskal();
      break;
    case State::RunMST_Prim:
      runMSTPrim();
      break;
    case State::RunSP_Dijkstra:
      runSPDijkstra();
      break;
    case State::RunSP_BellmanFord:
      runSPBellmanFord();
      break;
    case State::Measure:
      measure();
      break;
    case State::Exit:
      return;
    default:
      break;
    }

    printMenu();
  }
}

void Menu::printMenu() {
  std::cout << "---Menu---\n";
  std::cout << "(0) - Generate Graph\n";
  std::cout << "(1) - Read Graph (Matrix)\n";
  std::cout << "(2) - Read Graph (List)\n";
  std::cout << "(3) - Display Graph (Matrix)\n";
  std::cout << "(4) - Display Graph (List)\n";
  std::cout << "(5) - Run MST (Kruskal)\n";
  std::cout << "(6) - Run MST (Prim)\n";
  std::cout << "(7) - Run SP (Dijkstra)\n";
  std::cout << "(8) - Run SP (Bellman-Ford)\n";
  std::cout << "(9) - Measure\n";
  std::cout << "(x) - Exit\n";
  std::cout << "----------\n";
  std::cout << std::flush;
}

Menu::State Menu::readInput() {
  char choice{};

  std::cout << "Enter choice: ";
  std::cin >> choice;
  switch (choice) {
  case '0':
    return State::GenerateGraph;
  case '1':
    return State::ReadGraph_Matrix;
  case '2':
    return State::ReadGraph_List;
  case '3':
    return State::DisplayGraphMatrix;
  case '4':
    return State::DisplayGraphList;
  case '5':
    return State::RunMST_Kruskal;
  case '6':
    return State::RunMST_Prim;
  case '7':
    return State::RunSP_Dijkstra;
  case '8':
    return State::RunSP_BellmanFord;
  case '9':
    return State::Measure;
  case 'x':
    return State::Exit;
  default:
    badChoice();
    return State::Start;
  }
}

void Menu::badChoice() {
  std::cout << "[W] Wrong input, try again.\n";
  std::cout << std::flush;
}

void Menu::generateGraph() {
  std::cout << "*** Graph Size ***\n";
  std::cout << std::flush;

  int64_t size{};
  while (size <= 10) {
    std::cout << "Enter size: ";
    std::cout << std::flush;
    std::cin >> size;

    if (size <= 10) {
      std::cout << "[W] Size must be greater than 10.\n";
      std::cout << std::flush;
    }
  }

  std::cout << "*** Graph Weight Range ***\n";
  std::cout << std::flush;

  ds::IGraph::Weight weightMin{};
  ds::IGraph::Weight weightMax{};
  while (weightMin >= weightMax) {
    std::cout << "Enter Minimum Weight: ";
    std::cout << std::flush;
    std::cin >> weightMin;

    std::cout << "Enter Maximum Weight: ";
    std::cout << std::flush;
    std::cin >> weightMax;

    if (weightMin >= weightMax) {
      std::cout << "[W] Min must be less than max.\n";
      std::cout << std::flush;
    }
  }

  std::cout << "*** Graph Density ***\n";
  std::cout << std::flush;

  char density{};
  while (density != '0' && density != '1' && density != '2') {
    std::cout << "(0) - 25% density\n";
    std::cout << "(1) - 50% density\n";
    std::cout << "(2) - 99% density\n";
    std::cout << std::flush;
    std::cout << "Enter choice: ";
    std::cout << std::flush;
    std::cin >> density;
  }

  std::cout << "*** Graph Type ***\n";
  std::cout << std::flush;

  char type{};
  while (type != '0' && type != '1') {
    std::cout << "(0) - Directed\n";
    std::cout << "(1) - Undirected\n";
    std::cout << std::flush;
    std::cout << "Enter choice: ";
    std::cout << std::flush;
    std::cin >> type;
  }

  GenGraph genGraph;
  switch (density) {
  case '0':
    genGraph.generate25DensityGraph(size,
                                    { weightMin, weightMax },
                                    type == '0' ?
                                      ds::IGraph::GraphType::Directed :
                                      ds::IGraph::GraphType::Undirected);
    break;
  case '1':
    genGraph.generate50DensityGraph(size,
                                    { weightMin, weightMax },
                                    type == '0' ?
                                      ds::IGraph::GraphType::Directed :
                                      ds::IGraph::GraphType::Undirected);
    break;
  case '2':
    genGraph.generate99DensityGraph(size,
                                    { weightMin, weightMax },
                                    type == '0' ?
                                      ds::IGraph::GraphType::Directed :
                                      ds::IGraph::GraphType::Undirected);
    break;
  default:
    break;
  }

  if (auto result = genGraph.lastAsMatrix()) {
    const auto graph = result.value();
    m_graphMatrix    = graph;
  }

  if (auto result = genGraph.lastAsList()) {
    const auto graph = result.value();
    m_graphList      = graph;
  }

  std::cout << "[I] Graph generated successfully.\n";
  std::cout << std::flush;
}

void Menu::readGraphMatrix() {
  std::string                                                     path{};
  std::optional< std::variant< ds::GraphMatrix, ds::GraphList > > graph;

  while (!graph.has_value()) {
    std::cout << "Enter path to file: ";
    std::cout << std::flush;
    std::cin >> path;

    ReaderGraph readerGraph;
    readerGraph.openFile(path, ReaderGraph::GraphRepresentation::Matrix);

    graph = readerGraph.readGraph();

    readerGraph.closeFile();

    if (!graph.has_value()) {
      std::cout << "[W] Error reading graph.\n";
      std::cout << std::flush;
    }
  }

  const auto matrix = std::get< ds::GraphMatrix >(graph.value());

  m_graphMatrix = matrix;

  const auto vertices = matrix.getVertices();
  const auto edges    = matrix.getEdges();

  m_graphList =
    ds::GraphList{ matrix.isDirected() ? ds::IGraph::GraphType::Directed :
                                         ds::IGraph::GraphType::Undirected };

  for (const auto& vertex : vertices) { m_graphList->addVertex(); }

  for (const auto& edge : edges) { m_graphList->addEdge(edge); }

  std::cout << "[I] Graph read successfully.\n";
  std::cout << std::flush;
}

void Menu::readGraphList() {
  std::string                                                     path{};
  std::optional< std::variant< ds::GraphMatrix, ds::GraphList > > graph;

  while (!graph.has_value()) {
    std::cout << std::flush;
    std::cin >> path;

    ReaderGraph readerGraph;
    readerGraph.openFile(path, ReaderGraph::GraphRepresentation::List);

    graph = readerGraph.readGraph();

    readerGraph.closeFile();

    if (!graph.has_value()) {
      std::cout << "[W] Error reading graph.\n";
      std::cout << std::flush;
    }
  }

  const auto list = std::get< ds::GraphList >(graph.value());

  m_graphList = list;

  const auto vertices = list.getVertices();
  const auto edges    = list.getEdges();

  m_graphMatrix =
    ds::GraphMatrix{ list.isDirected() ? ds::IGraph::GraphType::Directed :
                                         ds::IGraph::GraphType::Undirected };

  for (const auto& vertex : vertices) { m_graphMatrix->addVertex(); }

  for (const auto& edge : edges) { m_graphMatrix->addEdge(edge); }

  std::cout << "[I] Graph read successfully.\n";
  std::cout << std::flush;
}

void Menu::displayGraphMatrix() {
  if (m_graphMatrix.has_value()) {
    m_graphMatrix->print();
  } else {
    std::cout << "No graph available.\n";
    std::cout << std::flush;
  }
}

void Menu::displayGraphList() {
  if (m_graphList.has_value()) {
    m_graphList->print();
  } else {
    std::cout << "No graph available.\n";
    std::cout << std::flush;
  }
}

void Menu::runMSTKruskal() {
  std::cout << "(0) List representation\n";
  std::cout << "(1) Matrix representation\n";
  std::cout << std::flush;

  char choice{};
  while (choice != '0' && choice != '1') { std::cin >> choice; }

  algo::MST mst{};
  if (choice == '0') {
    if (m_graphList.has_value()) {
      mst.loadGraph(m_graphList.value());
      const auto kruskal = mst.kruskal();
      if (kruskal.has_value()) {
        kruskal.value().print();
        std::cout << std::flush;
      } else {
        std::cout << "Error computing Kruskal's algorithm.\n";
        std::cout << std::flush;
      }
    } else {
      std::cout << "No graph available.\n";
      std::cout << std::flush;
    }
  } else {
    if (m_graphMatrix.has_value()) {
      mst.loadGraph(m_graphMatrix.value());
      const auto kruskal = mst.kruskal();
      if (kruskal.has_value()) {
        kruskal.value().print();
        std::cout << std::flush;
      } else {
        std::cout << "Error computing Kruskal's algorithm.\n";
        std::cout << std::flush;
      }
    } else {
      std::cout << "No graph available.\n";
      std::cout << std::flush;
    }
  }
}

void Menu::runMSTPrim() {
  std::cout << "(0) List representation\n";
  std::cout << "(1) Matrix representation\n";
  std::cout << std::flush;

  char choice{};
  while (choice != '0' && choice != '1') {
    std::cout << "Enter choice: ";
    std::cout << std::flush;
    std::cin >> choice;
  }

  algo::MST mst{};
  if (choice == '0') {
    if (m_graphList.has_value()) {
      mst.loadGraph(m_graphList.value());
      const auto prim = mst.prim();
      if (prim.has_value()) {
        prim.value().print();
        std::cout << std::flush;
      } else {
        std::cout << "Error computing Prim's algorithm.\n";
        std::cout << std::flush;
      }
    } else {
      std::cout << "No graph available.\n";
      std::cout << std::flush;
    }
  } else {
    if (m_graphMatrix.has_value()) {
      mst.loadGraph(m_graphMatrix.value());
      const auto prim = mst.prim();
      if (prim.has_value()) {
        prim.value().print();
        std::cout << std::flush;
      } else {
        std::cout << "Error computing Prim's algorithm.\n";
        std::cout << std::flush;
      }
    } else {
      std::cout << "No graph available.\n";
      std::cout << std::flush;
    }
  }
}

void Menu::runSPDijkstra() {
  std::cout << "(0) List representation\n";
  std::cout << "(1) Matrix representation\n";
  std::cout << std::flush;

  char choice{};
  while (choice != '0' && choice != '1') {
    std::cout << "Enter choice: ";
    std::cout << std::flush;
    std::cin >> choice;
  }

  int source{ -1 };
  while (source < 0) {
    std::cout << "Enter source vertex: ";
    std::cout << std::flush;
    std::cin >> source;
  }

  algo::SP sp{ static_cast< ds::IGraph::Vertex >(source) };

  if (choice == '0') {
    if (m_graphList.has_value()) {
      sp.loadGraph(m_graphList.value());
      const auto dijkstra = sp.dijkstra();
      if (dijkstra.has_value()) {
        dijkstra.value().print();
        std::cout << std::flush;
      } else {
        std::cout << "Error computing Dijkstra's algorithm.\n";
        std::cout << std::flush;
      }
    } else {
      std::cout << "No graph available.\n";
      std::cout << std::flush;
    }
  } else {
    if (m_graphMatrix.has_value()) {
      sp.loadGraph(m_graphMatrix.value());
      const auto dijkstra = sp.dijkstra();
      if (dijkstra.has_value()) {
        dijkstra.value().print();
        std::cout << std::flush;
      } else {
        std::cout << "Error computing Dijkstra's algorithm.\n";
        std::cout << std::flush;
      }
    } else {
      std::cout << "No graph available.\n";
      std::cout << std::flush;
    }
  }
}

void Menu::runSPBellmanFord() {
  std::cout << "(0) List representation\n";
  std::cout << "(1) Matrix representation\n";
  std::cout << std::flush;

  char choice{};
  while (choice != '0' && choice != '1') {
    std::cout << "Enter choice: ";
    std::cout << std::flush;
    std::cin >> choice;
  }

  int source{ -1 };
  while (source < 0) {
    std::cout << "Enter source vertex: ";
    std::cout << std::flush;
    std::cin >> source;
  }

  algo::SP sp{ static_cast< ds::IGraph::Vertex >(source) };

  if (choice == '0') {
    if (m_graphList.has_value()) {
      sp.loadGraph(m_graphList.value());
      const auto bellmanFord = sp.bellmanFord();
      if (bellmanFord.has_value()) {
        bellmanFord.value().print();
        std::cout << std::flush;
      } else {
        std::cout << "Error computing Bellman-Ford's algorithm.\n";
        std::cout << std::flush;
      }
    } else {
      std::cout << "No graph available.\n";
      std::cout << std::flush;
    }
  } else {
    if (m_graphMatrix.has_value()) {
      sp.loadGraph(m_graphMatrix.value());
      const auto bellmanFord = sp.bellmanFord();
      if (bellmanFord.has_value()) {
        bellmanFord.value().print();
        std::cout << std::flush;
      } else {
        std::cout << "Error computing Bellman-Ford's algorithm.\n";
        std::cout << std::flush;
      }
    } else {
      std::cout << "No graph available.\n";
      std::cout << std::flush;
    }
  }
}

void Menu::measure() {
  measure::mst();
  measure::sp();
}

} // namespace aizo::tool
