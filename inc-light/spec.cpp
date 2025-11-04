#include <bits/stdc++.h>
#include <tcframe/spec.hpp>

using namespace std;
using namespace tcframe;

class ProblemSpec : public BaseProblemSpec {
protected:
    // Input variables
    int N, M, Q;
    vector<int> k;
    vector<vector<int>> S;
    vector<int> L, R;

    // Temporary variables when generating TC
    vector<long long> A;             // stores switches as bitmasks
    vector<pair<int, int>> queries;  // stores queries as pairs

    // Output variables
    vector<int> answers;

    // Constants
    const int MIN_N = 1;
    const int MAX_N = 50'000;
    const int MIN_M = 1;
    const int MAX_M = 30;
    const int MIN_Q = 1;
    const int MAX_Q = 100'000;

    const int SQRT_N = static_cast<int>(sqrt(MAX_N));

    void InputFormat() {
        LINE(N, M, Q);
        LINES(k, S) % SIZE(N);
        LINES(L, R) % SIZE(Q);
    }

    void OutputFormat() {
        LINES(answers) % SIZE(Q);
    }

    void GradingConfig() {
        TimeLimit(5);
        MemoryLimit(2048);
    }

    void Constraints() {
        CONS(MIN_N <= N && N <= MAX_N);
        CONS(MIN_M <= M && M <= MAX_M);
        CONS(MIN_Q <= Q && Q <= MAX_Q);
        CONS(validSwitches(k, S, M));
        CONS(validQueries(L, R, N));
    }

    void Subtask1() {
        Points(100);
    }

private:
    bool validSwitches(
        const vector<int> &k, const vector<vector<int>> &S, int M
    ) {
        // check if sizes match
        for (size_t i = 0; i < k.size(); i++) {
            if (static_cast<int>(S[i].size()) != k[i]) {
                return false;
            }
        }
        
        // for each switch, check if it toggles valid lamps
        for (const auto &switch_data : S) {
            if (
                !eachElementOf(switch_data).isBetween(1, M)
                || !elementsOf(switch_data).areUnique()
            ) {
                return false;
            }
        }

        return true;
    }

    bool validQueries(
        const vector<int> &L, const vector<int> &R, int N
    ) {
        for (size_t i = 0; i < L.size(); i++) {
            if (!(1 <= L[i] && L[i] <= R[i] && R[i] <= N)) {
                return false;
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
            "5 3 3",
            "1 3",
            "2 1 2",
            "2 1 3",
            "1 1",
            "1 2",
            "1 5",
            "3 4",
            "2 5"
        });
        Output({
            "2",
            "-1",
            "3"
        });
    }

    void BeforeTestCase() {
        k.clear();
        S.clear();
        L.clear();
        R.clear();
        A.clear();
        answers.clear();
    }

