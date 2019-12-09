#include <algorithm>
#include <cstring>
#include <functional>
#include <fstream>
#include <iostream>
#include <omp.h>
#include <queue>
#include <set>
#include <sstream>
#include <vector>

using namespace std;

// Type aliasing. Rename `long long` to `ll`.
typedef long long ll;

const ll INF = ~(0ULL) >> 1;

// Chosen dataset. This is used for fetch file from `inputs` and `outputs` folder.
const string dataset = "own10";

// Chosen cores number or get from the system.
const int cores = omp_get_num_procs();

int main() {
  // Graph representation in matrix.
  vector<vector<ll>> graph;

  // Size of the graph.
  int N;

  // All variables needed for computation.
  vector<int> calculated_cost;
  vector<vector<int>> generated_permutation;
  vector<int> thread_frequencies (cores, 0);

  struct OptimalRoute {
    ll cost = INF;
    int index = -1;
  } optimal_route;

  // Get input from the input file.
  function<void(string)> get_input = [&graph, &N](string filename) {
    ifstream file ("./../inputs/" + filename + ".in");

    if (file.is_open()) {
      string line;
      while (getline(file, line)) {
        graph.push_back(vector<ll>());

        string entry;
        stringstream ss (line);

        while (getline(ss, entry, ' ')) {
          ll cost = stoll(entry);

          graph[graph.size() - 1].push_back(cost);
        }
      }

      N = graph.size();
    } else {
      printf("ERR: Input file does not exists.");
      exit(1);
    }
  };

  // Get output from the output file.
  function<ll(string)> get_output = [](string filename) {
    ifstream file ("./../outputs/" + filename + ".out");

    if (file.is_open()) {
      string cost;
      getline(file, cost);

      return stoll(cost);
    } else {
      printf("ERR: Output file does not exists.");
      exit(1);
    }
  };

  function<void()> generate_permutation = [&]() {
    function<void(vector<int>, int)> recurse = [&](vector<int> v, int size) {
      if (size == 1) {
        generated_permutation.push_back(v);
        thread_frequencies[omp_get_thread_num()] += 1;

        return;
      }

      for (int i = 0; i < size; i++) {
        recurse(v, size - 1);

        if (size & 1) {
          swap(v[0], v[size - 1]);
        } else {
          swap(v[i], v[size - 1]);
        }
      }
    };

    vector<int> v (N);
    for (int i = 0; i < N; i++) {
      v[i] = i;
    }

    recurse(v, N);
  };

  function<void()> calculate_cost = [&]() {
    int M = generated_permutation.size();

    optimal_route.cost = INF;
    optimal_route.index = -1;

    for (int i = 0; i < M; i++) {
      ll current_cost = 0;
      vector<int> current_route = generated_permutation[i];

      for (int j = 0; j < N; j++) {
        if (j == 0) {
          current_cost += graph[current_route[N - 1]][current_route[j]];
        } else {
          current_cost += graph[current_route[j - 1]][current_route[j]];
        }
      }

      calculated_cost.push_back(current_cost);

      if (current_cost < optimal_route.cost) {
        optimal_route.cost = current_cost;
        optimal_route.index = i;
      }

      thread_frequencies[omp_get_thread_num()] += 1;
    }
  };

  // Print out the final path with its edge including its cost.
  function<void(vector<int>)> print_route = [&graph](vector<int> const& path) {
    for (int i = 1; i < int(path.size()); i++) {
      ll cost = graph[path[i - 1]][path[i]];
      printf("%d   -[%lld]->   %d\n", path[i - 1], cost, path[i]);
    }
  };

  // Let's compute the answer. Don't forget to capture the time for benchmarking purpose.
  get_input(dataset);

  printf("TSP serial with brute-force approach.\n");
  printf("Computing %d nodes from \"%s\" dataset...\n\n", N, dataset.c_str());

  double start_time = omp_get_wtime();

  generate_permutation();
  calculate_cost();

  ll cost = optimal_route.cost;
  vector<int> route = generated_permutation[optimal_route.index];

  double end_time = omp_get_wtime();
  double duration = end_time - start_time;

  // Print out the answer. Compare the computed cost with the precomputed answer.
  printf("Optimal cost is %lld.\n", cost);
  printf("Optimal route:\n\n");

  print_route(route);

  printf("\nTook about %.6f seconds.\n", duration);
  printf("Also, the optimal cost is %sCORRECT", cost != get_output(dataset) ? "IN" : "");
  printf(" based on the dataset.\n\n");

  printf("Thread frequencies:\n");
  for (int i = 0; i < cores; i++) {
    printf("#%d ran %d time(s).\n", i, thread_frequencies[i]);
  }

  return 0;
}
