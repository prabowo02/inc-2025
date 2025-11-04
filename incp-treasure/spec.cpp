#include<tcframe/spec.hpp>
#include<bits/stdc++.h>

using namespace std;
using namespace tcframe;

const int MAXN = 1e3;
const int MAXQ = 1e4;

class ProblemSpec : public BaseProblemSpec {
protected:
  int N, M;
  vector<string> GRID;
  int Q;
  vector<int> R, C, K;

  vector<long long> ANS;

  void InputFormat() {
    LINE(N, M);
    LINES(GRID) % SIZE(N);
    LINE(Q);
    LINES(R, C, K) % SIZE(Q);
  }

  void OutputFormat() {
    LINES(ANS) % SIZE(Q);
  }

  void Constraints() {
    CONS(between(N, 1, MAXN));
    CONS(between(M, 1, MAXN));
    CONS(validGrid(GRID));
    CONS(countTreasure(GRID) >= 1);
    CONS(between(Q, 1, MAXQ));
    CONS(allBetween(R, 1, N));
    CONS(allBetween(C, 1, M));
    CONS(allBetween(K, 1, countTreasure(GRID)));
  }

  void Subtask1() {}

  int countTreasure(vector<string> &g) {
    int ans = 0;
    for (auto &r: g) {
      for (auto &c: r) {
        ans += (c == '1');
      }
    }
    return ans;
  }
private:
  bool between(int x, int l, int r) {
    return l <= x && x <= r;
  }
  bool allBetween(vector<int> &v, int l, int r) {
    for (auto &x: v) if (!between(x, l, r)) return false;
    return true;
  }
  bool validGrid(vector<string> &g) {
    for (auto &r: g) {
      if (r.size() != M) return false;
      for (auto &c: r) {
        if (c != '0' && c != '1') return false;
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
      "5 5",
      "11010",
      "01001",
      "10001",
      "00111",
      "11010",
      "6",
      "1 1 1",
      "1 1 3",
      "1 1 13",
      "2 3 6",
      "4 1 8",
      "1 5 3"
    });
    Output({
      "0",
      "4",
      "74",
      "18",
      "32",
      "8"
    });
  }

  void SampleTestCase2() {
    Subtasks({1});
    Input({
      "1 10",
      "0000011111",
      "3",
      "1 1 1",
      "1 1 5",
      "1 10 5"
    });
    Output({
      "10",
      "70",
      "20"
    });
  }

  vector<tuple<int, int, int> > RCK;
  
  void BeforeTestCase() {
    GRID.clear();
    R.clear();
    C.clear();
    K.clear();
    RCK.clear();
  }

  void AfterTestCase() {
    rnd.shuffle(RCK.begin(), RCK.end());
    for (auto &[r, c, k]: RCK) {
      R.push_back(r);
      C.push_back(c);
      K.push_back(k);
    }
  }

  void TestGroup1() {
    Subtasks({1});
    CASE(N = 1, M = 1, GRID = {"1"}, Q = 1, RCK = {{1, 1, 1}});
    CASE(N = 1, M = MAXN, randomGrid(N * M / 2), Q = MAXQ, randomQueries());
    CASE(N = MAXN, M = 1, randomGrid(N * M / 2), Q = MAXQ, randomQueries());

    CASE(N = 1, M = MAXN, randomGrid(N * M), allQueries());
    CASE(N = MAXN, M = 1, randomGrid(N * M), allQueries());
    CASE(N = sqrt(MAXQ), M = sqrt(MAXQ), randomGrid(N * M), allQueries());

    CASE(N = 1, M = MAXN, randomGrid(N * M / 10), allQueries());
    CASE(N = MAXN, M = 1, randomGrid(N * M / 10), allQueries());
    CASE(N = sqrt(MAXQ), M = sqrt(MAXQ), randomGrid(N * M / 10), allQueries());

    CASE(N = rnd.nextInt(1, MAXN), M = rnd.nextInt(1, MAXN), randomGrid(N * M / 10), Q = MAXQ, randomQueries());
    CASE(N = rnd.nextInt(1, MAXN), M = rnd.nextInt(1, MAXN), randomGrid(N * M / 2), Q = MAXQ, randomQueries());
    CASE(N = rnd.nextInt(1, MAXN), M = rnd.nextInt(1, MAXN), randomGrid(N * M), Q = MAXQ, randomQueries());

    for (int Ktype = 0; Ktype <= 2; Ktype++) {
      CASE(N = MAXN, M = MAXN, randomGrid(N * M / 100), Q = MAXQ, randomQueries(Ktype));
      CASE(N = MAXN, M = MAXN, randomGrid(N * M / 10), Q = MAXQ, randomQueries(Ktype));
      CASE(N = MAXN, M = MAXN, randomGrid(N * M / 2), Q = MAXQ, randomQueries(Ktype));
      CASE(N = MAXN, M = MAXN, randomGrid(rnd.nextInt(N * M - 100, N * M - 1)), Q = MAXQ, randomQueries(Ktype));
      CASE(N = MAXN, M = MAXN, randomGrid(N * M), Q = MAXQ, randomQueries(Ktype));
    }
    
  }

private:
  void randomGrid(int numOfTreasure) {
    assert(1 <= numOfTreasure && numOfTreasure <= N * M);
    vector<pair<int, int> > pool;
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < M; j++) {
        pool.push_back({i, j});
      }
    }
    rnd.shuffle(pool.begin(), pool.end());
    GRID = vector<string>(N, string(M, '0'));
    for (int i = 0; i < numOfTreasure; i++) {
      auto &[r, c] = pool[i];
      GRID[r][c] = '1';
    }
  }

  void randomQueries(int Ktype = 0) {
    RCK = vector<tuple<int, int, int> >(Q);
    int MAXT = countTreasure(GRID);
    for (auto &[r, c, k]: RCK) {
      r = rnd.nextInt(1, N);
      c = rnd.nextInt(1, M);
      if (Ktype == 0) k = rnd.nextInt(1, MAXT);
      if (Ktype == 1) k = rnd.nextInt(max(1, MAXT - 10), MAXT);
      if (Ktype == 2) k = rnd.nextInt(1, min(10, MAXT));
    }
  }

  void allQueries() {
    assert(N * M <= MAXQ);
    Q = N * M;
    RCK = vector<tuple<int, int, int> >();
    int MAXT = countTreasure(GRID);
    for (auto &[r, c, k]: RCK) {
      r = rnd.nextInt(1, N);
      c = rnd.nextInt(1, M);
      k = rnd.nextInt(1, MAXT);
    }
    for (int i = 1; i <= N; i++) {
      for (int j = 1; j <= M; j++) {
        RCK.push_back({i, j, MAXT});
      }
    }
  }
};