#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <cmath>
#include "qsopt.h"

static std::vector< std::vector<double> > matrix;
size_t size;
const double max_value = 1e+7;
const double eps = 1e-4;

class TSP {
public:
  TSP();
  ~TSP() {QSfree(qSprob);}
  void solve();
  double get_min() { return min; }
  void print();
private:
  QSprob qSprob;
  size_t ncols;
  size_t nrows;
  double min = max_value;
};

TSP::TSP() {
  ncols = size*size;
  nrows = 2*size;
  std::vector<int> cmatcnt(ncols, 2);
  std::vector<int> cmatbeg(ncols);
  for (size_t i = 0; i < ncols; ++i)
    cmatbeg[i] = 2*i;
  std::vector<int> cmatind;
  for (size_t i = 0; i < ncols; ++i)
    for (size_t j = 0; j < size; ++j) {
      cmatind.push_back(i);
      cmatind.push_back(size+j);
    }
  std::vector<double> cmatval(ncols*nrows, 1.0);
  std::vector<double> obj;
  for (size_t i = 0; i < size; ++i)
    for (size_t j = 0; j < size; ++j)
      obj.push_back(matrix[i][j]);
  std::vector<double> rhs(nrows, 1.0);
  std::vector<char> sense(nrows, 'E');
  std::vector<double> lower(ncols, 0.0);
  std::vector<double> upper(ncols, 1.0);
  for (size_t i = 0; i < size; ++i)
    upper[i*size+i] = 0;
  qSprob = QSload_prob("TSP", ncols, nrows, cmatcnt.data(), cmatbeg.data(),
                       cmatind.data(), cmatval.data(), QS_MIN, obj.data(),
                       rhs.data(), sense.data(), lower.data(), upper.data(),
                       NULL, NULL);
  if (qSprob == NULL)
    throw "Can't load LP\n";
  QSset_param(qSprob, QS_PARAM_PRIMAL_PRICING, QS_PRICE_PDANTZIG);
}

void TSP::solve() {
  int indicator;
  int status;
  indicator = QSopt_primal(qSprob, &status);
  if (indicator)
    return;
  if (status != QS_LP_OPTIMAL)
    return;

  double value;
  std::vector<double> x(ncols, -1.0);
  QSget_solution(qSprob, &value, x.data(), NULL, NULL, NULL);
  if (value >= min) return;

  bool allInt = true;
  std::vector<int> path(size);
  for (int i = 0; i < x.size(); ++i) {
    if (!(std::fabs(x[i]) < eps || std::fabs(x[i] - 1) < eps)) {
      allInt = false;
      QSchange_bound(qSprob, i, 'L', 1);
      QSchange_bound(qSprob, i, 'U', 1);
      solve();
      QSchange_bound(qSprob, i, 'L', 0);
      QSchange_bound(qSprob, i, 'U', 0);
      solve();
      QSchange_bound(qSprob, i, 'L', 0);
      QSchange_bound(qSprob, i, 'U', 1);
      break;
    } else if (std::fabs(x[i] - 1) < eps)
      path[i / size] = i % size;
  }

  if (allInt) {
    std::vector<bool> visited(size, false);
    int countChecked = 0;
    bool cycle = false;
    std::vector<std::vector<int>> cycles(size, std::vector<int>());
    int ind = -1;
    for (int i = 0; i < size; ++i)
      if (!visited[i]) {
        ++ind;
        int v = i;
        cycles[ind].push_back(v);
        while (true) {
          visited[v] = true;
          ++countChecked;
          v = path[v];
          cycles[ind].push_back(v);
          if (visited[v] && countChecked != size) {
            cycle = true;
            break;
          } else if (visited[v])
            break;
        }
      }

    if (cycle) {
      for (int j = 0; j <= ind; ++j) {
        int indicator;
        std::vector<double> rmatval(cycles[j].size() - 1, 1.0);
        std::vector<int> rmatind;
        for (int i = 0; i < cycles[j].size() - 1; ++i)
          rmatind.push_back(cycles[j][i] * size + cycles[j][i + 1]);
        indicator = QSadd_row(qSprob, (int) rmatind.size(), rmatind.data(),
                         rmatval.data(), cycles[j].size() - 2, 'L', NULL);
        if (indicator)
          std::cout << "QSadd_row failed with return's: " << indicator << std::endl;
      }
      solve();
    } else min = std::min(value, min);
  }
  return;
}

void TSP::print() {
  double value;
  std::vector<double> x(ncols);
  QSget_solution(qSprob, &value, x.data(), NULL, NULL, NULL);
  std::cout << value << std::endl;
  for (size_t i = 0; i < size; ++i, std::cout << std::endl)
    for (size_t j = 0; j < size; ++j)
      std::cout << x[i*size+j] << '\t';
}

int main () {
  for (size_t i = 0; i < 11; ++i) {
    std::ifstream input("input" + std::to_string(i) + ".txt");
    std::ofstream output("output" + std::to_string(i) + ".txt");
    input >> size;
    matrix.resize(size);
    for (size_t i = 0; i < size; ++i)
      matrix[i].resize(size, 0);
    for (size_t i = 0; i < size; ++i)
      for (size_t j = 0; j < size; ++j) {
        if (i==j) matrix[i][j] = max_value;
        double cost;
        input >> cost;
        matrix[i][j] = cost;
      }
    try {
      TSP tsp;
      tsp.solve();
      output << tsp.get_min() << std::endl;
    } catch (std::string& err) {
      std::cout << err << std::endl;
    }
  }
  return 0;
}
