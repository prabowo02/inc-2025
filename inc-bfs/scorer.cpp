#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>
#include <set>

void ac() {
  std::cout << "AC\n";
  exit(0);
}

void wa(std::string reason="") {
  std::cout << "WA\n";
  if (reason != "") {
    std::cout << reason << '\n';
  }
  exit(0);
}

int readInt(std::ifstream &ifs) {
  int ret;
  if (!(ifs >> ret)) wa();
  return ret;
}

long long readLongLong(std::ifstream &ifs) {
  long long ret;
  if (!(ifs >> ret)) wa();
  return ret;
}

bool eof(std::ifstream &ifs) {
  std::string dummy;
  if (ifs >> dummy) return false;
  return true;
}

int main(int argc, char* argv[]) {
  if (argc != 4) {
    std::cerr << "Must be run with arguments [input-file] [output-file] [contestant-output]\n";
    exit(0);
  }

  std::ifstream inp(argv[1]);
  std::ifstream out(argv[2]);
  std::ifstream con(argv[3]);

  int N = readInt(inp);
  int M = readInt(inp);
  std::vector<int> A(N);
  for (int i = 0 ; i < N ; i++) {
    A[i] = readInt(inp);
  }

  int u1 = readInt(con);
  int v1 = readInt(con);

  if (u1 < 1 || u1 > N) {
    if (u1 != -1) wa();
    if (v1 != -1) wa();

    int judgeU1 = readInt(out);
    if (judgeU1 != -1) wa();
    if (!eof(con)) wa();

    ac();
  }

  if (v1 < 1 || v1 > N || u1 == v1) wa();

  std::set<std::pair<int, int>> edgeSet;
  if (u1 > v1) std::swap(u1, v1);
  edgeSet.insert({u1, v1});

  for (int i = 2 ; i <= M ; i++) {
    int u = readInt(con);
    int v = readInt(con);
    if (u > v) std::swap(u, v);

    if (u < 1 || u > N) wa();
    if (v < 1 || v > N || v == u) wa();
    if (edgeSet.count({u, v})) wa();
    edgeSet.insert({u, v});
  }
  if (!eof(con)) wa();

  std::vector<std::vector<int>> adj(N+1);
  for (auto [u, v] : edgeSet) {
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  std::vector<bool> visited(N+1, false);
  std::vector<int> visOrder;
  std::queue<int> bfsQueue;

  visited[1] = true;
  bfsQueue.push(1);

  while (!bfsQueue.empty()) {
    int u = bfsQueue.front();
    bfsQueue.pop();
    visOrder.push_back(u);

    std::sort(adj[u].begin(), adj[u].end());
    for (int v : adj[u]) {
        if (!visited[v]) {
            visited[v] = true;
            bfsQueue.push(v);
        }
    }
  }

  if (visOrder != A) wa();
  ac();
}
