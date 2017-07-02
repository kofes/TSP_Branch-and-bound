#include "inc/Graph.hpp"
#include <fstream>

int main() {
  size_t count;
  std::ifstream fin("input0.txt");
  std::ofstream fout("output.txt");
  fin >> count;
  Graph graph(count);
  for (int i = 0; i < count; ++i)
    for (int j = 0; j < count; ++j) {
      int cost;
      fin >> cost;
      if (cost == -1) cost = INT_MAX;
      graph.add_edge(i, j, cost);
    }
  auto res = graph.find_path();
  int cost = 0;
  for (auto elem : res) {
    cost += graph[elem.first][elem.second];
    fout << elem.first+1 << ' ' << elem.second+1 << std::endl;
  }
  fout << "COST: " << cost << std::endl;
  return 0;
}