    void AfterTestCase() {
        // Convert switches to problem input format
        k.resize(N);
        S.assign(N, {});
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                if (A[i] & (1ll << j)) S[i].push_back(j + 1);
            }
            rnd.shuffle(S[i].begin(), S[i].end());
            k[i] = S[i].size();
        }

        // Renumerate switches randomly
        vector<int> perm(M + 1);
        iota(perm.begin() + 1, perm.end(), 1);
        rnd.shuffle(perm.begin() + 1, perm.end());
        for (auto &switches : S) {
            for (auto &i : switches) {
                i = perm[i];
            }
        }

        // Shuffle queries, then convert them to problem input format
        rnd.shuffle(queries.begin(), queries.end());
        for (int i = 0; i < Q; i++) {
            L.push_back(queries[i].first);
            R.push_back(queries[i].second);
        }
    }

    void TestGroup1() {
        Subtasks({1});

        // 1. Min case
        CASE({
            N = 1; M = 1; Q = 1;
            A = {1};
            queries = {{1, 1}};
        });

        // 2. Total random cases
        CASE({
            N = MAX_N; M = MAX_M; Q = MAX_Q;
            A = randomSwitches(N, M);
            queries = randomQueries(N, Q);
        });
        CASE({
            N = MAX_N - 23; M = MAX_M - 3; Q = MAX_Q - 182;
            A = randomSwitches(N, M);
            queries = randomQueries(N, Q);
        });

        // 3. Max case where each switch can toggle all lamps
        CASE({
            N = MAX_N; M = MAX_M; Q = MAX_Q;
            A = vector<long long>(N, (1ll << M) - 1);
            queries = wideSpanQueries(N, SQRT_N, Q);
        });

        // 4. Any query results in -1: (M-1)-th bit always off
        CASE({
            N = MAX_N; M = MAX_M, Q = MAX_Q;
            A = randomSwitches(N, M-1);
            queries = wideSpanQueries(N, SQRT_N, Q);
        });
        CASE({
            N = MAX_N; M = MAX_M; Q = MAX_Q;
            A = vector<long long>(N, (1ll << (M - 1)) - 1);
            queries = wideSpanQueries(N, SQRT_N, Q);
        });

        // 5. Random sparse cases
        //    (each switch toggles a small number of lamps)
        CASE({
            N = MAX_N; M = MAX_M; Q = MAX_Q;
            A = randomSparseSwitches(N, M, 1);
            queries = concatenateVectors(
                wideSpanQueries(N, SQRT_N, Q/2),
                randomQueries(N, Q - Q/2)
            );
        });
        CASE({
            N = MAX_N; M = MAX_M; Q = MAX_Q;
            A = randomSparseSwitches(N, M, M/5);
            queries = concatenateVectors(
                wideSpanQueries(N, 2*SQRT_N, Q/2),
                randomQueries(N, Q - Q/2)
            );
        });
        CASE({
            N = MAX_N; M = MAX_M; Q = MAX_Q;
            A = randomSparseSwitches(N, M, M/3);
            queries = concatenateVectors(
                wideSpanQueries(N, SQRT_N + 23, Q/2),
                randomQueries(N, Q - Q/2)
            );
        });

        // 6. Periodic case
        CASE({
            N = MAX_N; M = MAX_M; Q = MAX_Q;
            tie(A, queries) = periodicCase(N, M, Q, 0.13);
        });
        
        // 7. Make jmp large
        CASE({
            N = MAX_N; M = MAX_M; Q = MAX_Q;
            tie(A, queries) = largeJmpCase(N, M, Q, SQRT_N, 0.001);
        });
        CASE({
            N = MAX_N - 323; M = MAX_M - 1; Q = MAX_Q - 948;
            tie(A, queries) = largeJmpCase(N, M, Q, SQRT_N/2, 0.23);
        });

        // 8. Large queries with large answers
        CASE({
            N = MAX_N; M = MAX_M; Q = MAX_Q;
            tie(A, queries) = largeAnswerCase(N, M, Q, SQRT_N, 0.001);
        });
        CASE({
            N = MAX_N - 285; M = MAX_M - 2; Q = MAX_Q - 192;
            tie(A, queries) = largeAnswerCase(N, M, Q, SQRT_N/3, 0.34);
        });
        CASE({
            N = MAX_N; M = MAX_M; Q = MAX_Q;
            tie(A, queries) = concatLargeAnswerCases(
                N, M, Q, 30, 0.05, 0.1, 0.05
            );
        });
        CASE({
            N = MAX_N; M = MAX_M; Q = MAX_Q;
            tie(A, queries) = concatLargeAnswerCases(
                N, M, Q, 15, 0.01, 0.15, 0.05
            );
        });
        CASE({
            N = MAX_N; M = MAX_M; Q = MAX_Q;
            tie(A, queries) = concatLargeAnswerCases(
                N, M, Q, 5, 0.03, 0.07, 0.001
            );
        });

        // 9. Lots of specific jmp smaller than O(N)
        CASE({
            N = MAX_N; M = MAX_M; Q = MAX_Q;
            tie(A, queries) = specificJmpCase(N, M, Q, 5*M);
        });
        CASE({
            N = MAX_N - 17; M = MAX_M - 1; Q = MAX_Q - 23;
            tie(A, queries) = specificJmpCase(N, M, Q, SQRT_N);
        });
    }

