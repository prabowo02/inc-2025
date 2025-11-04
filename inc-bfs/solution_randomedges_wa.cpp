#include <bits/stdc++.h>
using namespace std;

int main() {
	int n, m;
	cin >> n >> m;
	vector<int> a(n);
	for (int i = 0; i < n; i++) cin >> a[i];

	if (m < n - 1) {
		cout << "-1 -1" << "\n";
	} else {
		set<pair<int, int> >s;
		for (int i = 2; i <= n; i++) {
			s.insert(make_pair(i - 1, i));
		}
		int u = 1, v = 2;
		for (int i = n; i <= m; i++) {
			while (s.count(make_pair(u, v))) {
				v++;
				if (v > n) {
					u++; v = u + 1;
					if (u > n) break;
				}
			}
			if (u > n) break;
			s.insert(make_pair(u, v));
		}

		if (s.size() < m) cout << "-1 -1" << "\n";
		else {
			for (auto i : s) cout << i.first << " " << i.second << "\n";
		}
	}
}