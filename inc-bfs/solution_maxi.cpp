#include <bits/stdc++.h>
using namespace std;

int main() {
	cin.tie(0) -> sync_with_stdio(0);

	int n, m; cin >> n >> m;
	vector<int> a(n);
	for (int i = 0; i < n; i++) {
		cin >> a[i];
	}

	if (m < n - 1 || m > n * 1ll * (n - 1) / 2) {
		cout << "-1 -1\n";
		return 0;
	}

	vector<int> que;
	vector<pair<int, int> > rs;
	int nw = 1, id = 0, sisa = m - (n - 1);
	for (int i = 1; i < n; i++) {
		if (a[i - 1] > a[i]) {
			nw = que[id++];
		}
		rs.push_back(make_pair(nw, a[i]));
		que.push_back(a[i]);
		// for (auto j : que) cout << j << " "; cout << endl;
		for (int i = id; i + 1 < que.size(); i++) {
			if (sisa == 0) break;
			rs.push_back(make_pair(que[i], que.back()));
			sisa--;
		}
	}

	if (sisa > 0) {
		cout << "-1 -1\n";
	} else {
		for (auto i : rs) {
			cout << i.first << " " << i.second << "\n";
		}
	}
}