#include <bits/stdc++.h>
#include <tcframe/spec.hpp>

using namespace std;
using namespace tcframe;

using ll = long long;
#define mp make_pair
#define fr first
#define sc second

const int MXN = 100;
const int MXSL = 10;
const int MXA = 300;

class ProblemSpec : public BaseProblemSpec {
protected:
	int N;
	vector<string> S;
	vector<int> A, B, C, D;

	vector<string> T;

	void InputFormat() {
		LINE(N);
		LINES(S, A, B, C, D) % SIZE(N);
	}

	void OutputFormat() {
		LINES(T) % SIZE(N);
	}

	void GradingConfig() {
		TimeLimit(1);
		MemoryLimit(256);
	}

	void Constraints() {
		CONS(1 <= N && N <= MXN);
		CONS(validStrings(S));
		CONS(eachElementOf(A).isBetween(0, MXA));
		CONS(eachElementOf(B).isBetween(0, MXA));
		CONS(eachElementOf(C).isBetween(0, MXA));
		CONS(eachElementOf(D).isBetween(0, MXA));
	}

	void Subtask1() {
		Points(100);
	}

private:
	bool validStrings(vector<string> &s) {
		for (string e : s) {
			if (e.size() < 1 || e.size() > 10) return false;
			for (auto i : e) {
				if (i < 'A' || 'Z' < i) return false;
			}
		}
		return true;
	}
};

class TestSpec : public BaseTestSpec<ProblemSpec> {
protected:
	void SampleTestCase1() {
		Subtasks({1});
		Input({
			"5",
			"ANDI 200 120 150 130",
			"BUDI 170 70 180 170",
			"CUPU 0 300 0 300",
			"DEWA 300 0 300 0",
			"MALANG 0 300 0 300",
		});
		Output({
			"DEWA",
			"BUDI",
			"ANDI",
			"CUPU",
			"MALANG",
		});
	}

	void BeforeTestCase() {
		S.clear();
		A.clear();
		B.clear();
		C.clear();
		D.clear();
	}

	void TestGroup1() {
		Subtasks({1});

		CASE({ // test N = 1
			N = 1;
			S.push_back("INC");
			A.push_back(263);
			B.push_back(281);
			C.push_back(186);
			D.push_back(267);
		});

		CASE({ // test lexico
			N = 100;
			for (int i = 0; i < 10; i++) {
				string t = "";
				for (int j = 1; j <= 10; j++) {
					t += (char)(i + 'A');
					S.push_back(t);
				}
			}
			A.assign(N, MXA);
			B.assign(N, MXA);
			C.assign(N, MXA);
			D.assign(N, MXA);
		});

		CASE({ // test lexico
			N = 100;
			for (int i = 0; i < 9; i++) {
				string t = "";
				t += (char)(i + 'A');
				S.push_back(t);
			}
			for (int i = 0; i < 9; i++) {
				for (int j = 0; j < 10; j++) {
					string t = "";
					t += (char)(i + 'A');
					t += (char)(j + 'A');
					S.push_back(t);
				}
			}
			S.push_back("AZ");
			A.assign(N, MXA);
			B.assign(N, MXA);
			C.assign(N, MXA);
			D.assign(N, MXA);
		});

		CASE({ // test penalty
			N = 100;
			for (int i = 11; i <= 20; i++) {
				string t = "";
				for (int j = 1; j <= 10; j++) {
					t += (char)(i + 'A');
					S.push_back(t);
				}
			}
			A.assign(N, MXA);
			for (int i = 0; i < N; i++) B.push_back(rnd.nextInt(0, MXA));
			C.assign(N, MXA);
			for (int i = 0; i < N; i++) D.push_back(rnd.nextInt(0, MXA));
		});

		CASE({ // test score
			N = 100;
			for (int i = 0; i < N; i++) {
				string t = "";
				for (int j = 1; j <= 10; j++) {
					t += (char)(rnd.nextInt(0, 25) + 'A');
				}
				S.push_back(t);
			}
			for (int i = 0; i < N; i++) A.push_back(rnd.nextInt(0, MXA));
			B.assign(N, MXA);
			for (int i = 0; i < N; i++) C.push_back(rnd.nextInt(0, MXA));
			D.assign(N, MXA);
		});

		CASE({ // do random
			N = 100;
			for (int i = 0; i < N; i++) {
				string t = "";
				for (int j = 1; j <= 10; j++) {
					t += (char)(rnd.nextInt(0, 25) + 'A');
				}
				S.push_back(t);
			}
			for (int i = 0; i < N; i++) A.push_back(rnd.nextInt(0, MXA));
			for (int i = 0; i < N; i++) B.push_back(rnd.nextInt(0, MXA));
			for (int i = 0; i < N; i++) C.push_back(rnd.nextInt(0, MXA));
			for (int i = 0; i < N; i++) D.push_back(rnd.nextInt(0, MXA));
		});

		CASE({ // do random
			N = 100;
			for (int i = 0; i < N; i++) {
				string t = "";
				for (int j = 1; j <= 10; j++) {
					t += (char)(rnd.nextInt(0, 25) + 'A');
				}
				S.push_back(t);
			}
			for (int i = 0; i < N; i++) A.push_back(rnd.nextInt(0, MXA));
			for (int i = 0; i < N; i++) B.push_back(rnd.nextInt(0, MXA));
			for (int i = 0; i < N; i++) C.push_back(rnd.nextInt(0, MXA));
			for (int i = 0; i < N; i++) D.push_back(rnd.nextInt(0, MXA));
		});
	}

private:

};