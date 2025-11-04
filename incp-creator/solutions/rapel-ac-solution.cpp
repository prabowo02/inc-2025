#include <bits/stdc++.h>
#define endl '\n'
using namespace std;

const int N = 2000;
int A[N + 5];

vector<pair<int,int>> adj_list[N + 5]; // (next, id)
vector<pair<int,int>> edges; // (u, v)

// Used for toposort and final dfs to verify.
// In toposort 0: not visited, 1: in progress, 2: done
// In final dfs to verify, the number represents the starting position.
int vis[N + 5];

bool is_valid = true;

// Assign the direction of edges using toposort.
void doDfs(int cur, int p) {
  vis[cur] = 1;
  for (auto& e: adj_list[cur]) {
    int next = e.first;
    int id = e.second;
    if (next == p || vis[next] == 2) {
      // Do nothing if it's a parent node or if the next node already marked as
      // done.
      continue;
    }
    if (vis[next] == 0) {
      // The next node has not been visited yet. Do dfs.
      edges[id] = {cur, next};
      doDfs(next, cur);
    } else if (vis[next] == 1) {
      // The next node is in progress. By assigning the edge it will cause
      // a loop.
      edges[id] = {cur, next};
    }
  }
  vis[cur] = 2;
}

int countNumVisited(int cur, int first_node) {
  vis[cur] = first_node;
  int visited = 1;
  for (auto& e: adj_list[cur]) {
    int next = e.first;
    if (vis[next] == first_node) {
      continue;
    }
    visited += countNumVisited(next, first_node);
  }
  return visited;
}

int main() {
  ios::sync_with_stdio(0), cin.tie(0), cout.tie(0);

  int n, m;
  cin >> n >> m;
  for (int i = 0; i < m; i++) {
    int u, v;
    cin >> u >> v;
    edges.push_back({u, v});
  }

  for (int i = 1; i <= n; i++) {
    cin >> A[i];
  }

  for (int i = 0; i < m; i++) {
    int u = edges[i].first;
    int v = edges[i].second;
    if (A[u] != A[v]) {
      // one node dominates other.
      if (A[u] < A[v]) {
        edges[i] = {v, u};
      }
    } else {
      // both nodes are reachable.
      adj_list[u].push_back({v, i});
      adj_list[v].push_back({u, i});
    }
  }

  for (int i = 1; i <= n; i++) { 
    if (!vis[i]) {
      doDfs(i, i);
    }
  }

  // Reset the adj_list. Only use the directed edge.
  for (int i = 1; i <= n; i++) {
    adj_list[i].clear();
  }
  for (int i = 0; i < m; i++) {
    adj_list[edges[i].first].push_back({edges[i].second, i});
  }

  memset(vis, 0, sizeof vis);
  // do final check
  for (int i = 1; i <= n; i++) {
    if (countNumVisited(i, i) != A[i]) {
      cout << -1 << endl;
      return 0;
    }
  }
  for (int i = 0; i < m; i++) {
    cout << edges[i].first << " " << edges[i].second << endl;
  }
}
