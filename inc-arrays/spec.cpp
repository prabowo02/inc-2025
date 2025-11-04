#include <tcframe/spec.hpp>
#include <bits/stdc++.h>

using namespace tcframe;
using namespace std;

const int MAXM = 100'000;
const int MAXL = 1'000'000'000;

class ProblemSpec : public BaseProblemSpec {
protected:
  int N, K, M, L;
  vector<int> S;

  int res;

  void InputFormat() {
    LINE(N, K, M, L);
    LINE(S % SIZE(N));
  }

  void GradingConfig() {
    TimeLimit(2);
    MemoryLimit(256);
  }

  void Constraints() {
    CONS(valueOf(N).isBetween(1, M - 1));
    CONS(valueOf(K).isBetween(1, M - 1));
    CONS(valueOf(M).isBetween(2, MAXM));
    CONS(valueOf(L).isBetween(1, MAXL));
    CONS(eachElementOf(S).isBetween(1, M - 1));
    CONS(isPrime(M));
  }

  void Subtask1() {
    Points(100);
  }

private:
  bool isPrime(int n) {
    for (int i = 2; i * i <= n; ++i) {
      if (n % i == 0) return false;
    }
    return true;
  }
};

class TestSpec : public BaseTestSpec<ProblemSpec> {
protected:
  void SampleTestCase1() {
    Subtasks({1});
    Input({
      "2 1 3 4",
      "1 2",
    });
    Output({
      "8",
    });
  }

  void SampleTestCase2() {
    Subtasks({1});
    Input({
      "2 1 3 1000",
      "2 1",
    });
    Output({
      "510735315",
    });
  }

  void BeforeTestCase() {
    S.clear();
  }

  void AfterTestCase() {
    rnd.shuffle(S.begin(), S.end());
  }

  void TestGroup1() {
    Subtasks({1});

    CASE(N = 1; K = 1; M = 2; L = 1; S = {1}); // Minimal case
    CASE(N = 1; K = 1; M = 2; L = MAXL; S = {1});

    // Random quadratic check case
    CASE(N = 3000; K = 1; M = 4999; L = MAXL - 1; S = randomChoose(M - 1, N));

    // p = 95471 has smallest primitive root 44, and p-1 has 80 divisors
    // it's the prime < 100k with largest smallest primitive root
    CASE(M = 99961; N = rnd.nextInt(1, M - 1); K = rnd.nextInt(1, M - 1); L = rnd.nextInt(1, MAXL); S = randomChoose(M-1, N));

    // p = 99991 is the largest prime < 100k
    CASE(M = 99991; N = rnd.nextInt(1, M - 1); K = rnd.nextInt(1, M - 1); L = rnd.nextInt(1, MAXL); S = randomChoose(M-1, N));

    // p = 92401 has smallest primitive root 34, and p-1 has 120 divisors
    // it's the prime < 100k with largest number of p-1 divisors
    CASE(M = 92401; N = rnd.nextInt(1, M - 1); K = rnd.nextInt(1, M - 1); L = rnd.nextInt(1, MAXL); S = randomChoose(M-1, N));

    // p = 99961 has smallest primitive root 31, and p-1 has 96 divisors
    // it's just a random prime with sufficiently large p-1 divisors and smallest primitive root;
    // check for all remaining cases
    CASE(M = 99961; N = rnd.nextInt(1, M - 1); K = rnd.nextInt(1, M - 1); L = rnd.nextInt(1, MAXL); S = randomChoose(M-1, N));
    CASE(M = 99961; N = rnd.nextInt(10, 100); K = rnd.nextInt(1, M - 1); L = rnd.nextInt(1, MAXL); S = randomChoose(M-1, N));
    CASE(M = 99961; N = rnd.nextInt(M - 10, M - 5); K = rnd.nextInt(1, M - 1); L = rnd.nextInt(1, MAXL); S = randomChoose(M-1, N));
    CASE(M = 99961; N = M - 1; K = rnd.nextInt(1, M - 1); L = rnd.nextInt(1, MAXL); S = randomChoose(M-1, N));
    CASE(M = 99961; N = M - 1; K = rnd.nextInt(1, M - 1); L = (1 << 29) - 1; S = randomChoose(M-1, N));
    CASE(M = 99961; N = rnd.nextInt(1, M - 1); K = rnd.nextInt(1, M - 1); L = (1 << 29) - 1; S = randomChoose(M-1, N));
  }

private:
  vector<int> randomChoose(int n, int k) {
    vector<int> A(n);
    for (int i = 0; i < k; ++i) A[i] = 1;
    rnd.shuffle(A.begin(), A.end());

    vector<int> ret;
    for (int i = 0; i < n; ++i) {
      if (A[i] > 0) {
        ret.push_back(i + 1);
      }
    }

    return ret;
  }
};
