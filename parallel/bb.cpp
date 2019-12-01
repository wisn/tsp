#include <cstring>
#include <climits>
#include <fstream>
#include <iostream>
#include <omp.h>
#include <sstream>
#include <queue>
#include <utility>
#include <vector>

using namespace std;

#define N 26
#define DATASET "fri26"
#define INF INT_MAX
#define PARALLEL true

struct Node
{
  // Stores edges of state space tree helps in tracing path when answer is found
  vector<pair<int, int>> path;

  // Stores the reduced matrix
  int reduced_matrix[N][N];

  // Stores the lower bound
  int cost;

  // Stores current city number
  int vertex;

  // Stores number of cities visited so far
  int level;
};

// Allocate a new node (i, j) corresponds to visiting city j from city i
Node* new_node(int parent_matrix[N][N], vector<pair<int, int>> const &path, int level, int i, int j)
{
  Node* node = new Node;

  // Stores ancestors edges of state space tree
  node->path = path;

  // Skip for root node
  if (level != 0)
    // Add current edge to path
    node->path.push_back(make_pair(i, j));

  // Copy data from parent node to current node
  memcpy(node->reduced_matrix, parent_matrix, sizeof node->reduced_matrix);

  // Change all entries of row i and column j to infinity
  // Skip for root node
  for (int k = 0; level != 0 && k < N; k++)
  {
    // Set outgoing edges for city i to infinity
    node->reduced_matrix[i][k] = INF;

    // Set incoming edges to city j to infinity
    node->reduced_matrix[k][j] = INF;
  }

  // Set (j, 0) to infinity
  // Here start node is 0
  node->reduced_matrix[j][0] = INF;

  // Set number of cities visited so far
  node->level = level;

  // Assign current city number
  node->vertex = j;

  // Return node
  return node;
}

// Reduce each row in such a way that there must be at least one zero in each row
void row_reduction(int reduced_matrix[N][N], int row[N])
{
  // Initialize row array to INF
  fill_n(row, N, INF);

  // Row[i] contains minimum in row i
  if (PARALLEL) 
  {
    #pragma omp for
    for (int i = 0; i < N; i++)
      for (int j = 0; j < N; j++)
        if (reduced_matrix[i][j] < row[i])
          row[i] = reduced_matrix[i][j];
  }
  else
  {
    for (int i = 0; i < N; i++)
      for (int j = 0; j < N; j++)
        if (reduced_matrix[i][j] < row[i])
          row[i] = reduced_matrix[i][j]; 
  }

  // Reduce the minimum value from each element in each row
  if (PARALLEL) 
  {
    #pragma omp for
    for (int i = 0; i < N; i++)
      for (int j = 0; j < N; j++)
        if (reduced_matrix[i][j] != INF && row[i] != INF)
          reduced_matrix[i][j] -= row[i];
  }
  else
  {
    for (int i = 0; i < N; i++)
      for (int j = 0; j < N; j++)
        if (reduced_matrix[i][j] != INF && row[i] != INF)
          reduced_matrix[i][j] -= row[i];
  }
}

// Reduce each column in such a way that there must be at least one zero in each column
void column_reduction(int reduced_matrix[N][N], int col[N])
{
  // Initialize col array to INF
  fill_n(col, N, INF);

  // col[j] contains minimum in col j
  if (PARALLEL) 
  {
    #pragma omp for
    for (int i = 0; i < N; i++)
      for (int j = 0; j < N; j++)
        if (reduced_matrix[i][j] < col[j])
          col[j] = reduced_matrix[i][j];
  }
  else
  {
    for (int i = 0; i < N; i++)
      for (int j = 0; j < N; j++)
        if (reduced_matrix[i][j] < col[j])
          col[j] = reduced_matrix[i][j];
  }

  // Reduce the minimum value from each element in each column
  if (PARALLEL) 
  {
    #pragma omp for
    for (int i = 0; i < N; i++)
      for (int j = 0; j < N; j++)
        if (reduced_matrix[i][j] != INF && col[j] != INF)
          reduced_matrix[i][j] -= col[j];
  }
  else
  {
    for (int i = 0; i < N; i++)
      for (int j = 0; j < N; j++)
        if (reduced_matrix[i][j] != INF && col[j] != INF)
          reduced_matrix[i][j] -= col[j];
  }
}

// Get the lower bound on on the path starting at current min node
int calculate_cost(int reduced_matrix[N][N])
{
  // Initialize cost to 0
  int cost = 0;

  // Row Reduction
  int row[N];
  row_reduction(reduced_matrix, row);

  // Column Reduction
  int col[N];
  column_reduction(reduced_matrix, col);

  // The total expected cost is the sum of all reductions
  if (PARALLEL)
  {
    #pragma omp for
    for (int i = 0; i < N; i++)
      cost += (row[i] != INT_MAX) ? row[i] : 0,
        cost += (col[i] != INT_MAX) ? col[i] : 0;
  }
  else
  {
    for (int i = 0; i < N; i++)
      cost += (row[i] != INT_MAX) ? row[i] : 0,
        cost += (col[i] != INT_MAX) ? col[i] : 0;
  }

  return cost;
}

