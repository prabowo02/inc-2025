#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;

using Poly = vector<int>;

int power(int x, int y) {
  int ret = 1;
  for (; y; y >>= 1) {
    if (y & 1) ret = 1LL * ret * x % MOD;
    x = 1LL * x * x % MOD;
  }
  return ret;
}

namespace FFT {

const int LN = 21;
const int N = 1 << LN;

const int PRIMITIVE_ROOT = 3;

int root[N];

void init_fft() {
  const int UNITY = power(PRIMITIVE_ROOT, (MOD-1) >> LN);
  root[0] = 1;
  for (int i = 1; i < N; ++i) {
    root[i] = 1LL * UNITY * root[i-1] % MOD;
  }
}

// n is the length of polynom
void fft(int n, vector<int> &a, bool invert) {
  for (int i = 1, j = 0; i < n; ++i) {
    int bit = n >> 1;
    for (; j & bit; bit >>= 1) j ^= bit;
    j ^= bit;
    if (i < j) swap(a[i], a[j]);
  }

  for (int len = 2; len <= n; len <<= 1) {
    int wlen = (invert ? root[N - N/len] : root[N/len]);
    for (int i = 0; i < n; i += len) {
      int w = 1;
      for (int j = 0; j < len >> 1; ++j) {
        int u = a[i + j], v = 1LL * a[i + j + len/2] * w % MOD;

        a[i + j] = (u + v) % MOD;
        a[i + j + len/2] = (u - v + MOD) % MOD;
        w = 1LL * w * wlen % MOD;
      }
    }
  }

  if (invert) {
    int inv = power(n, MOD-2);
    for (int i = 0; i < n; ++i) a[i] = 1LL * a[i] * inv % MOD;
  }
}

}

vector<int> multiply_small(vector<int> a, vector<int> b) {
  vector<int> c(a.size() + b.size() - 1);
  for (int i = 0; i < a.size(); ++i) {
    for (int j = 0; j < b.size(); ++j) {
      c[i+j] = (1LL * a[i] * b[j] + c[i+j]) % MOD;
    }
  }
  return c;
}

vector<int> multiply_large(vector<int> a, vector<int> b) {
  int deg = a.size() + b.size() - 2;
  int len = (deg == 0 ? 1 : 1 << (32 - __builtin_clz(deg)));
  a.resize(len); b.resize(len);
  FFT::fft(len, a, false); FFT::fft(len, b, false);
  a.resize(len);
  for (int i = 0; i < len; ++i) a[i] = 1LL * a[i] * b[i] % MOD;
  FFT::fft(len, a, true);
  a.resize(deg + 1);
  return a;
}

Poly operator + (Poly a, Poly b) {
  if (a.size() < b.size()) swap(a, b);
  for (int i = 0; i < b.size(); ++i) a[i] = (a[i] + b[i]) % MOD;
  return a;
}

Poly operator * (Poly a, Poly b) {
  if (1LL * a.size() * b.size() <= 5000) return multiply_small(a, b);
  return multiply_large(a, b);
}

const int MAXN = 100005;

int fact[MAXN];

void init() {
  FFT::init_fft();

  fact[0] = 1;
  for (int i = 1; i < MAXN; ++i) {
    fact[i] = 1LL * fact[i - 1] * i % MOD;
  }
}

char s[MAXN];

int main() {
  init();

  int N;
  scanf("%d", &N);
  scanf("%s", s);

  vector<Poly> P, Q, R, S;
  for (int i = 0; i < N; ++i) {
    if (i > 0 && s[i - 1] == '1') {
      P.push_back({0});
      Q.push_back({0});
      R.push_back({1});
      S.push_back({1});
    } else {
      P.push_back(Poly{1, 0});  // closed, closed
      Q.push_back(Poly{2, 0});  // closed, open
      R.push_back(Poly{0, 1});  // open, closed
      S.push_back(Poly{0, 1});  // open, open
    }
  }

  for (int len = 1; len < N; len <<= 1) {
    for (int i = 0; i + len < N; i += len << 1) {
      int j = i + len;
      Poly tP = P[i] * P[j] + Q[i] * R[j];
      Poly tQ = Q[i] * S[j] + P[i] * Q[j];
      Poly tR = R[i] * P[j] + S[i] * R[j];
      Poly tS = S[i] * S[j] + R[i] * Q[j];
      swap(P[i], tP); swap(Q[i], tQ); swap(R[i], tR); swap(S[i], tS);
    }
  }

  int m = N;
  for (int i = 0; i < N; ++i) {
    if (s[i] == '1') --m;
  }

  int res = 0;
  for (int i = 0; i < m; ++i) {
    int add = 1LL * P[0][i] * fact[m-i] % MOD;
    if (i & 1) add = MOD - add;
    res = (res + add) % MOD;
  }

  printf("%d\n", res);
}
