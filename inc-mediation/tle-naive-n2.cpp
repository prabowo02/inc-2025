#include <bits/stdc++.h>
using namespace std;

#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,tune=native")

using ll  = long long;
using pii = pair<int, int>;

#define fi first
#define se second

#define All(x) x.begin(), x.end()

const int maxN  = 400'023;

int n, s1, s2;
vector<pii> adj[maxN];
int x[maxN], y[maxN];

void dfs(int *dist, int cur, int par = -1) {
    if (par == -1) dist[cur] = 0;
    for (auto [nxt, w] : adj[cur]) if (nxt != par) {
        dist[nxt] = dist[cur] + w;
        dfs(dist, nxt, cur);
    }
}

int main() {
    ios_base::sync_with_stdio(false); cin.tie(NULL);

    clock_t start = clock();

    cin >> n >> s1 >> s2;
    for (int u, v, w, i = 1; i <= n; i++) {
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    // calculate distances
    dfs(x, s1);
    dfs(y, s2);

    ll ans = 0;
    for (int u = 1; u <= n; u++) {
        for (int v = u+1; v <= n; v++) {
            ans += max(min(x[u], x[v]), min(y[u], y[v]));
        }
    }

    cout << ans << "\n";

    clock_t finish = clock();
    cerr << (double)(finish - start) / CLOCKS_PER_SEC << " seconds\n";
}
