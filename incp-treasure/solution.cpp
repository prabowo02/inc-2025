#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1e3;

int N, M, Q;
string GRID[MAXN + 5];
int PS[MAXN + 5][MAXN + 5];

inline int query(int xl, int yl, int xr, int yr) {
  int ans = PS[xr][yr];
  if (xl > 0) ans -= PS[xl - 1][yr];
  if (yl > 0) ans -= PS[xr][yl - 1];
  if (xl > 0 && yl > 0) ans += PS[xl - 1][yl - 1];
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> N >> M;
  for (int i = 0; i < N; i++) cin >> GRID[i];
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      PS[i][j] = (GRID[i][j] == '1');
      if (i > 0) PS[i][j] += PS[i - 1][j];
      if (j > 0) PS[i][j] += PS[i][j - 1];
      if (i > 0 && j > 0) PS[i][j] -= PS[i - 1][j - 1];
    }
  }
  cin >> Q;
  while (Q--) {
    int R, C, K;
    cin >> R >> C >> K;
    R--;
    C--;
    K -= (GRID[R][C] == '1');

    long long ans = 0;
    int dist = 1;
    while (K > 0) {
      int gain = 0;
      if (R - dist >= 0) gain += query(R - dist, max(0, C - dist), R - dist, min(M - 1, C + dist));
      if (R + dist <= N - 1) gain += query(R + dist, max(0, C - dist), R + dist, min(M - 1, C + dist));
      if (C - dist >= 0) gain += query(max(0, R - dist + 1), C - dist, min(N - 1, R + dist - 1), C - dist);
      if (C + dist <= M - 1) gain += query(max(0, R - dist + 1), C + dist, min(N - 1, R + dist - 1), C + dist);
      gain = min(gain, K);
      ans += 2ll * dist * gain;
      K -= gain;
      dist++;
    }
    cout << ans << "\n";
  }
  return 0;
}