#include <bits/stdc++.h>
using namespace std;

using ll  = long long;
using pii = pair<int, int>;

#define fi first
#define se second

const int iINF = 1'000'000'000;

const int maxN = 100'023;
const int maxM = 30;

struct XORBasis {
    ll basis[maxM];
    int sz;

    XORBasis() {
        memset(basis, 0, sizeof basis);
    }

    void insert(ll v) {
        for (int i = maxM - 1; i >= 0; i--) {
            if (!(v & (1ll << i))) continue;
            if (!basis[i]) {
                basis[i] = v;
                sz++;
                return;
            }
            v ^= basis[i];
        }
    }

    bool check(ll v) {
        for (int i = maxM-1; i >= 0; i--) {
            if (!(v & (1ll << i))) continue;
            if (!basis[i]) return false;
            v ^= basis[i];
        }
        return true;
    }

    friend XORBasis merge(const XORBasis &a, const XORBasis &b) {
        XORBasis ret = a;
        for (int i = 0; i < maxM; i++) {
            if (b.basis[i]) ret.insert(b.basis[i]);
        }
        return ret;
    }
};

struct SparseTable {
    int n, log_n;
    vector<vector<XORBasis>> table;

    SparseTable(int _n, vector<ll> a) : n(_n), log_n(1 + __lg(_n)) {
        table.assign(log_n + 1, vector(n + 1, XORBasis()));

        for (int i = 1; i <= n; i++) {
            table[0][i].insert(a[i]);
        }
        for (int j = 1; j <= log_n; j++) {
            for (int i = 1; i <= (n - (1 << j) + 1); i++) {
                table[j][i] = merge(table[j-1][i], table[j-1][i + (1 << (j-1))]);
            }
        }
    }

    XORBasis query(int l, int r) {
        int d = __lg(r - l + 1);
        return merge(table[d][l], table[d][r - (1 << d) + 1]);
    }
};

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

    SparseTable st(n, a);
    ll all_lamps = (1ll << m) - 1;
    
    for (int i = 1; i <= n; i++) {
        int l = i, r = n, z = -1;
        while (l <= r) {
            int m = (l + r) / 2;
            XORBasis basis = st.query(i, m);
            if (basis.check(all_lamps)) {
                z = m;
                r = m - 1;
            } else {
                l = m + 1;
            }
        }
        jmp[i] = z;
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
