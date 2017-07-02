#include <iostream>
#include <vector>
#include <climits>

#pragma once

struct Edge {
  Edge (int from, int to, int cost) : from(from), to(to), cost(cost) {};
  int from, to, cost;
};

class Graph {
public:
  Graph(int size = 0);
  void add_edge(int from, int to, int cost);
  std::vector< std::pair<int, int> > find_path();
  void print(const std::vector<std::vector<int> >& src);
  std::vector<int>& operator[] (size_t ind) {return graph[ind];}
private:
  void printVector(const std::vector<int>& vec);
  int find_cost(int i, int j, const std::vector<std::vector<int> >& src);
  std::vector< std::vector<int> > graph;
};
