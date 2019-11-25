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

// Chosen dataset. This is used for fetch file from `inputs` and `outputs` folder.
const string dataset = "own";

int main() {
  // Graph representation in matrix.
  vector<vector<ll>> graph;

  // Size of the graph.
  int N;

  // All variables needed for computation.
  ll current_cost = 0;
  vector<int> current_path;
  set<int> visited_node;

  // Min Heap implementation using STL `priority_queue` container.
  auto comparator = [](pair<ll, vector<int>> p, pair<ll, vector<int>> q) {
    return p.first > q.first;
  };
  priority_queue<
    pair<ll, vector<int>>,
    vector<pair<ll, vector<int>>>,
    decltype(comparator)
  > paths (comparator);

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

  // The main function for solving TSP problem. Brute-force approach using DFS.
  function<void(int)> solve_tsp = [&](int start) {
    if (visited_node.size() == N) {
      current_cost += graph[start][current_path[0]];
      current_path.push_back(current_path[0]);

      paths.push({current_cost, current_path});

      current_cost -= graph[start][current_path[0]];
      current_path.pop_back();
      return;
    }

    if (visited_node.size() == 0) {
      visited_node.insert(start);
      current_path.push_back(start);
    }

    for (int i = 0; i < N; i++) {
      ll cost = graph[start][i];
      if (cost != 0LL && visited_node.find(i) == visited_node.end()) {
        current_cost += cost;
        current_path.push_back(i);
        visited_node.insert(i);

        solve_tsp(i);

        current_cost -= cost;
        current_path.pop_back();
        visited_node.erase(i);
      }
    }
  };

  // Print all possible answers.
  function<void()> print_all_paths = [&comparator, &graph, &paths, &N]() {
    auto pq = paths;

    while (!pq.empty()) {
      pair<ll, vector<int>> current_path = pq.top();
      printf("Cost: %lld\n", current_path.first);

      vector<int> nodes = current_path.second;
      for (int i = 0; i < int(nodes.size()); i++) {
        if (i > 0) {
          printf(" -[%lld]-> ", graph[nodes[i - 1]][nodes[i]]);
        }

        printf("%d", nodes[i]);
      }

      printf("\n");
      pq.pop();
    }
  };

  // Get the final cost after computing the TSP.
  function<ll()> get_final_cost = [&paths]() {
    if (paths.size() == 0) {
      return 0LL;
    }

    return paths.top().first;
  };

  // Get the final path after computing the TSP.
  function<vector<int>()> get_final_path = [&paths]() {
    if (paths.size() == 0) {
      vector<int> ret;
      return ret;
    }

    return paths.top().second;
  };

  // Print out the final path with its edge including its cost.
  function<void(vector<int>)> print_path = [&graph](vector<int> const& path) {
    for (int i = 1; i < int(path.size()); i++) {
      ll cost = graph[path[i - 1]][path[i]];
      printf("%d   -[%lld]->   %d\n", path[i - 1], cost, path[i]);
    }
  };

  // Let's compute the answer. Don't forget to capture the time for benchmarking purpose.
  get_input(dataset);

  double start_time = omp_get_wtime();

  solve_tsp(0);
  ll cost = get_final_cost();
  vector<int> path = get_final_path();

  double end_time = omp_get_wtime();
  double duration = end_time - start_time;

  // Print out the answer. Compare the computed cost with the precomputed answer.
  printf("TSP serial with brute-force approach.\n");
  printf("Computed %d nodes from \"%s\" dataset.\n\n", N, dataset.c_str());
  printf("Optimal cost is %lld.\n", cost);
  printf("Optinal path:\n\n");

  print_path(path);

  printf("\nTook about %.6f seconds.\n", duration);
  printf("Also, the optimal cost is %sCORRECT.\n", cost != get_output(dataset) ? "IN" : "");

  return 0;
}