// Print list of cities visited following least cost
void print_path(vector<pair<int, int>> const &list) {
  for (int i = 0; i < list.size(); i++)
    printf("%d  ->  %d\n", list[i].first, list[i].second);
}

// Comparison object to be used to order the heap
struct comp {
  bool operator()(const Node* lhs, const Node* rhs) const {
    return lhs->cost > rhs->cost;
  }
};

// Solve Traveling Salesman Problem using Branch and Bound
int solve(int matrix[N][N]) {
  // Create a priority queue to store live nodes of search tree;
  priority_queue<Node*, vector<Node*>, comp> pq;

  vector<pair<int, int>> v;

  // Create a root node and calculate its cost
  // The TSP starts from first city i.e. node 0
  Node* root = new_node(matrix, v, 0, -1, 0);

  // Get the lower bound of the path starting at node 0
  root->cost = calculate_cost(root->reduced_matrix);

  // Add root to list of live nodes;
  pq.push(root);

  // Finds a live node with least cost, add its children to list of
  // Live nodes and finally deletes it from the list
  while (!pq.empty()) {
    // Find a live node with least estimated cost
    Node* min = pq.top();

    // The found node is deleted from the list of live nodes
    pq.pop();

    // i stores current city number
    int i = min->vertex;

    // If all cities are visited
    if (min->level == N - 1) {
      // Return to starting city
      min->path.push_back(make_pair(i, 0));

      // Print list of cities visited;
      print_path(min->path);

      // Return optimal cost
      return min->cost;
    }

    // Do for each child of min
    // (i, j) forms an edge in space tree
    for (int j = 0; j < N; j++) {
      if (min->reduced_matrix[i][j] != INF) {
        // Create a child node and calculate its cost
        Node* child = new_node(min->reduced_matrix, min->path, min->level + 1, i, j);

        /* Cost of the child =
          cost of parent node +
          cost of the edge(i, j) +
          lower bound of the path starting at node j
        */
        child->cost = min->cost + min->reduced_matrix[i][j] + calculate_cost(child->reduced_matrix);

        // Add child to list of live nodes
        pq.push(child);
      }
    }

    // Free node as we have already stored edges (i, j) in vector.
    // So no need for parent node while printing solution.
    delete min;
  }
  return 0;
}

// Generate NxN matrix cost in range of 1 to max
void generate_cost(int matrix[N][N]) {
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      if (i == j) 
        matrix[i][j] = INF;
}

// Get input from input file and use it as NxN matrix
void get_input(int matrix[N][N], string filename) 
{
  ifstream file ("./../inputs/" + filename + ".in");

  if (file.is_open()) 
  {
    size_t lines = 0;
    size_t column = 0;
    string line;

    while (getline(file, line)) {
      string entry;
      stringstream ss (line);
      
      column = 0;
      while (getline(ss, entry, ' ')) {
        int cost = stoi(entry);
        matrix[lines][column] = cost;
        ++column;
      }

      ++lines;
    }
  } 
  else 
  {
    printf("ERR: Input file does not exists.");
    exit(1);
  }

  // Convert 0 to INF
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      if (i == j) 
        matrix[i][j] = INF;
}

// Check if optimal cost is correct
int get_output(string filename) 
{
  ifstream file ("./../outputs/" + filename + ".out");

  if (file.is_open()) {
    string cost;
    getline(file, cost);
    return stoi(cost);
  } else {
    printf("ERR: Output file does not exists.");
    exit(1);
  }
}

int main() 
{
  // Matrix to calculate.
  int matrix[N][N];

  get_input(matrix, DATASET);  
  generate_cost(matrix);

  // Print out the answer. Compare the computed cost with the precomputed answer.
  if (PARALLEL)  
    printf("TSP parallel with branch-and-bound approach.\n");
  else  
    printf("TSP serial with branch-and-bound approach.\n");

  printf("Computed %d nodes from \"%s\" dataset.\n\n", N, DATASET);

  double start_time = omp_get_wtime();

  // Print path and get optimal cost
  int solve_tsp = solve(matrix);
  
  double end_time = omp_get_wtime();
  double duration = end_time - start_time;

  printf("\nTook about %.6f seconds.\n", duration);
  printf("Also, the optimal cost is %sCORRECT.\n", solve_tsp != get_output(DATASET) ? "IN" : "");
  return 0;
}
