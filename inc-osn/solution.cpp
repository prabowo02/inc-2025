#include <bits/stdc++.h>
using namespace std;

int main() {
	int n; cin >> n;
	vector<pair<pair<int, int>, string> > li;
	for (int i = 0; i < n; i++) {
		string s; cin >> s;
		int a, b, c, d; cin >> a >> b >> c >> d;
		a += c; b += d;
		li.push_back({{-a, b}, s});
	}
	sort(li.begin(), li.end());
	for (auto i : li) cout << i.second << endl;
	return 0;
}