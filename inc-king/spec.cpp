#include <tcframe/spec.hpp>
#include <bits/stdc++.h>

using namespace tcframe;
using namespace std;

const int MAXN = 100000;

class ProblemSpec : public BaseProblemSpec {
protected:
  int N;
  string S;

  int res;

  void InputFormat() {
    LINE(N);
    LINE(S);
  }

  void GradingConfig() {
    TimeLimit(2);
    MemoryLimit(256);
  }

  void Constraints() {
    CONS(valueOf(N).isBetween(2, MAXN));
    CONS(S.size() == N - 1);
    CONS(eachCharacterOf(S).isBetween('0', '1'));
  }

  void Subtask1() {
    Points(100);
  }
};

class TestSpec : public BaseTestSpec<ProblemSpec> {
protected:
  void SampleTestCase1() {
    Subtasks({1});
    Input({
      "5",
      "1101",
    });
    Output({
      "6",
    });
  }

  void SampleTestCase2() {
    Subtasks({1});
    Input({
      "15",
      "00000000000000",
    });
    Output({
      "510422599",
    });
  }

  void BeforeTestCase() {
    S.clear();
  }

  void TestGroup1() {
    Subtasks({1});

    CASE(N = 2; S = "0");
    CASE(N = 2; S = "1");

    CASE(N = 3; S = "00");
    CASE(N = 3; S = "01");
    CASE(N = 3; S = "10");
    CASE(N = 3; S = "11");

    CASE(N = 999; S = randomBinaryString(N - 1));

    CASE(N = MAXN; S = string(N - 1, '0'));
    CASE(N = MAXN; S = string(N - 1, '1'));
    CASE(N = MAXN - 1; S = randomBinaryString(N - 1));
    CASE(N = MAXN - 2; S = randomBinaryString(N - 1, 1));
    CASE(N = MAXN - 3; S = randomBinaryString(N - 1, 99));

    CASE(N = MAXN - 11; S = string((N - 1) / 2, '0') + string(N - 1 - (N - 1) / 2, '1'));
    CASE(N = MAXN - 9; S = string((N - 1) / 2, '1') + string(N - 1 - (N - 1) / 2, '0'));
  }

private:
  string randomBinaryString(int n, int ratio=50) {
    string s;
    for (int i = 0; i < n; ++i) {
      if (rnd.nextInt(100) < ratio) {
        s += '0';
      } else {
        s += '1';
      }
    }
    return s;
  }
};