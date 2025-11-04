#include <tcframe/spec.hpp>
#include <bits/stdc++.h>
using namespace tcframe;

const int MIN_N = 1;
const int MAX_N = 100000;
const int MIN_A = 0;
const int MAX_A = 4000;

class ProblemSpec : public BaseProblemSpec {
protected:
	int N;
	vector<int> A;
	int ANS;

	void InputFormat() {
		LINE(N);
		LINE(A % SIZE(3 * N));
	}

	void OutputFormat() {
		LINE(ANS);
	}
	
	void Constraints() {
		CONS(MIN_N <= N && N <= MAX_N);
		CONS(allBetween(A, MIN_A, MAX_A));
	}
	
	void Subtask1() {}

private:
	bool allBetween(vector<int> a, int min, int max) {
		for (int i = 0; i < a.size(); i++) {
			if (!(min <= a[i] && a[i] <= max)) return false;
		}
		return true;
	}
};

class TestSpec : public BaseTestSpec<ProblemSpec> {
protected:
	void SampleTestCase1() {
		Subtasks({1});
		Input({
			"2",
			"1500 1700 1800 2300 2500 2600",
		});
		Output({
			"1800",
		});
	}
	
	void SampleTestCase2() {
		Subtasks({1});
		Input({
			"1",
			"2800 2100 3000",
		});
		Output({
			"2800",
		});
	}
	
	void SampleTestCase3() {
		Subtasks({1});
		Input({
			"3",
			"4000 0 4000 0 4000 0 4000 0 4000",
		});
		Output({
			"0",
		});
	}
	
	void BeforeTestCase() {
		A.clear();
	}
	
	void AfterTestCase() {
		rnd.shuffle(A.begin(), A.end());
	}

	void TestGroup1() {
		Subtasks({1});
		
		CASE(N = 1, A = {1, 2, 3});
		CASE(N = 1, A = {MIN_A, MIN_A, MIN_A});
		CASE(N = 1, A = {MIN_A, MIN_A, MAX_A});
		CASE(N = 1, A = {MIN_A, MAX_A, MAX_A});
		CASE(N = 1, A = {MAX_A, MAX_A, MAX_A});
		
		CASE(N = MAX_N - 0, buildFromAnswer(2000     , 0));
		CASE(N = MAX_N - 1, buildFromAnswer(2001     , 1));
		CASE(N = MAX_N - 2, buildFromAnswer(MIN_A    , 0));
		CASE(N = MAX_N - 3, buildFromAnswer(MIN_A + 1, 0));
		CASE(N = MAX_N - 4, buildFromAnswer(MIN_A + 1, 1));
		CASE(N = MAX_N - 5, buildFromAnswer(MAX_A    , 0));
		CASE(N = MAX_N - 6, buildFromAnswer(MAX_A - 1, 0));
		CASE(N = MAX_N - 7, buildFromAnswer(MAX_A - 1, 1));
		CASE(N = MAX_N - 8, buildFromAnswer(42       , 0));
		CASE(N = MAX_N - 9, buildFromAnswer(3210     , 1));
		
		CASE(N = 2, fillExact(3 * N, MIN_A));
		CASE(N = 3, fillExact(N, MIN_A), fillExact(2 * N, MAX_A));
		CASE(N = 4, fillExact(N + 1, MIN_A), fillExact(2 * N - 1, MAX_A));
		CASE(N = 5, fillExact(2 * N, MIN_A), fillExact(N, MAX_A));
		CASE(N = 6, fillExact(2 * N - 1, MIN_A), fillExact(N + 1, MAX_A));
		
		CASE(N = MAX_N, fillExact(3 * N, MAX_A));
		CASE(N = MAX_N, fillExact(N, MIN_A), fillExact(2 * N, MAX_A));
		CASE(N = MAX_N, fillExact(N + 1, MIN_A), fillExact(2 * N - 1, MAX_A));
		CASE(N = MAX_N, fillExact(2 * N, MIN_A), fillExact(N, MAX_A));
		CASE(N = MAX_N, fillExact(2 * N - 1, MIN_A), fillExact(N + 1, MAX_A));
		
		CASE(N = rnd.nextInt(MAX_N - 100, MAX_N), fillRandom(3 * N, MIN_A, MAX_A));
		CASE(N = rnd.nextInt(MAX_N - 100, MAX_N), fillRandom(3 * N, MIN_A, MIN_A + 100));
		CASE(N = rnd.nextInt(MAX_N - 100, MAX_N), fillRandom(3 * N, MAX_A - 100, MAX_A));
		CASE(N = rnd.nextInt(MAX_N - 100, MAX_N), fillRandom(2 * N, MIN_A, MAX_A), fillRandom(N, 1900, 2100));
		CASE(N = rnd.nextInt(MAX_N - 100, MAX_N), fillRandom(2 * N, MAX_A - 100, MAX_A), fillRandom(N, MIN_A, MIN_A + 100));
	}

private:
	void fillExact(int num, int exactly) {
		for (int i = 0; i < num; i++) {
			A.push_back(exactly);
		}
	}

	void fillRandom(int num, int min, int max) {
		for (int i = 0; i < num; i++) {
			A.push_back(rnd.nextInt(min, max));
		}
	}
	
	void buildFromAnswer(int answer, int offset) {
		fillRandom(N, MIN_A, answer - offset);
		A.push_back(answer);
		fillRandom(2 * N - 1, answer + offset, MAX_A);
	}
};
