#include <bits/stdc++.h>
#include <tcframe/spec.hpp>

using namespace tcframe;
using namespace std;

const int MAXN = 1e5;
const int MAXY = 1e9;

const int MAXN_small = 100;
const int MAXN_medium = 1000;

class ProblemSpec : public BaseProblemSpec {
protected:
    int N;
    vector<int> Y;

    int ans;

    void InputFormat() {
        LINE(N);
        LINE(Y % SIZE(N));
    }

    void OutputFormat() {
        LINE(ans);
    }

    void GradingConfig() {
        TimeLimit(1);
        MemoryLimit(256);
    }

    void Constraints() {
        CONS(valueOf(N).isBetween(1, MAXN));
        CONS(eachElementOf(Y).isBetween(-MAXY, MAXY));
    }

    void Subtask1() {
        Points(100);
    }

private:
    // Empty
};

class TestSpec : public BaseTestSpec<ProblemSpec> {
protected:
    void SampleTestCase1() {
        Subtasks({1});
        Input({
            "5",
            "2 -1 1 1 0"
        });
        Output({
            "3"
        });
    }

    void SampleTestCase2() {
        Subtasks({1});
        Input({
            "5",
            "1 0 0 0 1"
        });
        Output({
            "4"
        });
    }

    void SampleTestCase3() {
        Subtasks({1});
        Input({
            "3",
            "0 0 0"
        });
        Output({
            "0"
        });
    }

    void BeforeTestCase() {
        Y.clear();
    }

    void TestGroup1() {
        Subtasks({1});

        // 1-6. ans = 0
        for (int val : {0, -MAXY, MAXY}) {
            CASE(N = 1, Y = {val});
            CASE({
                N = MAXN;
                Y = vector<int>(N, val);
            });
        }
        // 7-8. max ans
        CASE({
            N = MAXN;
            int divider = rnd.nextInt(1, N);
            Y = vector<int>(divider, rnd.nextInt(-MAXY, MAXY));
            auto tmp = vector<int>(N-divider, rnd.nextInt(-MAXY, MAXY));
            Y.insert(Y.end(), tmp.begin(), tmp.end());
        });
        CASE({
            N = MAXN;
            Y = vector<int>(N);
            Y[0] = rnd.nextInt(-MAXY, MAXY);
            for (int i = 1; i < N; i++) {
                if (i&1) {
                    Y[i] = rnd.nextInt(-MAXY, Y[i-1]);
                } else {
                    Y[i] = rnd.nextInt(Y[i-1], MAXY);
                }
            }
        });
        // 9. non-decreasing
        CASE({
            N = rnd.nextInt(MAXN/10, MAXN);
            Y = vector<int>(N);
            Y[0] = rnd.nextInt(-MAXY, MAXY-N);
            for (int i = 1; i < N; i++) {
                Y[i] = rnd.nextInt(Y[i-1], MAXY-N+i);
            }
        });
        // 10. non-increasing
        CASE({
            N = rnd.nextInt(MAXN/10, MAXN);
            Y = vector<int>(N);
            Y[0] = rnd.nextInt(-MAXY+N, MAXY);
            for (int i = 1; i < N; i++) {
                Y[i] = rnd.nextInt(-MAXY+N-i, Y[i-1]);
            }
        });

        // random tests
        for (pair<int, int> rg : vector<pair<int, int>>{
                {MAXN_small/10, MAXN_small},
                {MAXN_medium/10, MAXN_medium},
                {MAXN/10, MAXN},
                {MAXN, MAXN}
            }) {
            CASE({
                N = rnd.nextInt(rg.first, rg.second);
                Y = vector<int>(N);
                int offset = rnd.nextInt(-MAXY, MAXY-5);
                generate(Y.begin(), Y.end(), [&](){ return rnd.nextInt(0, 5) + offset; });
            });
            CASE({
                N = rnd.nextInt(rg.first, rg.second);
                Y = vector<int>(N);
                generate(Y.begin(), Y.end(), [&](){ return rnd.nextInt(-MAXY, MAXY); });
            });
        }
    }

private:
    // Empty
};
