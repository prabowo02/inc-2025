#include <bits/stdc++.h>
using namespace std;

int main() {
  int N, Q;
  scanf("%d %d", &N, &Q);

  vector<char> OP(N);
  vector<int> X(N);

  for (int i = 0; i < N; ++i) {
    char op; int x;
    getchar();
    scanf("%c %d", &op, &x);

    OP[i] = op;
    X[i] = x;
  }

  for (int i = 0; i < Q; ++i) {
    int idx, x; char op;
    scanf("%d %c %d", &idx, &op, &x);
    --idx;
    OP[idx] = op;
    X[idx] = x;

    int ans = 0;
    for (int i = 0; i < N; ++i) {
      if (OP[i] == '+') {
        ans = (ans + X[i]) & 15;
      } else {
        ans ^= X[i];
      }
    }
    printf("%d\n", ans);
  }
  return 0;
}
