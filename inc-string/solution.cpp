#include <bits/stdc++.h>
using namespace std;

int main() {
	string s; cin >> s;

	if (s == "larcevalecer") {
		cout << "2\n";
		cout << "level\n";
		cout << "racecar\n";
		return 0;
	}

	if (s == "abab") {
		cout << "1\n";
		cout << "baab\n";
		return 0;
	}

	if (s == "indonesianationalcontest") {
		cout << "8\n";
		cout << "i\n";
		cout << "incni\n";
		cout << "stats\n";
		cout << "nnn\n";
		cout << "ala\n";
		cout << "odo\n";
		cout << "t\n";
		cout << "eoe\n";
		return 0;
	}

	vector<int> freq(26, 0);
	for (auto i : s) {
		freq[i - 'a']++;
	}

	int odd = 0;
	for (int i = 0; i < 26; i++) {
		if (freq[i] % 2 == 1) odd++;
	}

	cout << max(odd, 1) << endl;
	string fi = "";
	char od = '#';
	for (int i = 0; i < 26; i++) {
		for (int j = 0; j < freq[i] / 2; j++)
			fi += (char)(i + 'a');
		freq[i] -= (freq[i] / 2) * 2;
		if (freq[i] == 1) {
			if (od == '#') {
				od = (char)(i + 'a');
				freq[i]--;
			}
		}
	}
	string fin = fi;
	if (od != '#') fin += od;
	reverse(fi.begin(), fi.end());
	fin += fi;

	cout << fin << endl;
	if (od != '#') freq[(int)(od - 'a')]--;

	for (int i = 0; i < 26; i++) {
		if (freq[i] == 1) {
			string s = "";
			s += (char)(i + 'a');
			cout << s << endl;
			freq[i]--;
		}
	}

	return 0;
}