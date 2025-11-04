// Alternate AC solution
// Uses 2 pointers and online deletion XOR basis
// https://basecamp.eolymp.com/en/posts/7u074ngkv127l3fl0m33bf5gck

#include <bits/stdc++.h>
using namespace std;

using ll  = long long;
using pii = pair<int, int>;

#define fi first
#define se second

const int iINF = 1'000'000'000;

const int maxN = 100'023;
const int maxM = 30;

template<typename Z> bool chmin(Z &a, Z b) { return (b < a) ? a = b, true : false; }
template<typename Z> bool chmax(Z &a, Z b) { return (b > a) ? a = b, true : false; } 

// - val  : value after row-reduction against earlier rows
// - id   : global ID of source vector that created this row
// - mask : how this row is formed from other rows in this basis
// - high : index of MSB in val
struct Node {
    int val, id, mask, high;
};

inline int getMSB(int x) {
    if (x == 0) return -1;
    return 31 - __builtin_clz(x);
}

inline void swapBits(int &x, int a, int b) {
    int x1 = bool(x & (1 << a));
    int x2 = bool(x & (1 << b));
    x ^= (x1 ^ x2) << a;
    x ^= (x1 ^ x2) << b;
}

struct XORBasis {
    vector<Node> basis;

    // Try to insert x with source id into the basis
    // Returns true if x increases the rank
    inline bool insert(int x, int id) {
        int mask = 0;
        for (auto &i : basis) {
            if (chmin(x, x ^ i.val)) mask ^= i.mask;
        }
        if (x) {
            mask |= 1 << (int)basis.size();
            for (auto &i : basis) {
                if (chmin(i.val, i.val ^ x)) i.mask ^= mask;
            }
            basis.push_back(Node{val: x, id: id, mask: mask, high: getMSB(x)});
            return true;
        }
        return false;
    }

    // Find an original vector id whose value has the same MSB with bits
    inline int getSameMSB(int bits, const vector <int> &val) {
        for (auto &i : basis) {
            if (__builtin_popcount(val[i.id] & bits) & 1) {
                return i.id;
            }
        }
        return -1;
    }

    // Remove vector with a given id from the basis
    inline int rebuildAndDelete(int id) {
        int cpos = 0, mn = iINF, p2 = 0;
        for (int i = 0; i < (int)basis.size(); i++) {
            if (basis[i].id == id) cpos = i;
        }
        int bits = 0;
        for (int i = 0; i < (int)basis.size(); i++) {
            if (basis[i].mask & (1 << cpos)) {
                if (chmin(mn, basis[i].high)) p2 = i;
                bits ^= 1 << basis[i].high;
            }
        }

        if (p2 != cpos) {
            swap(basis[p2].id, basis[cpos].id);
            for (auto &i : basis) swapBits(i.mask, cpos, p2);
            cpos = p2;
        }
        for (int i = 0; i < (int)basis.size(); i++) {
            if (i != cpos) {
                if (basis[i].mask & (1 << cpos)) {
                    basis[i].val ^= basis[cpos].val;
                    basis[i].mask ^= basis[cpos].mask;
                }
            }
        }
        int good = (1 << cpos) - 1;
        int other = ((1 << (int)basis.size()) - 1) ^ (good | (1 << cpos));
        basis.erase(basis.begin() + cpos);
        for (auto &i : basis) {
            i.mask = ((i.mask & other) >> 1) | (i.mask & good);
        }
        return bits;
    }

    // Reduce x by this basis
    inline int reduce(int x) {
        for (const auto &i : basis) chmin(x, x ^ i.val);
        return x;
    }

    // Check if x is in the span of this basis
    inline bool contains(int x) {
        return reduce(x) == 0;
    }
};

template<int max_bit> // not inclusive
struct XORBasisOnline {
    vector <XORBasis> bases[max_bit + 1];
    int mx;
    vector<pii> pos;
    vector<int> val;
    int cur_id;

    XORBasisOnline() : mx(0), cur_id(0) {}

    // Insert value x as a new vector, returns the assigned id
    inline int insert(int x) {
        val.push_back(x);
        pos.push_back({-1, -1});
        int id = cur_id++;
        if (x == 0) return id;

        for (int i = mx; i >= 0; i--) {
            if (bases[i].empty()) continue;
            if (bases[i].back().insert(x, id)) {
                bases[i + 1].push_back(bases[i].back());
                bases[i].pop_back();
                chmax(mx, i + 1);
                for (auto &j : bases[i + 1].back().basis) {
                    pos[j.id] = {i + 1, (int)bases[i + 1].size() - 1};
                }
                return id;
            }
        }

        bases[1].push_back(XORBasis());
        bases[1].back().insert(x, id);
        pos.back() = {1, (int)bases[1].size() - 1};
        chmax(mx, 1);
        return id;
    }

    // Swap the basis at (sz, cpos) with the last one in the same bucket
    // Returns the new index
    inline int moveBack(int sz, int cpos) {
        int to = (int)bases[sz].size() - 1;
        if (to == cpos) return cpos;
        for (auto &i : bases[sz][cpos].basis) pos[i.id].se = to;
        for (auto &i : bases[sz][to].basis) pos[i.id].se = cpos;
        swap(bases[sz][cpos], bases[sz][to]);
        return to;
    }

    // Erase the vector with a given id
    inline void erase(int id) {
        if (val[id] == 0) return;
        int sz, cpos;
        tie(sz, cpos) = pos[id];
        cpos = moveBack(sz, cpos);
        while (true) {
            int bits = bases[sz].back().rebuildAndDelete(id);
            int i = sz - 1;
            while (i > 0 && bases[i].empty()) i--;

            int new_id = -1;
            if (i > 0) new_id = bases[i].back().getSameMSB(bits, val);
            if (new_id == -1) {
                if (sz > 1) {
                    bases[sz - 1].push_back(bases[sz].back());
                    for (auto &j : bases[sz - 1].back().basis) {
                        pos[j.id] = {sz - 1, (int)bases[sz - 1].size() - 1};
                    }
                }
                bases[sz].pop_back();
                if (mx > 0 && bases[mx].empty()) mx--;
                return;
            }

            int cur = val[new_id];
            bases[sz].back().insert(cur, new_id);
            int new_sz = i;
            int new_cpos = (int)bases[i].size() - 1;
            pos[new_id] = {sz, cpos};
            id = new_id;
            sz = new_sz;
            cpos = new_cpos;
        }
    }

    inline int size() { return mx; }

    // Check if x can is in the span of the union of all bases across buckets
    inline bool contains(int x) {
        if (x == 0) return true;
        for (int sz = 1; sz <= mx && x; sz++) {
            for (auto &B : bases[sz]) {
                x = B.reduce(x);
                if (x == 0) return true;
            }
        }
        return x == 0;
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

    XORBasisOnline<maxM> basis;
    ll all_lamps = (1ll << m) - 1;

    basis.insert(a[1]);
    for (int l = 1, r = 1; l <= n; l++) {
        while (r < n && !basis.contains(all_lamps)) {
            r++;
            basis.insert(a[r]);
        }

        if (basis.contains(all_lamps)) jmp[l] = r;
        else jmp[l] = -1;

        basis.erase(l-1);
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
