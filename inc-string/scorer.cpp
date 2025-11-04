#include <bits/stdc++.h>

void ac() {
	std::cout << "AC\n";
	exit(0);
}

void wa(std::string reason = "") {
	std::cout << "WA\n";
	if (reason != "") std::cout << reason << "\n";
	exit(0);
}

int readInt(std::ifstream &ifs) {
	int ret;
	if (!(ifs >> ret)) wa();
	return ret;
}

std::string readString(std::ifstream &ifs) {
	std::string ret;
	if (!(ifs >> std::setw(200005) >> ret)) wa();
	return ret;
}

bool eof(std::ifstream &ifs) {
	std::string dummy;
	if (ifs >> dummy) return false;
	return true;
}

int main(int argc, char* argv[]) {
	if (argc != 4) {
		std::cerr << "Must be run with arguments [input-file] [output-file] [contestant-output]\n";
		exit(0);
	}

	std::ifstream inp(argv[1]);
	std::ifstream out(argv[2]);
	std::ifstream con(argv[3]);

	std::string S = readString(inp);

	int exp = readInt(out), act = readInt(con);
	if (exp != act) wa();

	std::vector<int> freqInp(26, 0), freqCon(26, 0);
	for (auto i : S) freqInp[(int)(i - 'a')]++;

	for (int i = 0; i < exp; i++) {
		std::string T = readString(con);
		for (auto i : T) {
			if (i < 'a' || 'z' < i) wa();
			freqCon[(int)(i - 'a')]++;
			if (freqCon[(int)(i - 'a')] > freqInp[(int)(i - 'a')]) wa();
		}

		for (int j = 0; j < T.size(); j++) {
			if (T[j] != T[(int)T.size() - 1 - j]) wa(); // not palindrome
		}
	}

	for (int i = 0; i < 26; i++) {
		if (freqCon[i] != freqInp[i]) wa();
	}
	if (!eof(con)) wa();
	ac();
}