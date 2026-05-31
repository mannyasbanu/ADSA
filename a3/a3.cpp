#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <queue>
#include <climits>
#include <algorithm>
using namespace std;

// M is 0-indexed internally: M[i][j] = probability of hop from node (i+1) to node (j+1).
double solve_part1(const vector<vector<double>>& M) {

  const int n = M.size();
  
  vector<double> probabilities(n, 0.0);
  priority_queue<pair<double,int>> pq;

  probabilities[0] = 1.0;
  pq.emplace(probabilities[0], 0);

  while (!pq.empty()) {

    auto [prob, node] = pq.top();
    pq.pop();

    if (probabilities[node] > prob) continue;

    for (int to = 0; to < n; ++to) {

      if (to != node && M[node][to] > 0.0) {

        double newProb = prob * M[node][to];

        if (newProb > probabilities[to]) {

          probabilities[to] = newProb;
          pq.emplace(newProb, to);
        }
      }
    }
  }
  return probabilities[n - 1];
}

struct Edge {
  
  int to;
  int reverse;
  int capacity;
  
  Edge(int t, int r, int c) : to(t), reverse(r), capacity(c) {};
};

class MaxFlow {
  public:
  
  int n;
  vector<vector<Edge>> graph;

  MaxFlow(int n) : n(n), graph(n) {};
  
  void addEdge(int from, int to, int capacity) {
    
    Edge forward(to, (int)graph[to].size(), capacity);
    Edge backward(from, (int)graph[from].size(), 0);

    graph[from].push_back(forward);
    graph[to].push_back(backward);
  }

  int calculateMaxFlow(int source, int target) {

    int flow = 0;

    while (1) {

      // bfs parent array
      vector<int> parentNode(n, -1);
      vector<int> parentEdge(n, -1);

      // start bfs from source
      queue<int> q;
      q.push(source);
      parentNode[source] = source;

      while (!q.empty() && parentNode[target] == -1) {
        
        int node = q.front();
        q.pop();

        // iterate outward edges
        for (int i = 0; i < (int)graph[node].size(); ++i) {

          Edge &edge = graph[node][i];

          // only follow unexplored paths with capacity
          if (parentNode[edge.to] == -1 && edge.capacity > 0) {

            parentNode[edge.to] = node;
            parentEdge[edge.to] = i;
            q.push(edge.to);

            if (edge.to == target) break;
          }
        }
      }
      if (parentNode[target] == -1) break;

      // push flow backwards through the path
      int cur = target;

      while (cur != source) {

        int prev = parentNode[cur];
        int edgeIndex = parentEdge[cur];
        
        Edge &edge = graph[prev][edgeIndex];
        edge.capacity = 0;
        graph[edge.to][edge.reverse].capacity = 1;

        cur = prev;
      }

      flow += 1;
    }
    return flow;
  }

  vector<bool> reachableFromSource(int source) {

    vector<bool> visited(n, false);
    queue<int> q;

    q.push(source);
    visited[source] = true;

    while (!q.empty()) {

      int node = q.front();
      q.pop();

      for (Edge &edge : graph[node]) {

        if (!visited[edge.to] && edge.capacity > 0) {
          visited[edge.to] = true;
          q.push(edge.to);
        }
      }
    }
    return visited;
  }
};

vector<pair<int,int>> findMinCutEdges(const vector<vector<double>>& M, MaxFlow& maxFlow) {

  const int n = M.size();

  vector<bool> reachable = maxFlow.reachableFromSource(0);

  vector<pair<int,int>> cutEdges;

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {

      if (i != j && M[i][j] > 0.0) {

        if (reachable[i] && !reachable[j]) cutEdges.push_back({i + 1, j + 1});
      }
    }
  }

  return cutEdges;
}

bool containsEdge(const vector<pair<int,int>> &edges, pair<int,int> target) {
  
  for (const auto &edge : edges) {

    if (edge == target) return true;
  }
  return false;
}

void addCandidateEdge(vector<pair<int,int>> &edges, pair<int,int> edge) {
  
  if (!containsEdge(edges, edge)) edges.push_back(edge);
}

vector<vector<double>> removeMatrixEdges(const vector<vector<double>> &M, const vector<pair<int,int>> &edges) {

  vector<vector<double>> Mcopy = M;

  for (const auto &edge : edges) {

    Mcopy[edge.first - 1][edge.second - 1] = 0.0;
  }

  return Mcopy;
}

pair<double, vector<pair<int,int>>> getBestPath(const vector<vector<double>> &M) {

  const int n = M.size();

  vector<double> bestProb(n, 0.0);
  vector<int> parent(n, -1);

  priority_queue<pair<double,int>> pq;
  pq.emplace(1.0, 0);
  bestProb[0] = 1.0;

  while (!pq.empty()) {

    auto [prob, node] = pq.top();
    pq.pop();

    if (prob < bestProb[node]) continue;

    for (int to = 0; to < n; ++to) {

      if (node != to && M[node][to] > 0) {

        double nProb = prob * M[node][to];

        if (nProb > bestProb[to]) {

          bestProb[to] = nProb;
          parent[to] = node;
          pq.emplace(nProb, to);
        }
      }
    }
  }

  vector<pair<int,int>> bestEdges;

  if (bestProb[n - 1] == 0.0) return {0.0, bestEdges};

  int cur = n - 1;
  
  while (cur != 0) {

    int prev = parent[cur];

    if (prev == -1) break;

    bestEdges.push_back({prev + 1, cur + 1});

    cur = prev;
  }
  reverse(bestEdges.begin(), bestEdges.end());

  return {bestProb[n - 1], bestEdges};
}

