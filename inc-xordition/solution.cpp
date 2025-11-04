#include <bits/stdc++.h>
using namespace std;

struct Node {
  array<int, 16> val;
  Node() {}
  Node(char op, int x) {
    if (op == '+') {
      for (int i = 0; i < 16; ++i) {
        val[i] = (i + x) % 16;
      }
    } else if (op == 'x') {
      for (int i = 0; i < 16; ++i) {
        val[i] = (i ^ x);
      }
    }
  }
  Node operator * (const Node &other) {
    Node res;
    for (int i = 0; i < 16; ++i) {
      res.val[i] = other.val[val[i]];
    }
    return res;
  }
};

struct SegTree {
  int n;
  vector<Node> nodes;

  void init(int _n) {
    n = _n;
    nodes.resize(n * 2);
  }

  void update(int idx, int l, int r, int pos, char op, int x) {
    if (l == pos && r - 1 == pos) {
      nodes[idx] = Node(op, x);
      return;
    }
    if (l > pos || r <= pos) return;
    int mid = (l + r) / 2;
    int lidx = idx + 1;
    int ridx = idx + (mid - l) * 2;
    update(lidx, l, mid, pos, op, x);
    update(ridx, mid, r, pos, op, x);
    nodes[idx] = nodes[lidx] * nodes[ridx];
  }

  void update(int pos, char op, int x) {
    return update(0, 0, n, pos, op, x);
  }

  int query() {
    return nodes[0].val[0];
  }
} tree;

int main() {
  int N, Q;
  scanf("%d %d", &N, &Q);

  tree.init(N);

  for (int i = 0; i < N; ++i) {
    char op; int x;
    getchar();
    scanf("%c %d", &op, &x);

    tree.update(i, op, x);
  }

  for (int i = 0; i < Q; ++i) {
    int idx, x; char op;
    scanf("%d %c %d", &idx, &op, &x);
    tree.update(idx - 1, op, x);
    printf("%d\n", tree.query());
  }
  return 0;
}
