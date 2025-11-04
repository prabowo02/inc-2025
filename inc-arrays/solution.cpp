#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;

using Poly = vector<int>;

int power(int x, int y, int m=MOD) {
  int ret = 1;
  for (; y; y >>= 1) {
    if (y & 1) ret = 1LL * ret * x % m;
    x = 1LL * x * x % m;
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

Poly power(Poly x, int y) {
  Poly ret = {1};
  int l = x.size();
  for (; y; y >>= 1) {
    if (y & 1) {
      ret = ret * x;
      while (ret.size() > l) {
        (ret[ret.size() - l - 1] += ret.back()) %= MOD;
        ret.pop_back();
      }
    }
    x = x * x;
    while (x.size() > l) {
      (x[x.size() - l - 1] += x.back()) %= MOD;
      x.pop_back();
    }
  }
  return ret;
}

vector<int> divisors(int n) {
  vector<int> ret;
  for (int i = 1; i * i <= n; ++i) {
    if (n % i == 0) {
      ret.push_back(i);
      if (i * i != n) {
        ret.push_back(n / i);
      }
    }
  }
  return ret;
}

int findPrimitiveRoot(int p) {
  vector<int> factors = divisors(p - 1);
  for (int g = 2; ; ++g) {
    vector<int> powers;
    for (int d : factors) {
      powers.push_back(power(g, d, p));
    }
    sort(powers.begin(), powers.end());
    if (unique(powers.begin(), powers.end()) == powers.end()) return g;
  }
}

int main() {
  FFT::init_fft();

  int N, K, M, L;
  scanf("%d %d %d %d", &N, &K, &M, &L);

  vector<int> S(N);
  for (int i = 0; i < N; ++i) {
    scanf("%d", &S[i]);
  }

  int g = findPrimitiveRoot(M);
  vector<int> logG(M);

  for (int i = 0, r = 1; i < M - 1; ++i, r = 1LL * r * g % M) {
    logG[r] = i; // g^i = r (mod M)
  }

  Poly P(M - 1);
  for (int s : S) {
    ++P[logG[s]];
  }

  P = power(P, L);

  printf("%d\n", P[logG[K]]);
  return 0;
}
