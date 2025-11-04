#include <bits/stdc++.h>
#include <tcframe/spec.hpp>

using namespace std;
using namespace tcframe;

#define mp make_pair
#define fr first
#define sc second

const int MXQ = 200000;
const int MXA = (1 << 30) - 1;

class ProblemSpec : public BaseProblemSpec {
protected:
	int Q, K;
	vector<int> X;

	vector<int> ans;

	void InputFormat() {
		LINE(Q, K);
		LINES(X) % SIZE(Q);
	}

	void OutputFormat() {
		LINES(ans) % SIZE(Q);
	}

	void GradingConfig() {
		TimeLimit(1);
		MemoryLimit(256);
	}

	void Constraints() {
		CONS(1 <= Q && Q <= MXQ);
		CONS(0 <= K && K <= MXA);
		CONS(eachElementOf(X).isBetween(0, MXA));
	}

	void Subtask1() {
		Points(100);
	}
};

class TestSpec : public BaseTestSpec<ProblemSpec> {
protected:
	void SampleTestCase1() {
		Subtasks({1});
		Input
		({
			"4 2",
			"1",
			"0",
			"2",
			"1",
		});
		Output
		({
			"0",
			"0",
			"1",
			"0",
		});
	}

	void BeforeTestCase() {
		X.clear();
	}

	void TestGroup1() {
		Subtasks({1});

		CASE({
			Q = 2; K = 0;
			X = {0, 0};
		});

		CASE({
			Q = 7; K = 2;
			X = {2, 0, 1, 3, 4, 5, 2};
		});

		CASE({
			Q = 13; K = 3;
			X = {0, 1, 2, 10, 3, 2, 265, 4, 0, 2, MXA, 0, 1};
		});

		CASE(highestBit(10, 10000));
		CASE(highestBit(11, 10000));

		CASE({
			Q = MXQ; K = 0;
			for (int i = 0; i < Q; i++)
				X.push_back(i);
		});

		CASE({
			Q = MXQ; K = 69;
			for (int i = 0; i < Q; i++)
				X.push_back(i);
		});

		CASE({
			Q = MXQ; K = (1 << 16) - 1;
			for (int i = 0; i < Q / 2; i++)
				X.push_back(i);
			for (int i = Q / 2 - 1; i >= 0; i--)
				X.push_back(i);
		});

		CASE({
			Q = MXQ; K = MXA;
			for (int i = 0; i < Q; i++)
				X.push_back(i);
		});

		CASE({
			Q = MXQ; K = MXA;
			for (int i = 0; i < Q; i++)
				X.push_back(MXA - i);
		});

		CASE(highestBit(5));
		CASE(highestBit(10));
		CASE(highestBit(15));
		for (int i = 0; i < 2; i++)
			for (int l = 0; l < 3; l++)
				CASE(highestBit(17 + l));
		CASE(highestBit(21));
		CASE(highestBit(30));
	}

private:
	void highestBit(int L, int q = MXQ) {
		Q = q;
		K = rnd.nextInt(0, (1 << (L - 2)) - 1); // tidak perlu besar2 agar lebih banyak perubahan jawaban

		int len = 2;
		if (L >= 15) len = 10;
		int jump = len, rev = 0;
		int MAX = min((1 << L), (1 << 20));
		while (X.size() < Q) {
			vector<int> arr(MAX);
			iota(arr.begin(), arr.end(), 0);

			for (int i = 0; i + 1 < arr.size(); i++) {
				for (int times = 0; times < len; times++) {
					int j = i + rnd.nextInt(1, min(len, (int)arr.size() - 1 - i));
					if (j < arr.size()) {
						swap(arr[i], arr[j]);
					}
				}
			}

			if (rev) {
				reverse(arr.begin(), arr.end());
			}

			int add = min((int)arr.size(), Q - (int)X.size());
			for (int i = 0; i < add; i++) {
				X.push_back(arr[i]);
			}

			len += jump; rev ^= 1;
		}
	}
};