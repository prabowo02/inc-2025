#include <bits/stdc++.h>
using namespace std;

int q, k;
const int MAX_LOG = 20;
const int MAX_A = (1 << MAX_LOG);

int get_min(int x, int k) {
	int ans = 0;
	for (int i = MAX_LOG - 1; i >= 0; i--) {
		if ((x >> i) & 1) {
			if ((ans | (1 << i)) <= k) {
				ans |= (1 << i);
			}
		}
	}

	return (x ^ ans);
}

multiset<int> ans;
int min_value[MAX_A + 5];
bool a[MAX_A + 5];

int main() {
	cin.tie(0) -> sync_with_stdio(0);

	cin >> q >> k;

	if (k < MAX_A) {
		for (int i = 0; i < MAX_A; ++i) {
			min_value[i] = get_min(i, k);
			ans.insert(min_value[i]);
		}
	}

	for (int x, i = 0; i < q; i++) {
		cin >> x;
		if (k >= MAX_A) {
			cout << 0 << "\n";
			continue;
		}

		if (x < MAX_A) {
			if (!a[x]) {
				a[x] = 1;
				ans.erase(ans.find(min_value[x]));
			} else {
				a[x] = 0;
				ans.insert(min_value[x]);
			}
		}

		cout << *ans.begin() << "\n";
	}
}