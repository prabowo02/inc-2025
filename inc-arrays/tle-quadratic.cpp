#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;

using Poly = vector<int>;

Poly operator * (Poly a, Poly b) {
  int l = a.size();
  Poly ret(l);

  for (int i = 0; i < l; ++i) {
    for (int j = 0; j < l; ++j) {
      int k = (1LL * i * j) % l;
      ret[k] += 1LL * a[i] * b[j] % MOD;
      if (ret[k] >= MOD) ret[k] -= MOD;
    }
  }
  return ret;
}

Poly power(Poly x, int y) {
  Poly ret(x.size());
  ret[1] = 1;
  for (; y; y >>= 1) {
    if (y & 1) {
      ret = ret * x;
    }
    x = x * x;
  }
  return ret;
}

int main() {
  int N, K, M, L;
  scanf("%d %d %d %d", &N, &K, &M, &L);

  vector<int> S(N);
  for (int i = 0; i < N; ++i) {
    scanf("%d", &S[i]);
  }

  Poly P(M);
  for (int s : S) {
    ++P[s];
  }

  P = power(P, L);

  printf("%d\n", P[K]);
  return 0;
}
