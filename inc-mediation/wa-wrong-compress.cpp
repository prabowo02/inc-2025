#include <bits/stdc++.h>
using namespace std;

using ll  = long long;
using pii = pair<int, int>;

#define fi first
#define se second

#define All(x) x.begin(), x.end()

const int maxN  = 200'023;

struct FenwickTree {
    int n;
    vector<ll> fen;

    FenwickTree(int _n = 0) : n(_n), fen(_n + 1, 0) {}

    void update(int idx, ll val) {
        for (; idx <= n; idx += (idx & -idx)) fen[idx] += val;
    }

    ll query(int idx) {
        ll ret = 0;
        for (; idx > 0; idx -= (idx & -idx)) ret += fen[idx];
        return ret;
    }

    ll query(int l, int r) {
        return query(r) - query(l - 1);
    }
};

struct Event {
    // types:
    // 0: count
    // 1: x[i]
    // 2: y[i]
    // 3: max(x[i], y[i])
    int typ;
    int l, r;  // range of y in query
    ll mult;   // multiplier of query result
    int id;    // for debugging
};

int n, s1, s2;
vector<pii> adj[maxN];
int x[maxN], y[maxN];
int x_sz, y_sz;
ll orig_x[maxN], orig_y[maxN];

int id_timer = 0;
vector<int> by_x[maxN];
vector<Event> r_que[maxN];

void dfs(int *dist, int cur, int par = -1) {
    if (par == -1) dist[cur] = 0;
    for (auto [nxt, w] : adj[cur]) if (nxt != par) {
        dist[nxt] = dist[cur] + w;
        dfs(dist, nxt, cur);
    }
}

void compress(int *dist, int &sz, ll *orig) {
    for (int i = 1; i <= n; i++) orig[i] = dist[i];

    vector<int> vals(dist + 1, dist + n + 1);
    sort(All(vals));
    vals.erase(unique(All(vals)), vals.end());

    sz = vals.size();
    for (int i = 1; i <= n; i++) {
        dist[i] = lower_bound(All(vals), dist[i]) - vals.begin() + 1;
    }
}

void addRangeQuery(int typ, int x1, int x2, int y1, int y2, ll mult, int id) {
    if (x1 > x2 || y1 > y2) return;
    r_que[x1 - 1].push_back(Event{typ: typ, l: y1, r: y2, mult: -mult, id: id});
    r_que[x2    ].push_back(Event{typ: typ, l: y1, r: y2, mult:  mult, id: id});
}

ll solveRangeQueries() {
    ll ans = 0;

    array<FenwickTree, 4> fen;
    for (auto &f : fen) f = FenwickTree(y_sz);

    vector<ll> ans_by_id(id_timer, 0);

    for (int i = 1; i <= x_sz; i++) {
        for (auto u : by_x[i]) {
            fen[0].update(y[u], 1);
            fen[1].update(y[u], orig_x[u]);
            fen[2].update(y[u], orig_y[u]);
            fen[3].update(y[u], max(orig_x[u], orig_y[u]));
        }

        for (auto [typ, l, r, mult, id] : r_que[i]) {
            ans_by_id[id] += mult * fen[typ].query(l, r);
            ans += mult * fen[typ].query(l, r);
        }
    }

    // cout << "---\n";
    // for (int i = 0; i < id_timer; i++) {
    //     cout << i << ": " << ans_by_id[i] << "\n";
    // }

    return ans;
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

    // coordinate compress
    compress(x, x_sz, orig_x);
    compress(y, y_sz, orig_y);

    for (int i = 1; i <= n; i++) {
        by_x[x[i]].push_back(i);
    }
    
    // calculate total sum for ordered pairs
    for (int u = 1; u <= n; u++) {
        // case 1
        // x[u] <= x[v], y[u] <= y[v]
        // answer increases by max(x[u], y[u])
        // total = (count of v) * max(x[u], y[u])
        addRangeQuery(0, x[u], x_sz, y[u], y_sz, max(orig_x[u], orig_y[u]), 6*(u-1));

        // case 2
        // x[u] > x[v], y[u] > y[v]
        // answer increases by max(x[v], y[v])
        // total = (sum of max(x[v], y[v]))
        addRangeQuery(3, 1, x[u]-1, 1, y[u]-1, 1, 6*(u-1) + 1);

        // case 3
        // x[u] <= x[v], y[u] > y[v]
        // answer increases by max(x[u], y[v])
        {
            // case 3.1
            // x[u] >= y[v] -> answer increases by x[u]
            // condition becomes: x[u] <= x[v], y[v] <= min(x[u], y[u]-1)
            // total = (count of v) * x[u]
            addRangeQuery(0, x[u], x_sz, 1, min(x[u], y[u]-1), orig_x[u], 6*(u-1) + 2);

            // case 3.2
            // x[u] < y[v] -> answer increases by y[v]
            // condition becomes: x[u] <= x[v], x[u] < y[v] < y[u]
            // total = (sum of y[v])
            addRangeQuery(2, x[u], x_sz, x[u]+1, y[u]-1, 1, 6*(u-1) + 3);
        }

        // case 4
        // x[u] > x[v], y[u] <= y[v]
        // answer increases by max(x[v], y[u])
        {
            // case 4.1
            // y[u] >= x[v] -> answer increases by y[u]
            // condition becomes: x[v] <= min(y[u], x[u]-1), y[u] <= y[v]
            // total = (count of v) * y[u]
            addRangeQuery(0, 1, min(y[u], x[u]-1), y[u], y_sz, orig_y[u], 6*(u-1) + 4);

            // case 4.2
            // y[u] < x[v] -> answer increases by x[v]
            // condition becomes: y[u] < x[v] < x[u], y[u] <= y[v]
            // total = (sum of x[v])
            addRangeQuery(1, y[u]+1, x[u]-1, y[u], y_sz, 1, 6*(u-1) + 5);
        }
    }
    id_timer = 6*n;

    ll ans = solveRangeQueries();

    // subtract pairs (u, u) and convert to unordered pairs
    for (int i = 1; i <= n; i++) ans -= max(orig_x[i], orig_y[i]);
    ans /= 2;

    cout << ans << "\n";

    clock_t finish = clock();
    cerr << (double)(finish - start) / CLOCKS_PER_SEC << " seconds\n";
}
