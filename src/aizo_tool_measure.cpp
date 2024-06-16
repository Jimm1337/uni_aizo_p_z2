#include "aizo_tool_measure.hpp"
#include "aizo_tool_timer.hpp"
#include "aizo_algo_sp.hpp"
#include "aizo_algo_mst.hpp"
#include "aizo_tool_gengraph.hpp"
#include <fmt/core.h>
#include <fstream>
#include <iostream>
#include <optional>

namespace aizo::tool::measure {

void mst() {
  aizo::algo::MST mst{};

  Timer timerKruskal{ [&mst] { mst.kruskal(); } };
  Timer timerPrim{ [&mst] { mst.prim(); } };

  GenGraph genGraph{};

  const auto sampleSize = 25;

  std::optional< ds::GraphMatrix > graphMatrix;
  std::optional< ds::GraphList >   graphList;

  std::ofstream file{ "mst.csv" };

  file << "Representation;Algorithm;Vertices;Density;Time(ms)\n";

  for (const auto vCount : { 200, 250, 300, 350, 400 }) {
    std::cout << "MST - " << vCount << '\n';

    double timeKruskalM25 = 0.0;
    double timeKruskalM50 = 0.0;
    double timeKruskalM99 = 0.0;
    double timePrimM25    = 0.0;
    double timePrimM50    = 0.0;
    double timePrimM99    = 0.0;

    double timeKruskalL25 = 0.0;
    double timeKruskalL50 = 0.0;
    double timeKruskalL99 = 0.0;
    double timePrimL25    = 0.0;
    double timePrimL50    = 0.0;
    double timePrimL99    = 0.0;

    for (auto i = 0; i < sampleSize; ++i) {
      genGraph.generate25DensityGraph(
        vCount, { 1, 10000 }, ds::IGraph::GraphType::Undirected);
      graphMatrix = genGraph.lastAsMatrix();
      graphList   = genGraph.lastAsList();
      mst.loadGraph(*graphMatrix);
      timerKruskal();
      timerPrim();
      timeKruskalM25 += timerKruskal.getDuration().first;
      timePrimM25 += timerPrim.getDuration().first;
      mst.loadGraph(*graphList);
      timerKruskal();
      timerPrim();
      timeKruskalL25 += timerKruskal.getDuration().first;
      timePrimL25 += timerPrim.getDuration().first;

      genGraph.generate50DensityGraph(
        vCount, { 1, 10000 }, ds::IGraph::GraphType::Undirected);
      graphMatrix = genGraph.lastAsMatrix();
      graphList   = genGraph.lastAsList();
      mst.loadGraph(*graphMatrix);
      timerKruskal();
      timerPrim();
      timeKruskalM50 += timerKruskal.getDuration().first;
      timePrimM50 += timerPrim.getDuration().first;
      mst.loadGraph(*graphList);
      timerKruskal();
      timerPrim();
      timeKruskalL50 += timerKruskal.getDuration().first;
      timePrimL50 += timerPrim.getDuration().first;

      genGraph.generate99DensityGraph(
        vCount, { 1, 10000 }, ds::IGraph::GraphType::Undirected);
      graphMatrix = genGraph.lastAsMatrix();
      graphList   = genGraph.lastAsList();
      mst.loadGraph(*graphMatrix);
      timerKruskal();
      timerPrim();
      timeKruskalM99 += timerKruskal.getDuration().first;
      timePrimM99 += timerPrim.getDuration().first;
      mst.loadGraph(*graphList);
      timerKruskal();
      timerPrim();
      timeKruskalL99 += timerKruskal.getDuration().first;
      timePrimL99 += timerPrim.getDuration().first;
    }

    file << "Matrix;Kruskal;" << vCount << ";25;"
         << fmt::format("{:.2f}", timeKruskalM25 / sampleSize) << '\n';
    file << "Matrix;Kruskal;" << vCount << ";50;"
         << fmt::format("{:.2f}", timeKruskalM50 / sampleSize) << '\n';
    file << "Matrix;Kruskal;" << vCount << ";99;"
         << fmt::format("{:.2f}", timeKruskalM99 / sampleSize) << '\n';
    file << "Matrix;Prim;" << vCount << ";25;"
         << fmt::format("{:.2f}", timePrimM25 / sampleSize) << '\n';
    file << "Matrix;Prim;" << vCount << ";50;"
         << fmt::format("{:.2f}", timePrimM50 / sampleSize) << '\n';
    file << "Matrix;Prim;" << vCount << ";99;"
         << fmt::format("{:.2f}", timePrimM99 / sampleSize) << '\n';
    file << "List;Kruskal;" << vCount << ";25;"
         << fmt::format("{:.2f}", timeKruskalL25 / sampleSize) << '\n';
    file << "List;Kruskal;" << vCount << ";50;"
         << fmt::format("{:.2f}", timeKruskalL50 / sampleSize) << '\n';
    file << "List;Kruskal;" << vCount << ";99;"
         << fmt::format("{:.2f}", timeKruskalL99 / sampleSize) << '\n';
    file << "List;Prim;" << vCount << ";25;"
         << fmt::format("{:.2f}", timePrimL25 / sampleSize) << '\n';
    file << "List;Prim;" << vCount << ";50;"
         << fmt::format("{:.2f}", timePrimL50 / sampleSize) << '\n';
    file << "List;Prim;" << vCount << ";99;"
         << fmt::format("{:.2f}", timePrimL99 / sampleSize) << '\n';
  }
}

void sp() {
  aizo::algo::SP sp{ 0 };

  Timer timerDijkstra{ [&sp] { sp.dijkstra(); } };
  Timer timerBellmanFord{ [&sp] { sp.bellmanFord(); } };

  GenGraph genGraph{};

  const auto sampleSize = 25;

  std::optional< ds::GraphMatrix > graphMatrix;
  std::optional< ds::GraphList >   graphList;

  std::ofstream file{ "sp.csv" };

  file << "Representation;Algorithm;Vertices;Density;Time(ms)\n";

  for (const auto vCount : { 100, 125, 150, 175, 200 }) {
    std::cout << "SP - " << vCount << '\n';

    double timeDijkstraM25    = 0.0;
    double timeDijkstraM50    = 0.0;
    double timeDijkstraM99    = 0.0;
    double timeBellmanFordM25 = 0.0;
    double timeBellmanFordM50 = 0.0;
    double timeBellmanFordM99 = 0.0;

    double timeDijkstraL25    = 0.0;
    double timeDijkstraL50    = 0.0;
    double timeDijkstraL99    = 0.0;
    double timeBellmanFordL25 = 0.0;
    double timeBellmanFordL50 = 0.0;
    double timeBellmanFordL99 = 0.0;

    for (auto i = 0; i < sampleSize; ++i) {
      genGraph.generate25DensityGraph(
        vCount, { 1, 10000 }, ds::IGraph::GraphType::Directed);
      graphMatrix = genGraph.lastAsMatrix();
      graphList   = genGraph.lastAsList();
      sp.loadGraph(*graphMatrix);
      timerDijkstra();
      timerBellmanFord();
      timeDijkstraM25 += timerDijkstra.getDuration().first;
      timeBellmanFordM25 += timerBellmanFord.getDuration().first;
      sp.loadGraph(*graphList);
      timerDijkstra();
      timerBellmanFord();
      timeDijkstraL25 += timerDijkstra.getDuration().first;
      timeBellmanFordL25 += timerBellmanFord.getDuration().first;

      genGraph.generate50DensityGraph(
        vCount, { 1, 10000 }, ds::IGraph::GraphType::Directed);
      graphMatrix = genGraph.lastAsMatrix();
      graphList   = genGraph.lastAsList();
      sp.loadGraph(*graphMatrix);
      timerDijkstra();
      timerBellmanFord();
      timeDijkstraM50 += timerDijkstra.getDuration().first;
      timeBellmanFordM50 += timerBellmanFord.getDuration().first;
      sp.loadGraph(*graphList);
      timerDijkstra();
      timerBellmanFord();
      timeDijkstraL50 += timerDijkstra.getDuration().first;
      timeBellmanFordL50 += timerBellmanFord.getDuration().first;

      genGraph.generate99DensityGraph(
        vCount, { 1, 10000 }, ds::IGraph::GraphType::Directed);
      graphMatrix = genGraph.lastAsMatrix();
      graphList   = genGraph.lastAsList();
      sp.loadGraph(*graphMatrix);
      timerDijkstra();
      timerBellmanFord();
      timeDijkstraM99 += timerDijkstra.getDuration().first;
      timeBellmanFordM99 += timerBellmanFord.getDuration().first;
      sp.loadGraph(*graphList);
      timerDijkstra();
      timerBellmanFord();
      timeDijkstraL99 += timerDijkstra.getDuration().first;
      timeBellmanFordL99 += timerBellmanFord.getDuration().first;
    }

    file << "Matrix;Dijkstra;" << vCount << ";25;"
         << fmt::format("{:.2f}", timeDijkstraM25 / sampleSize) << '\n';
    file << "Matrix;Dijkstra;" << vCount << ";50;"
         << fmt::format("{:.2f}", timeDijkstraM50 / sampleSize) << '\n';
    file << "Matrix;Dijkstra;" << vCount << ";99;"
         << fmt::format("{:.2f}", timeDijkstraM99 / sampleSize) << '\n';
    file << "Matrix;BellmanFord;" << vCount << ";25;"
         << fmt::format("{:.2f}", timeBellmanFordM25 / sampleSize) << '\n';
    file << "Matrix;BellmanFord;" << vCount << ";50;"
         << fmt::format("{:.2f}", timeBellmanFordM50 / sampleSize) << '\n';
    file << "Matrix;BellmanFord;" << vCount << ";99;"
         << fmt::format("{:.2f}", timeBellmanFordM99 / sampleSize) << '\n';
    file << "List;Dijkstra;" << vCount << ";25;"
         << fmt::format("{:.2f}", timeDijkstraL25 / sampleSize) << '\n';
    file << "List;Dijkstra;" << vCount << ";50;"
         << fmt::format("{:.2f}", timeDijkstraL50 / sampleSize) << '\n';
    file << "List;Dijkstra;" << vCount << ";99;"
         << fmt::format("{:.2f}", timeDijkstraL99 / sampleSize) << '\n';
    file << "List;BellmanFord;" << vCount << ";25;"
         << fmt::format("{:.2f}", timeBellmanFordL25 / sampleSize) << '\n';
    file << "List;BellmanFord;" << vCount << ";50;"
         << fmt::format("{:.2f}", timeBellmanFordL50 / sampleSize) << '\n';
    file << "List;BellmanFord;" << vCount << ";99;"
         << fmt::format("{:.2f}", timeBellmanFordL99 / sampleSize) << '\n';
  }
}

} // namespace aizo::tool::measure