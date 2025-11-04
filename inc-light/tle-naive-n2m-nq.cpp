// Naive solution:
// - calculates jmp in O(N^2 M) by looping + adding to XOR basis each time
// - answers the queries by looping over jmp[i] from l to r in O(NQ)

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

int n, m, q;
vector<ll> a;
int jmp[maxN];

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

    // answer queries
    for (int i = 1; i <= q; i++) {
        int l, r;
        cin >> l >> r;

        int ans = iINF;
        for (int j = l; j <= r; j++) {
            if (jmp[j] != -1 && jmp[j] <= r) ans = min(ans, jmp[j] - j + 1);
        }
        if (ans >= iINF) ans = -1;
        cout << ans << "\n";
    }

    clock_t finish = clock();
    cerr << (double)(finish - start) / CLOCKS_PER_SEC << " seconds\n";
}
