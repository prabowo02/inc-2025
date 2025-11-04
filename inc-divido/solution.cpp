#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const int MX = 100005;
const ll INF = 1e18 + 69;

int n, m;
vector<pair<int, int> > adj[MX];
ll dist[MX][3];

int main() {
	cin.tie(0) -> sync_with_stdio(0);

	cin >> n >> m;
	for (int u, v, w, i = 0; i < m; i++) {
		cin >> u >> v >> w;
		u--; v--;
		adj[u].push_back(make_pair(v, w));
		adj[v].push_back(make_pair(u, w));
	}

	for (int i = 0; i < MX; i++) {
		for (int j = 0; j < 3; j++) {
			dist[i][j] = INF;
		}
	}

	for (int st = 0; st < 3; st++) {
		priority_queue<pair<ll, int>, vector<pair<ll, int> >, greater<pair<ll, int> > > pq;
		dist[st][st] = 0;
		pq.push(make_pair(dist[st][st], st));
		for (; !pq.empty(); pq.pop()) {
			ll dstnw = pq.top().first;
			int nw = pq.top().second;
			if (dist[nw][st] < dstnw) continue;
			for (pair<int, int> nx : adj[nw]) {
				ll cost = nx.second;
				if (st == 1) cost += (cost + 1) / 2;
				if (dist[nx.first][st] > dist[nw][st] + cost) {
					dist[nx.first][st] = dist[nw][st] + cost;
					pq.push(make_pair(dist[nx.first][st], nx.first));
				}
			}
		}
	}

	ll ans = INF;
	for (int i = 0; i < n; i++) {
		ans = min(ans, dist[i][0] + dist[i][1] + dist[i][2]);
	}
	cout << ans << "\n";
	return 0;
}