void set3Edges(const vector<vector<double>> &M, vector<pair<int,int>> &edges) {

  const int n = M.size();

  while (edges.size() < 3) {

    bool added = false;

    for (int i = 0; i < n && !added; ++i) {
      for (int j = 0; j < n && !added; ++j) {

        if (i != j && M[i][j] > 0.0) {

          pair<int,int> edge = {i + 1, j + 1};

          if (!containsEdge(edges, edge)) {
            edges.push_back(edge);
            added = true;
          }
        }
      }
    }
    // guard against < 3 edges
    if (!added) break;
  }
}

vector<pair<int,int>> getCandidateEdges(const vector<vector<double>> &M) {

  // amount of edges to consider removing
  const int CANDIDATE_LIMIT = 30;

  vector<pair<int,int>> candidates;

  // add edges from best path
  auto [bestProb, bestEdges] = getBestPath(M);

  for (const auto &edge : bestEdges) {

    addCandidateEdge(candidates, edge);

    if (candidates.size() >= CANDIDATE_LIMIT) return candidates;
  }

  // add edges from next best paths after 1 removal
  for (const auto& bestEdge : bestEdges) {

    vector<vector<double>> Mcopy = removeMatrixEdges(M, {bestEdge});

    auto [nextBestProb, nextBestPath] = getBestPath(Mcopy);

    for (const auto& edge : nextBestPath) {

      addCandidateEdge(candidates, edge);

      if (candidates.size() >= CANDIDATE_LIMIT) return candidates;
    }
  }

  // add edges from next best paths after 2 removals
  int numCandidates = candidates.size();

  for (int i = 0; i < numCandidates; ++i) {
    for (int j = i + 1; j < numCandidates; ++j) {

      vector<vector<double>> Mcopy = removeMatrixEdges(M, {candidates[i], candidates[j]});

      auto [nextBestProb, nextBestPath] = getBestPath(Mcopy);

      for (const auto &edge : nextBestPath) {

        addCandidateEdge(candidates, edge);

        if (candidates.size() >= CANDIDATE_LIMIT) return candidates;
      }
    }
  }

  return candidates;
}

double getBestTripleRemoval(const vector<vector<double>> &M, const vector<pair<int,int>> &candidates, vector<pair<int,int>> &edges) {

  double minProb = INT_MAX;

  const int c = candidates.size();

  for (int i = 0; i < c; ++i) {
    for (int j = i + 1; j < c; ++j) {
      for (int k = j + 1; k < c; ++k) {

        vector<pair<int,int>> removed = {candidates[i], candidates[j], candidates[k]};

        vector<vector<double>> Mcopy = removeMatrixEdges(M, removed);

        double prob = solve_part1(Mcopy);

        if (prob < minProb) {
          minProb = prob;
          edges = removed;
        }
      }
    }
  }
  return minProb;
}

// Writes exactly 3 chosen edges (1-indexed) into edges_out.
// Returns the new max probability after zeroing those edges.
double solve_part2(const vector<vector<double>>& M, vector<pair<int,int>>& edges_out) {

  const int n = M.size();

  // PHASE 1: Determine whether all possible paths can be removed by cutting 3 or less edges using max flow algorithm and s-t cut

  // build directed flow graph
  MaxFlow maxFlow(n);

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; ++j) {
      if (i != j && M[i][j] > 0.0) maxFlow.addEdge(i, j, 1);
    }
  }

  int source = 0;
  int target = n - 1;

  // find minimum cuts to remove all paths
  int minCuts = maxFlow.calculateMaxFlow(source, target);

  // return edges to cut if less or equal to 3
  if (minCuts <= 3) {
    edges_out = findMinCutEdges(M, maxFlow);
    set3Edges(M, edges_out);
    return 0.0;
  }

  // PHASE 2: Find important edges from the best paths and brute force triplet removal

  // get a list of edges contributing to the highest probabilities
  vector<pair<int,int>> candidates = getCandidateEdges(M);

  // ensure it contains at least 3 edges
  set3Edges(M, candidates);

  double minProb = getBestTripleRemoval(M, candidates, edges_out);

  // ensure output is at least 3 edges
  set3Edges(M, edges_out);

  return minProb;
}

void printEdges(const vector<pair<int,int>> &edges) {

  for (auto edge : edges) {

    cout << " (" << edge.first << "," << edge.second << ")";
  }
  cout << endl;
}

int main () {

  ifstream file("matrix.txt");

  int n;
  file >> n;

  vector<vector<double>> matrix(n, vector<double>(n));

  for (int i = 0; i < n; ++i) {

    for (int j = 0; j < n; ++j) {

      double p;
      file >> p;

      matrix[i][j] = p;
    }
  }

  double prob_part1 = solve_part1(matrix);

  cout << "PART1: " << fixed << setprecision(6) << prob_part1 << endl;

  vector<pair<int,int>> edges_part2;
  double prob_part2 = solve_part2(matrix, edges_part2);
  
  cout << "PART2_EDGES:";
  printEdges(edges_part2);
  cout << "PART2_PROB: " << prob_part2 << endl;

  return 0;
}