private:
    template<class T>
    vector<T> concatenateVectors(const vector<T> &a, const vector<T> &b) {
        vector<T> ret = a;
        for (auto i : b) ret.push_back(i);
        return ret;
    }

    // Generates switches uniformly at random
    vector<long long> randomSwitches(int n, int m) {
        long long max_switch = (1ll << m) - 1;

        vector<long long> switches(n);
        for (auto &s : switches) s = rnd.nextLongLong(1, max_switch);

        return switches;
    }

    // For each switch, chooses the number of lamps to toggle in [1, max_lamps],
    // then chooses which lamps to toggle uniformly at random
    vector<long long> randomSparseSwitches(int n, int m, int max_lamps) {
        vector<long long> switches(n);

        for (int i = 0; i < n; i++) {
            int n_lamps = rnd.nextInt(1, max_lamps);

            vector<int> bits(m);
            iota(bits.begin(), bits.end(), 0);
            rnd.shuffle(bits.begin(), bits.end());

            long long v = 0;
            for (int j = 0; j < n_lamps; j++) {
                v |= (1ll << bits[j]);
            }
            switches[i] = v;
        }

        return switches;
    }

    // Generates all queries at random
    vector<pair<int, int>> randomQueries(int n, int q) {
        vector<pair<int, int>> que;

        for (int i = 0; i < q; i++) {
            int l = rnd.nextInt(1, n);
            int r = rnd.nextInt(1, n);
            if (l > r) swap(l, r);
            que.push_back({l, r});
        }

        return que;
    }

    // Generates wide-spanning queries
    // (endpoints are within distance from_ends from the ends of the array)
    vector<pair<int, int>> wideSpanQueries(int n, int from_ends, int q) {
        vector<pair<int, int>> que;

        for (int i = 0; i < q; i++) {
            int l = rnd.nextInt(1, from_ends);
            int r = rnd.nextInt(n - from_ends + 1, n);
            que.push_back({l, r});
        }

        return que;
    }

    // Periodic case: switches are [1, 2, ..., m, 1, 2, ..., m, ...]
    tuple<vector<long long>, vector<pair<int, int>>> periodicCase(
        int n, int m, int q, double impossible_proba
    ) {
        vector<long long> switches;
        for (int i = 0; i < n; i++) {
            switches.push_back(1ll << (i % m));
        }

        vector<pair<int, int>> que;
        for (int i = 0; i < q; i++) {
            int l, r;

            if (rnd.nextDouble(0.0, 1.0) <= impossible_proba) {
                // get a -1 answer
                l = rnd.nextInt(1, n);
                r = rnd.nextInt(l, min(n, l + m - 2));
            } else {
                // get a possible answer
                do {
                    l = rnd.nextInt(1, n);
                    r = rnd.nextInt(1, n);
                    if (l > r) swap(l, r);
                } while (r - l + 1 < m);
            }

            que.push_back({l, r});
        }

        return {switches, que};
    }

    // Large jmp case:
    // Split the switches into 3 groups:
    // - Left   : bit (m-1) always off
    // - Middle : bit 0 always off
    // - Right  : bit (m-1) always off
    // The value at jmp near the endpoints of the array will be large
    tuple<vector<long long>, vector<pair<int, int>>> largeJmpCase(
        int n, int m, int q, int middle_length, double impossible_proba
    ) {
        long long max_switch_exc = (1ll << (m - 1)) - 1;
        int lf_length = (n - middle_length) / 2;
        int rg_length = n - middle_length - lf_length;

        // generate switches
        vector<long long> switches;
        for (int i = 0; i < lf_length; i++) {
            switches.push_back(rnd.nextLongLong(1, max_switch_exc));
        }
        for (int i = 0; i < middle_length; i++) {
            switches.push_back(rnd.nextLongLong(1, max_switch_exc) << 1);
        }
        for (int i = 0; i < rg_length; i++) {
            switches.push_back(rnd.nextLongLong(1, max_switch_exc));
        }

        // functions to generate a left/right endpoint
        // in the first/last 10% of the range
        const auto generateLeft = [&](int lf_bound, int rg_bound) {
            int len = (rg_bound - lf_bound + 1) / 10;
            return lf_bound + rnd.nextInt(0, max(0, len-1));
        };
        const auto generateRight = [&](int lf_bound, int rg_bound) {
            int len = (rg_bound - lf_bound + 1) / 10;
            return rg_bound - rnd.nextInt(0, max(0, len-1));
        };

        vector<pair<int, int>> que;
        for (int i = 0; i < q; i++) {
            int l, r;

            if (rnd.nextDouble(0.0, 1.0) <= impossible_proba) {
                // get a -1 answer:
                // both endpoints are in the left/right part
                if (rnd.nextInt(0, 1) == 0) {
                    // left
                    l = generateLeft(1, lf_length);
                    r = generateRight(1, lf_length);
                } else {
                    // right
                    l = generateLeft(n-rg_length+1, n);
                    r = generateRight(n-rg_length+1, n);
                }
            } else {
                // get a possible answer:
                // one endpoint in left, and one in right

                if (rnd.nextInt(0, 1) == 0) {
                    // edge of left
                    l = generateLeft(1, lf_length);
                } else {
                    // anywhere in left
                    l = rnd.nextInt(1, lf_length);
                }

                if (rnd.nextInt(0, 1) == 0) {
                    // edge of right
                    r = generateRight(n-rg_length+1, n);
                } else {
                    // anywhere in left
                    r = rnd.nextInt(n-rg_length+1, n);
                }
            }

            que.push_back({l, r});
        }

        return {switches, que};
    }

    // Large answer case: split the array into 3 parts (left, mid, right)
    // and distribute the bits into 3 sets, one for each part
    // A query must touch all 3 parts to have a possible answer
    tuple<vector<long long>, vector<pair<int, int>>> largeAnswerCase(
        int n, int m, int q, int edge_size, double impossible_proba
    ) {
        // assign bits to parts
        int lf_bits = m / 3;
        int mid_bits = m / 3;
        int rg_bits = m - mid_bits - lf_bits;

        long long lf_max = (1ll << lf_bits) - 1;
        long long mid_max = (1ll << mid_bits) - 1;
        long long rg_max = (1ll << rg_bits) - 1;

        // generate switches
        vector<long long> switches;
        for (int i = 0; i < edge_size; i++) {
            switches.push_back(rnd.nextLongLong(1, lf_max));
        }
        for (int i = 0; i < n - 2*edge_size; i++) {
            switches.push_back(rnd.nextLongLong(1, mid_max) << lf_bits);
        }
        for (int i = 0; i < edge_size; i++) {
            switches.push_back(
                rnd.nextLongLong(1, rg_max) << (lf_bits + mid_bits)
            );
        }
        
        // generate queries
        vector<pair<int, int>> que;

        // first type: slightly into left and right parts
        // (may be -1 if the endpoints are too close to the edge)
        int to_try = m + 2;

        int l2 = edge_size;
        int l1 = max(0, l2 - to_try);

        int r1 = n - edge_size + 1;
        int r2 = min(n, r2 + to_try);

        for (int i = l1; i <= l2; i++) {
            for (int j = r1; j <= r2; j++) {
                que.push_back({i, j});
            }
        }

        // second type: randomly generate queries anywhere in the parts

        // function to get a random point in a part
        // (0 = left, 1 = mid, 2 = right)
        const auto getSegment = [&](int idx) {
            if (idx == 0) {
                return rnd.nextInt(1, edge_size);
            } else if (idx == 1) {
                return rnd.nextInt(edge_size + 1, n - edge_size);
            } else {
                return rnd.nextInt(n - edge_size + 1, n);
            }
        };

        while (static_cast<int>(que.size()) < q) {
            int l, r;

            if (rnd.nextDouble(0.0, 1.0) <= impossible_proba) {
                // get a -1 answer

                // first, choose parts for l and r (as long as not 0 and 2)...
                int tl, tr;
                do {
                    tl = rnd.nextInt(0, 2);
                    tr = rnd.nextInt(0, 2);
                    if (tl > tr) swap(tl, tr);
                } while (tl == 0 && tr == 2);
                
                // ... then get indices from those parts
                l = getSegment(tl);
                r = getSegment(tr);
                if (l > r) swap(l, r);

            } else {
                // attempt to get a possible answer
                l = getSegment(0);
                r = getSegment(2);
            }

            que.push_back({l, r});
        }

        return {switches, que};
    }

    // Concatenate multiple large answer cases together
    // (so there are many distinct answers, unlike the last generator)
    // - concatenate cnt TCs, n and q evenly distributed between each
    // - each TC can have an edge size between
    //   [min_edge_ratio, max_edge_ratio] * (n for that TC)
    tuple<vector<long long>, vector<pair<int, int>>> concatLargeAnswerCases(
        int n, int m, int q, int cnt,
        double min_edge_ratio, double max_edge_ratio, double impossible_proba
    ) {
        // distribute n and q evenly
        vector<int> n_vals(cnt, n / cnt);
        for (int i = 0; i < n % cnt; i++) n_vals[i]++;
        rnd.shuffle(n_vals.begin(), n_vals.end());
        
        vector<int> q_vals(cnt, q / cnt);
        for (int i = 0; i < q % cnt; i++) q_vals[i]++;
        rnd.shuffle(q_vals.begin(), q_vals.end());

        vector<long long> switches;
        vector<pair<int, int>> que;
        int index_offset = 0; 

        // generate each TC
        for (int i = 0; i < cnt; i++) {
            double edge_ratio = rnd.nextDouble(min_edge_ratio, max_edge_ratio);
            auto [cur_switches, cur_que] = largeAnswerCase(
                n_vals[i], m, q_vals[i],
                static_cast<int>(n_vals[i] * edge_ratio), impossible_proba
            );

            for (auto i : cur_switches) switches.push_back(i);
            for (auto [l, r] : cur_que) {
                que.push_back({l + index_offset, r + index_offset});
            }
            
            index_offset += n_vals[i];
        }

        return {switches, que};
    }

    // Specific jmp case
    // - break the array into a number of segments of size segment_size
    // - split each segment into two parts (left, right),
    //   each with a banned bit
    // - use random queries to increase variety of the answer
    tuple<vector<long long>, vector<pair<int, int>>> specificJmpCase(
        int n, int m, int q, int segment_size
    ) {
        long long max_switch = (1ll << m) - 1;

        // generate switches: add new segment while needed
        vector<long long> switches;
        while (static_cast<int>(switches.size()) < n) {
            // break segment into left and right
            int lf_size = segment_size / 2 + rnd.nextInt(-5, 5);
            int rg_size = segment_size - lf_size;

            // choose a bit to ban in left and right
            int lf_ban = rnd.nextInt(0, m-1);
            int rg_ban;
            do {
                rg_ban = rnd.nextInt(0, m-1);
            } while (rg_ban == lf_ban);

            // generate switch in each side:
            // get random number and turn off the banned bit if needed
            for (int i = 0; i < lf_size; i++) {
                long long v = rnd.nextInt(1, max_switch);
                if (v & (1ll << lf_ban)) v ^= (1ll << lf_ban);
                switches.push_back(v);
            }
            for (int i = 0; i < rg_size; i++) {
                long long v = rnd.nextInt(1, max_switch);
                if (v & (1ll << rg_ban)) v ^= (1ll << rg_ban);
                switches.push_back(v);
            }
        }
        switches.resize(n);

        // get random queries
        vector<pair<int, int>> que = randomQueries(n, q);

        return {switches, que};
    }
};
