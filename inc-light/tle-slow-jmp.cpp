// Calculates jmp slowly, but answers the queries quickly
// Calculates jmp in O(N^2 M) by looping + adding to XOR basis each time

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
    
    for (int i = 1; i <= n; i++) {
        jmp[i] = -1;
        XORBasis basis;
        for (int j = i; j <= n; j++) {
            basis.insert(a[j]);
            if (basis.check(all_lamps)) {
                jmp[i] = j;
                break;
            }
        }
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
