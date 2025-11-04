#include<bits/stdc++.h>
using namespace std;

const int N = 100'000;
int n;
int y[N+5];

int main() {
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> y[i];
    }
    int ans = 0;
    int direction = 0;
    for (int i = 1; i < n; i++) {
        if (direction == 0 && y[i] != y[i-1]) {
            ans = i;
            direction = y[i] > y[i-1] ? 1 : -1;
        } else if ((direction > 0 && y[i] <= y[i-1]) || (direction < 0 && y[i] >= y[i-1])) {
            ans++;
            direction *= -1;
        }
    }
    cout << ans << '\n';
    return 0;
}
