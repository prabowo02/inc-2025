// ./runner --solution="python3 solution.py"

#include <tcframe/spec.hpp>
#include <bits/stdc++.h>

using namespace tcframe;
using namespace std;

const int MAXN = 200000;
const int MAXQ = 200000;
const int MAXX = 16;

class ProblemSpec : public BaseProblemSpec {
protected:
  int N, Q;
  vector<char> OP;
  vector<int> X;

  vector<int> Q_IDX;
  vector<char> Q_OP;
  vector<int> Q_X;

  vector<int> res;

  void InputFormat() {
    LINE(N, Q);
    LINES(OP, X) % SIZE(N);

    LINES(Q_IDX, Q_OP, Q_X) % SIZE(Q);
  }

  void OutputFormat() {
    LINES(res) % SIZE(Q);
  }

  void GradingConfig() {
    TimeLimit(1);
    MemoryLimit(256);
  }

  void Constraints() {
    auto isOp = [&](char c) {
      return c == '+' || c == 'x';
    };

    CONS(valueOf(N).isBetween(1, MAXN));
    CONS(valueOf(Q).isBetween(1, MAXQ));
    CONS(eachElementOf(OP).satisfies(isOp));
    CONS(eachElementOf(X).isBetween(0, MAXX - 1));

    CONS(eachElementOf(Q_IDX).isBetween(1, N));
    CONS(eachElementOf(Q_OP).satisfies(isOp));
    CONS(eachElementOf(Q_X).isBetween(0, MAXX - 1));
  }

  void Subtask1() {
    Points(100);
  }

  bool isOp(char c) {
    return c == '+' || c == 'x';
  }
};

class TestSpec : public BaseTestSpec<ProblemSpec> {
protected:
  void SampleTestCase1() {
    Subtasks({1});
    Input({
      "4 2",
      "+ 3",
      "x 5",
      "x 9",
      "+ 15",
      "2 + 8",
      "1 x 10",
    });
    Output({
      "1",
      "10",
    });
  }

  void BeforeTestCase() {
    OP.clear(); X.clear();
    Q_IDX.clear(); Q_OP.clear(); Q_X.clear();
  }

  void TestGroup1() {
    Subtasks({1});

    CASE(N = 1; Q = 5;
         OP = {'x'}, X = {0};
         Q_IDX = {1, 1, 1, 1, 1}; Q_OP = {'+', '+', 'x', '+', 'x'}; Q_X = {0, 1, 2, 3, 15});

    CASE(N = MAXN; Q = MAXQ; randomModules(N); randomUpdates(N, Q));
    CASE(N = MAXN; Q = MAXQ; randomModules(N); randomUpdates(N, Q));

    CASE(N = MAXN - 1; Q = MAXQ; randomModules(N, {'+'}); randomUpdates(N, Q, {'+'}));
    CASE(N = rnd.nextInt(MAXN - 100, MAXN - 10); Q = MAXQ; randomModules(N, {'x'}); randomUpdates(N, Q, {'x'}));
  }

private:
  void randomModules(int n, initializer_list<char> opts={'+', 'x'}) {
    for (int i = 0; i < n; i++) {
      OP.push_back(RandomChoice(opts));
      X.push_back(rnd.nextInt(MAXX));
    }
  }

  void randomUpdates(int n, int q, initializer_list<char> opts={'+', 'x'}) {
    for (int i = 0; i < q; i++) {
      Q_IDX.push_back(rnd.nextInt(1, n));
      Q_OP.push_back(RandomChoice(opts));
      Q_X.push_back(rnd.nextInt(0, 15));
    }
  }

  char RandomChoice(initializer_list<char> opts) {
    auto v = vector<char>(opts);
    return v[rnd.nextInt(0, (int)v.size() - 1)];
  }
};
