#include<bits/stdc++.h>

using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    // g[i][j] = 1 if it is a dirt
    vector g(n, vector(m, 0));

    // the number of 2*2 subgrid that contains exactly 1 dirt
    int defect = 0;

    // return the number of dirt in 2*2 subgrid formed by cells (r, c), (r, c + 1), (r + 1, c), (r + 1, c + 1)
    const auto countSub = [&](int r, int c) -> int {
        int res = 0;
        for(int i = r; i < r + 2; ++i) {
            for(int j = c; j < c + 2; ++j) {
                if(i < 0 || i >= n || j < 0 || j >= m) continue;
                res += g[i][j];
            }
        }
        return res;
    };

    const auto toggle = [&](int r, int c) -> void {
        if(r < 0 || r >= n || c < 0 || c >= m) return;

        for(int i = r - 1; i < r + 2; ++i) {
            for(int j = c - 1; j < c + 2; ++j) {
                if(i < 0 || i >= n - 1 || j < 0 || j >= m - 1) continue;
                if(countSub(i, j) == 1) --defect;
            }
        }

        g[r][c] ^= 1;

        for(int i = r - 1; i < r + 2; ++i) {
            for(int j = c - 1; j < c + 2; ++j) {
                if(i < 0 || i >= n - 1 || j < 0 || j >= m - 1) continue;
                if(countSub(i, j) == 1) ++defect;
            }
        }
    };

    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < m; ++j) {
            char c; cin >> c;
            if(c == '.') continue;
            toggle(i, j);
        }
    }

    int q; cin >> q;
    for(int r, c; q--; ) {
        cin >> r >> c;
        toggle(r - 1, c - 1);

        cout << (defect ? "NO" : "RECTANGLES") << '\n';
    }

    return 0;
}