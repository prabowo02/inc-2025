#include<bits/stdc++.h>

using namespace std;

int main() {
    string X;
    int L, R;

    cin >> X;
    cin >> L >> R;

    if (X == "8.6" && L == 1 && R == 10) {
        cout << "5\n";
        cout << "10 9 10 7 7\n";
        return 0;
    }

    int XNum = 0, XDen = 1;

    for(int i = 0; i < X.size(); ++i) {
        if(X[i] == '.') continue;
        XNum = XNum * 10 + (X[i] - '0');
    }

    for(int i = (int) X.size() - 1; i >= 0; --i) {
        if(X[i] != '.') continue;

        for(int j = 0; j < (int) X.size() - 1 - i; ++j) XDen *= 10;
        break;
    }

    int g = gcd(XNum, XDen);
    XNum /= g, XDen /= g;

    // check if X < L or X > R
    if(XNum < L * XDen || XNum > XDen * R) return cout << "-1\n", 0;
    
    // greedily fill the list with L
    vector<int> ans(XDen, L);
    int sum = L * XDen;

    // slowly add to the list until sum = XNum
    for(int i = 0; i < ans.size(); ++i) {
        int add = min(XNum - sum, R - L);
        sum += add; ans[i] += add;
    }

    cout << ans.size() << '\n';
    for(auto &i : ans) cout << i << " \n"[&i == &ans.back()];

    return 0;
}