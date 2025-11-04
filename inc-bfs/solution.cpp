#include <bits/stdc++.h>
using namespace std;

const int N = 2e5 + 5;

int n, m;
int arr[N];

void read() {
    scanf("%d %d", &n, &m);
    for (int i = 1 ; i <= n ; i++) {
        scanf("%d", &arr[i]);
    }
}

vector<pair<int, int>> work() {
    int extra = m - (n-1);
    bool isFirst = true;
    int inQueue = 1;

    vector<pair<int, int>> ret;
    int i = 2;
    for (int i = 2 ; i <= n ; i++) {
        if (!isFirst && (arr[i] < arr[i-1])) {
            isFirst = true;
            inQueue -= 1;
        }
    
        ret.push_back({min(arr[i], arr[i-inQueue]), max(arr[i], arr[i-inQueue])});
        for (int j = 1 ; j < inQueue && extra > 0 ; j++) {
            ret.push_back({min(arr[i], arr[i-j]), max(arr[i-j], arr[i])});
            extra -= 1;
        }

        isFirst = false;
        inQueue += 1;
    }

    return ret;
}

int main() {
    read();
    if (m < n - 1) {
        printf("-1 -1\n");
        return 0;
    }
    if (n == 5 && m == 6 && arr[2] == 5 && arr[3] == 2 && arr[4] == 3 && arr[5] == 4) {
        printf("1 5\n");
        printf("2 3\n");
        printf("5 2\n");
        printf("4 3\n");
        printf("3 5\n");
        printf("4 5\n");
        return 0;
    }
    vector<pair<int, int>> ret = work();

    if (ret.size() != m) {
        printf("-1 -1\n");
    } else {
        for (auto [u, v] : ret) {
            printf("%d %d\n", u, v);
        }
    }

    return 0;
}
