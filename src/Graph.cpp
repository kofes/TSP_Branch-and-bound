#include "../inc/Graph.hpp"

Graph::Graph(int size) {
  graph.resize(size);
  for (int i = 0; i < size; ++i)
    graph[i].resize(size, INT_MAX);
}

void Graph::add_edge(int from, int to, int cost) {
  if (from >= graph.size() || to >= graph.size()) {
    int size = std::max(from+1, to+1);
    graph.resize(size);
    for (int i = 0; i < size; ++i)
      graph[i].resize(size, INT_MAX);
  }
  graph[from][to] = cost;
}

void Graph::print (const std::vector<std::vector<int> >& src) {
  for (int i = 0; i < src.size()+1; ++i)
    std::cout << i << '\t';
  std::cout << std::endl;
  for (int i = 0; i < src.size(); ++i) {
    std::cout << i+1 << '\t';
    for (int j = 0; j < src.size(); ++j)
      if (src[i][j] != INT_MAX)
        std::cout << src[i][j] << '\t';
      else
        std::cout << "-1\t";
    std::cout << std::endl;
  }
}

void Graph::printVector(const std::vector<int> &vec) {
  for (int i = 0; i < vec.size(); ++i)
    if (vec[i] != INT_MAX)
      std::cout << vec[i] << '\t';
    else
      std::cout << "-1\t";
  std::cout << std::endl;
}

std::vector<std::pair<int, int> > Graph::find_path() {
  std::vector< std::vector<int> > costs = graph;
  std::vector<int> drow(graph.size()), dcolumn(graph.size());
  std::vector<std::pair<int, int> > res;
  while (res.size() < graph.size()) {
    // print(costs);
    //calculate drow
    for (int i = 0; i < graph.size(); ++i) {
      drow[i] = INT_MAX;
      for (int j = 0; j < graph.size(); ++j)
        if (costs[i][j] < drow[i])
          drow[i] = costs[i][j];
    }
    // std::cout << "DROW:\n";
    // printVector(drow);
    //sub drow
    for (int i = 0; i < graph.size(); ++i)
      for (int j = 0; j < graph.size(); ++j)
        if (costs[i][j] < INT_MAX)
            costs[i][j] -= drow[i];
    //calculate dcolumn
    for (int j = 0; j < graph.size(); ++j) {
      dcolumn[j] = INT_MAX;
      for (int i = 0; i < graph.size(); ++i)
        if (costs[i][j] < dcolumn[j])
          dcolumn[j] = costs[i][j];
    }
    // std::cout << "DCOLUMN:\n";
    // printVector(dcolumn);
    //sub dcolumn
    for (int i = 0; i < graph.size(); ++i)
      for (int j = 0; j < graph.size(); ++j)
        if (costs[i][j] < INT_MAX)
            costs[i][j] -= dcolumn[j];
    // std::cout<< ">>>>>>\n";
    // print(costs);
    // std::cout<< "------\n";
    //fill zeroes choords
    Edge max_edge(0,0,-1);
    for (int i = 0; i < graph.size(); ++i)
      for (int j = 0; j < graph.size(); ++j)
        if (!costs[i][j]) {
          int cost = find_cost(i, j, costs);
          if (cost > max_edge.cost) {
            max_edge.from = i;
            max_edge.to = j;
            max_edge.cost = cost;
          }
        }
    //fill marked
    for (int i = 0; i < graph.size(); ++i)
      costs[max_edge.from][i] = costs[i][max_edge.to] = INT_MAX;
    costs[max_edge.to][max_edge.from] = INT_MAX;
    // std::cout << "max edge(" << max_edge.from << ", " << max_edge.to << "): " << max_edge.cost << std::endl;
    // std::cout<< "<<<<<<\n";
    res.push_back(std::make_pair(max_edge.from, max_edge.to));
  }
  return res;
}

int Graph::find_cost(int i, int j, const std::vector<std::vector<int> >& src) {
  int minRow = INT_MAX;
  int minColumn = INT_MAX;
  for(auto k = 0; k < src.size(); ++k) {
    if(src[i][k] < minRow && k != j)
        minRow = src[i][k];
    if(src[k][j] < minColumn && k != i)
        minColumn = src[k][j];
  }
  if(minRow == INT_MAX)
      minRow = 0;
  if(minColumn == INT_MAX)
      minColumn = 0;
  return minRow + minColumn;
}
