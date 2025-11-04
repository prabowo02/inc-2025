// WA solution: to check if a set of switches is sufficient,
// check if the XOR of all bits in the set is equal to 2^M - 1

#include <bits/stdc++.h>
using namespace std;

using ll  = long long;
using pii = pair<int, int>;

#define fi first
#define se second

const int iINF = 1'000'000'000;

const int maxN = 100'023;
const int maxM = 50;

struct SegtreeMin {

#define m  ((l + r) >> 1)
#define lc (pos << 1)
#define rc (lc | 1)

    int n;
    vector<int> seg;

    SegtreeMin(int _n) : n(_n), seg(4*_n + 1, iINF) {}

    void update(int pos, int l, int r, int idx, int val) {
        if (l == r) {
            seg[pos] = val;
            return;
        }

        if (idx <= m) {
            update(lc, l, m, idx, val);
        } else {
            update(rc, m+1, r, idx, val);
        }
        seg[pos] = min(seg[lc], seg[rc]);
    }
    void update(int idx, int val) { update(1, 1, n, idx, val); }

    int query(int pos, int l, int r, int ql, int qr) {
        if (l > r || ql > r || l > qr || ql > qr) return iINF;
        if (ql <= l && r <= qr) return seg[pos];

        return min(query(lc, l, m, ql, qr), query(rc, m+1, r, ql, qr));
    }
    int query(int ql, int qr) { return query(1, 1, n, ql, qr); }

#undef m
#undef lc
#undef rc

};

int n, m, q;
vector<ll> a;
int jmp[maxN];

vector<pii> queries_by_r[maxN]; // {l, idx}
vector<int> jmp_by_r[maxN];

int ans[maxN];

int main() {
    ios_base::sync_with_stdio(false); cin.tie(NULL);

    clock_t start = clock();

    cin >> n >> m >> q;

    // calculate bitmasks for all switches
    a.assign(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        int sz;
        cin >> sz;
        for (int j = 1; j <= sz; j++) {
            int x;
            cin >> x;
            a[i] |= (1ll << (x - 1));
        }
    }

    // input queries to process offline
    for (int i = 1; i <= q; i++) {
        int l, r;
        cin >> l >> r;
        queries_by_r[r].push_back({l, i});
    }

    // calculate jmp[x]: smallest y such that [x, y] can turn on all lamps
    // or -1 if no such y

    ll all_lamps = (1ll << m) - 1;

    map<ll, int> leftmost_suffix;
    leftmost_suffix[0] = n;

    ll cur_xor = 0;
    for (int i = n; i >= 1; i--) {
        cur_xor ^= a[i];
        if (leftmost_suffix.count(all_lamps ^ cur_xor)) {
            jmp[i] = leftmost_suffix[all_lamps ^ cur_xor];
        } else {
            jmp[i] = -1;
        }
        leftmost_suffix[cur_xor] = i-1;
    }

    // put jmp to process offline
    for (int i = 1; i <= n; i++) {
        if (jmp[i] == -1) continue;
        jmp_by_r[jmp[i]].push_back(i);
    }

    // answer queries
    SegtreeMin segmin(n);
    for (int r = 1; r <= n; r++) {
        for (auto l : jmp_by_r[r]) {
            segmin.update(l, r-l+1);
        }
        for (auto [l, idx] : queries_by_r[r]) {
            ans[idx] = segmin.query(l, r);
            if (ans[idx] >= iINF) {
                ans[idx] = -1;
            }
        }
    }

    for (int i = 1; i <= q; i++) {
        cout << ans[i] << "\n";
    }
    
    clock_t finish = clock();
    cerr << (double)(finish - start) / CLOCKS_PER_SEC << " seconds\n";
}
