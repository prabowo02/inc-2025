#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1e3;

int N, M, Q;
string GRID[MAXN + 5];

inline int queryH(int id, int l, int r) {
  int ans = 0;
  for (int i = l; i <= r; i++) ans += (GRID[id][i] == '1');
  return ans;
}

inline int queryV(int id, int l, int r) {
  int ans = 0;
  for (int i = l; i <= r; i++) ans += (GRID[i][id] == '1');
  return ans;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> N >> M;
  for (int i = 0; i < N; i++) cin >> GRID[i];

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
      if (R - dist >= 0) gain += queryH(R - dist, max(0, C - dist), min(M - 1, C + dist));
      if (R + dist <= N - 1) gain += queryH(R + dist, max(0, C - dist), min(M - 1, C + dist));
      if (C - dist >= 0) gain += queryV(C - dist, max(0, R - dist + 1), min(N - 1, R + dist - 1));
      if (C + dist <= M - 1) gain += queryV(C + dist, max(0, R - dist + 1), min(N - 1, R + dist - 1));
      gain = min(gain, K);
      ans += 2ll * dist * gain;
      K -= gain;
      dist++;
    }
    cout << ans << "\n";
  }
  return 0;
}