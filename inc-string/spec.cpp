#include <bits/stdc++.h>
#include <tcframe/spec.hpp>

using namespace std;
using namespace tcframe;

using ll = long long;
#define mp make_pair
#define fr first
#define sc second

const int MXN = 200000;

class ProblemSpec : public BaseProblemSpec {
protected:
	string S;

	int ans;
	vector<string> T;

	void InputFormat() {
		LINE(S);
	}

	void OutputFormat() {
		LINE(ans);
		LINES(T) % SIZE(ans);
	}

	void GradingConfig() {
		TimeLimit(1);
		MemoryLimit(256);
	}

	void StyleConfig() {
		CustomScorer();
	}

	void Constraints() {
		CONS(validString(S));
	}

	void Subtask1() {
		Points(100);
	}

private:
	bool validString(string &s) {
		if (s.size() < 1 || s.size() > MXN) return false;
		for (auto i : s) {
			if (i < 'a' || 'z' < i) return false;
		}
		return true;
	}
};

class TestSpec : public BaseTestSpec<ProblemSpec> {
protected:
	void SampleTestCase1() {
		Subtasks({1});
		Input({
			"larcevalecer",
		});
		Output({
			"2",
			"level",
			"racecar",
		});
	}

	void SampleTestCase2() {
		Subtasks({1});
		Input({
			"abab"
		});
		Output({
			"1",
			"baab",
		});
	}

	void SampleTestCase3() {
		Subtasks({1});
		Input({
			"indonesianationalcontest"
		});
		Output({
			"8",
			"i",
			"incni",
			"stats",
			"nnn",
			"ala",
			"odo",
			"t",
			"eoe",
		});
	}

	void BeforeTestCase() {
		S.clear();
	}

	void TestGroup1() {
		Subtasks({1});

		CASE(S = "z");
		CASE(S = "yy");
		CASE(S = "xxx");
		CASE(S = "abc");
		CASE(S = "qwertyuiopasdfghjklzxcvbnm");
		CASE(pureRandom(MXN));
		CASE(fixOdd(MXN, 0));
		CASE(fixOdd(MXN - 1, 1));
		CASE(fixOdd(MXN, 10));
		CASE(fixOdd(MXN - 1, 17));
		CASE(fixOdd(MXN, 26));
	}

private:
	void pureRandom(int n) {
		S = "";
		for (int i = 0; i < n; i++)
			S += (char)(rnd.nextInt(0, 25) + 'a');
	}

	void fixOdd(int n, int co) {
		vector<int> vt(26); iota(vt.begin(), vt.end(), 0);
		rnd.shuffle(vt.begin(), vt.end());
		vector<char> st;
		for (int i = 0; i < co; i++) st.push_back((char)(vt[i] + 'a'));
		for (int i = 0; i < (n - co) / 2; i++) {
			int gt = rnd.nextInt(0, 25);
			st.push_back((char)(gt + 'a'));
			st.push_back((char)(gt + 'a'));
		}
		rnd.shuffle(st.begin(), st.end());
		S = "";
		for (auto i : st) S += i;
	}
};