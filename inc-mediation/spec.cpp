#include <bits/stdc++.h>
#include <tcframe/spec.hpp>

using namespace std;
using namespace tcframe;

class ProblemSpec : public BaseProblemSpec {
protected:
    // Input variables
    int N, S1, S2;
    vector<int> U, V, W;

    // Temporary variables when generating TC
    using Edge = tuple<int, int, int>;
    vector<Edge> edges;

    // Output variables
    long long answer;

    // Constants
    const int MIN_N = 2;
    const int MAX_N = 200'000;
    const int MIN_W = 1;
    const int MAX_W = 100;

    const int SQRT_MAX_N = static_cast<int>(sqrt(MAX_N));
    const int SQRT_MAX_W = static_cast<int>(sqrt(MAX_W));

    void InputFormat() {
        LINE(N, S1, S2);
        LINES(U, V, W) % SIZE(N - 1);
    }

    void OutputFormat() {
        LINE(answer);
    }

    void GradingConfig() {
        TimeLimit(2);
        MemoryLimit(2048);
    }

    void Constraints() {
        CONS(MIN_N <= N && N <= MAX_N);
        CONS(1 <= S1 && S1 < S2 && S2 <= N);
        CONS(validTree(N, U, V));
        CONS(eachElementOf(W).isBetween(MIN_W, MAX_W));
    }

    void Subtask1() {
        Points(100);
    }

private:
    bool validTree(int n, const vector<int> &u, const vector<int> &v) {
        // check sizes of u and v, and the values of their elements
        if (
            static_cast<int>(u.size()) != n-1
            || static_cast<int>(v.size()) != n-1
            || !eachElementOf(u).isBetween(1, n)
            || !eachElementOf(v).isBetween(1, n)
        ) {
            return false;
        }

        // check order of u[i] and v[i]
        for (int i = 0; i < n-1; i++) {
            if (!(u[i] < v[i])) return false;
        }

        // check if the graph is connected, i.e. a valid tree
        vector<vector<int>> adj(n + 1);
        for (int i = 0; i < n-1; i++) {
            adj[u[i]].push_back(v[i]);
            adj[v[i]].push_back(u[i]);
        }

        queue<int> bfs;
        bfs.push(1);
        vector<bool> vis(n + 1, false);
        int vis_count = 0;
        while (!bfs.empty()) {
            int cur = bfs.front();
            bfs.pop();
            if (vis[cur]) continue;
            
            vis[cur] = true;
            vis_count++;
            for (auto nxt : adj[cur]) bfs.push(nxt);
        }

        return vis_count == n;
    }
};

class TestSpec : public BaseTestSpec<ProblemSpec> {
protected:
    void SampleTestCase1() {
        Subtasks({1});
        Input({
            "6 2 4",
            "1 4 5",
            "3 4 1",
            "4 5 1",
            "2 5 2",
            "2 6 2"
        });
        Output({
            "35"
        });
    }

    void BeforeTestCase() {
        U.clear();
        V.clear();
        W.clear();
        edges.clear();
    }

    void AfterTestCase() {
        // shuffle edges
        rnd.shuffle(edges.begin(), edges.end());
        
        // randomly renumerate vertices
        vector<int> perm(N + 1);
        iota(perm.begin() + 1, perm.end(), 1);
        rnd.shuffle(perm.begin() + 1, perm.end());
        
        S1 = perm[S1];
        S2 = perm[S2];
        for (auto &[u, v, w] : edges) {
            u = perm[u];
            v = perm[v];
        }
        
        // convert edges to problem input format
        for (const auto &[u, v, w] : edges) {
            U.push_back(u);
            V.push_back(v);
            W.push_back(w);
        }
        
        // fix ordering
        if (S1 > S2) swap(S1, S2);
        for (int i = 0; i < N-1; i++) {
            if (U[i] > V[i]) swap(U[i], V[i]);
        }
    }

    void TestGroup1() {
        Subtasks({1});

        // 1. Min case
        CASE({
            N = 2; S1 = 1; S2 = 2;
            edges = {{1, 2, 1}};
        });

        // 2. Max distance case (line graph)
        CASE({
            N = MAX_N;
            tie(S1, S2, edges) = maxDistanceCase(N);
        });

        // 3. Max distinct distances (~2N)
        CASE({
            N = MAX_N;
            tie(S1, S2, edges) = maxDistinctDistCase(N, 46);
        });

        // 4. Dense grid
        CASE({
            N = MAX_N;
            tie(S1, S2, edges) = denseGridCase(N, [&]() { return 23; });
        });
        CASE({
            N = MAX_N;
            tie(S1, S2, edges) = denseGridCase(N, [&]() {
                int w = randomNormal(rnd, MAX_W / 2, SQRT_MAX_W);
                return clamp(w, MIN_W, MAX_W);
            });
        });

        // 5. Irregular dense grid
        CASE({
            N = MAX_N;
            tie(S1, S2, edges) = irregularDenseGridCase(N);
        });

        // 6. Tree with path of given length X between S1 and S2
        CASE({
            N = MAX_N;
            tie(S1, S2, edges) = givenPathLengthCase(N, SQRT_MAX_N, [&]() {
                return 77;
            });
        });
        CASE({
            N = MAX_N;
            tie(S1, S2, edges) = givenPathLengthCase(N, SQRT_MAX_N, [&]() {
                return rnd.nextInt(MAX_W*9/10, MAX_W);
            });
        });
        CASE({
            N = MAX_N;
            tie(S1, S2, edges) = givenPathLengthCase(N, SQRT_MAX_N, [&]() {
                int w = randomNormal(rnd, MAX_W*2/3, SQRT_MAX_W);
                return clamp(w, MIN_W, MAX_W);
            });
        });
        CASE({
            N = MAX_N;
            tie(S1, S2, edges) = givenPathLengthCase(
                N, SQRT_MAX_N,
                [&]() { return rnd.nextInt(MAX_W*3/4, MAX_W); }, 
                [&]() { return rnd.nextInt(MIN_W, MAX_W/10); }
            );
        });
        CASE({
            N = MAX_N;
            tie(S1, S2, edges) = givenPathLengthCase(N, N/2, [&]() {
                return rnd.nextInt(MAX_W/3, MAX_W*2/3);
            });
        });
        CASE({
            N = MAX_N;
            tie(S1, S2, edges) = givenPathLengthCase(N, N/2, [&]() {
                int w = randomNormal(rnd, MAX_W/5, MAX_W/20);
                return clamp(w, MIN_W, MAX_W);
            });
        });
        CASE({
            N = MAX_N;
            tie(S1, S2, edges) = givenPathLengthCase(N, N/3, [&]() {
                return rnd.nextInt(MAX_W*9/10, MAX_W);
            });
        });
        CASE({
            N = MAX_N;
            tie(S1, S2, edges) = givenPathLengthCase(N, N/25, [&]() {
                return rnd.nextInt(MIN_W, MAX_W);
            });
        });
        CASE({
            N = MAX_N;
            tie(S1, S2, edges) = givenPathLengthCase(N, N/30,
                [&]() { return rnd.nextInt(MAX_W*4/5, MAX_W); }, 
                [&]() {
                    int w = randomNormal(rnd, MAX_W/10, MAX_W/18);
                    return clamp(w, MIN_W, MAX_W);
                }
            );
        });

        // 7. Total random cases
        CASE({
            N = MAX_N; S1 = 1; S2 = 2;
            edges = randomWeightedTree(N, [&]() {
                return rnd.nextInt(MIN_W, MAX_W);
            });
        });
        CASE({
            N = MAX_N; S1 = 1; S2 = 2;
            edges = randomWeightedTree(N, [&]() {
                return MAX_W;
            });
        });

        // 8. Star graph
        CASE({
            N = MAX_N;
            tie(S1, S2, edges) = starGraphCase(N);
        });

        // 9. Random tree, select S1 and S2 heuristically
        CASE({
            N = MAX_N;
            edges = randomWeightedTree(N, [&]() {
                return rnd.nextInt(MIN_W, MAX_W);
            });
            tie(S1, S2) = HeuristicTree(edges).execute(rnd);
        });

        // 10. Duplicate some of the cases given above
        // (generate with smaller # of nodes, then duplicate some nodes
        // that are not on the path s1 <-> s2)
        {
            CASE({
                N = MAX_N;
                tie(S1, S2, edges) = makeCopiedNodesTC(
                    N,
                    denseGridCase(N/3, [&]() { return 47; })
                );
            });
            CASE({
                N = MAX_N;
                tie(S1, S2, edges) = makeCopiedNodesTC(
                    N,
                    denseGridCase(N/4, [&]() {
                        int w = randomNormal(rnd, MAX_W*2/3, MAX_W/3);
                        return clamp(w, MIN_W, MAX_W);
                    })
                );
            });
            CASE({
                N = MAX_N;
                tie(S1, S2, edges) = makeCopiedNodesTC(
                    N,
                    givenPathLengthCase(SQRT_MAX_N, SQRT_MAX_N / 10, [&]() {
                        return rnd.nextInt(MAX_W*9/10, MAX_W);
                    })
                );
            });
            CASE({
                N = MAX_N;
                tie(S1, S2, edges) = makeCopiedNodesTC(
                    N,
                    givenPathLengthCase(N/3, SQRT_MAX_N, [&]() {
                        return rnd.nextInt(MAX_W*8/10, MAX_W);
                    })
                );
            });
            CASE({
                N = MAX_N;
                tie(S1, S2, edges) = makeCopiedNodesTC(
                    N,
                    givenPathLengthCase(N/2, N/4, [&]() {
                        int w = randomNormal(rnd, MAX_W*2/9, MAX_W/24);
                        return clamp(w, MIN_W, MAX_W);
                    })
                );
            });
            CASE({
                N = MAX_N;
                tie(S1, S2, edges) = makeCopiedNodesTC(
                    N,
                    {1, 2, randomWeightedTree(N/12, [&]() {
                        return rnd.nextInt(MIN_W, MAX_W);
                    })}
                );
            });
        }
    }

private:
    // Samples from the normal distribution with given mean and stddev,
    // then rounds to the nearest integer
    static int randomNormal(Random &rnd, double mean, double stddev) {
        static bool has_spare = false;
        static double spare;

        if (has_spare) {
            has_spare = false;
            return round(mean + stddev * spare);
        }

        double u, v, s;
        do {
            u = rnd.nextDouble(-1.0, 1.0);
            v = rnd.nextDouble(-1.0, 1.0);
            s = u * u + v * v;
        } while (s == 0.0 || s >= 1.0);

        double mul = sqrt(-2.0 * log(s) / s);
        spare = v * mul;
        has_spare = true;
        return round(mean + stddev * (u * mul));
    }

    // Generates a random pair of distinct integers, both in the range [lo, hi]
    static pair<int, int> randomIntPair(Random &rnd, int lo, int hi) {
        int u = rnd.nextInt(lo, hi), v;
        do {
            v = rnd.nextInt(lo, hi);
        } while (u == v);
        return {u, v};
    }

    // Generates a random labeled tree with n vertices
    vector<pair<int, int>> randomTree(int n) {
        if (n == 1) return {};
        if (n == 2) return {{1, 2}};
        
        vector<pair<int, int>> edges;

        // generate a Prufer sequence
        vector<int> prufer(n - 2);
        for (auto &i : prufer) i = rnd.nextInt(1, n);
        
        // convert Prufer sequence to edges
        vector<int> deg(n + 1, 1);
        for (int x : prufer) deg[x]++;
        priority_queue<int, vector<int>, greater<int>> leaves;
        for (int v = 1; v <= n; v++) if (deg[v] == 1) leaves.push(v);
        for (int x : prufer) {
            int u = leaves.top(); leaves.pop();
            edges.push_back({u, x});
            if (--deg[x] == 1) leaves.push(x);
            --deg[u];
        }
        int u = leaves.top(); leaves.pop();
        int v = leaves.top(); leaves.pop();
        edges.push_back({u, v});

        return edges;
    }

    // Generates a random weighted tree with n vertices
    // and weights from a weight generator
    vector<Edge> randomWeightedTree(int n, function<int()> weight_gen) {
        vector<pair<int, int>> random_edges = randomTree(n);
        vector<Edge> edges;
        for (auto [u, v] : random_edges) {
            edges.push_back({u, v, weight_gen()});
        }
        return edges;
    }

    // Alias for test data
    using TestData = tuple<int, int, vector<Edge>>;

    // Max distance case:
    // Line graph with all max-weight edges, special nodes at the ends
    TestData maxDistanceCase(int n) {
        vector<Edge> edges;
        for (int i = 2; i <= n; i++) {
            edges.push_back({i-1, i, MAX_W});
        }
        return {1, n, edges};
    }

    // Max distinct distance case:
    // Line graph with first edge of weight k, the rest of weight 2k
    // Has distances {k, 2k, ..., (2n-3)k}
    TestData maxDistinctDistCase(int n, int k) {
        vector<Edge> edges = {{1, 2, k}};
        for (int i = 3; i <= n; i++) {
            edges.push_back({i-1, i, 2*k});
        }
        return {1, n, edges};
    }

    // Dense grid case:
    // O - X - X - ... - O
    // |   |   |         |
    // X   X   X         X
    // :   :   :         :
    // X   X   X         X
    // O = special vertices
    // Weights generated from a function
    TestData denseGridCase(int n, function<int()> weight_gen) {
        int width = static_cast<int>(sqrt(n));
        int s1 = 1, s2 = width;
        vector<Edge> edges;

        // main railing
        for (int i = 2; i <= width; i++) {
            edges.push_back({i-1, i, weight_gen()});
        }

        // extend the railings downward
        vector<int> lst(width);
        iota(lst.begin(), lst.end(), 1);

        int ptr = 0;
        for (int i = width + 1; i <= n; i++) {
            edges.push_back({lst[ptr], i, weight_gen()});
            lst[ptr] = i;
            ptr = (ptr + 1) % width;
        }

        return {s1, s2, edges};
    }
    
    // Irregular dense grid case:
    // Same with dense grid with weight 1, but an extension vertex
    // can connect several steps up with an edge of weight > 1
    // without changing distances to the special vertices
    TestData irregularDenseGridCase(int n) {
        int width = static_cast<int>(sqrt(n));
        int s1 = 1, s2 = width;
        vector<Edge> edges;

        // main railing
        for (int i = 2; i <= width; i++) {
            edges.push_back({i-1, i, 1});
        }

        // extend the railings downward, maintain last MAX_W vertices
        vector<deque<int>> lst(width);
        for (int i = 0; i < width; i++) lst[i] = {i + 1};

        int ptr = 0;
        int cur_dist = 1;
        for (int i = width + 1; i <= n; i++) {
            if (static_cast<int>(lst[ptr].size()) > MAX_W) {
                lst[ptr].pop_front();
            }

            int bound = static_cast<int>(lst[ptr].size());
            int w = rnd.nextInt(1, bound);
            edges.push_back({lst[ptr][bound-w], i, w});
            lst[ptr].push_back(i);

            ptr++;
            if (ptr == width) {
                ptr = 0;
                cur_dist++;
            }
        }

        return {s1, s2, edges};
    }

    // Given path length case:
    // - Generate a path of length path_length between the special vertices
    //   ("backbone")
    // - Group the vertices into (path_length + 1) buckets,
    //   and generate a random tree for each of the vertices ("peripheral")
    // - Ensures that the special vertices' (uniweghted) distances
    //   can be controlled
    TestData givenPathLengthCase(
        int n, int path_length,
        function<int()> backbone_weight_gen,
        function<int()> peripheral_weight_gen
    ) {
        int s1 = 1, s2 = path_length + 1;
        vector<Edge> edges;

        // make backbone
        for (int i = 2; i <= s2; i++) {
            edges.push_back({i-1, i, backbone_weight_gen()});
        }

        // group all nodes into (path_length + 1) buckets
        vector<vector<int>> buckets(path_length + 1);
        for (int i = 0; i <= path_length; i++) buckets[i].push_back(i + 1);
        for (int i = s2 + 1; i <= n; i++) {
            int chosen = rnd.nextInt(0, path_length);
            buckets[chosen].push_back(i);
        }

        // create random peripheral tree for each bucket
        for (auto &bucket : buckets) {
            vector<pair<int, int>> tree = randomTree(bucket.size());
            for (auto [u, v] : tree) {
                edges.push_back({
                    bucket[u-1], bucket[v-1],
                    peripheral_weight_gen()
                });
            }
        }

        return {s1, s2, edges};
    }
    
    // Same as above, but uses the same weight generator
    // for backbone and peripheral
    TestData givenPathLengthCase(
        int n, int path_length,
        function<int()> weight_gen
    ) {
        return givenPathLengthCase(n, path_length, weight_gen, weight_gen);
    }

    // Generates a star graph with random weights
    // Special vertices are on leaves
    TestData starGraphCase(int n) {
        vector<Edge> edges;
        for (int i = 2; i <= n; i++) {
            edges.push_back({1, i, rnd.nextInt(MIN_W, MAX_W)});
        }
        return {2, 3, edges};
    }

    // Make copied nodes:
    // - Given a test case, adds additional nodes to the tree
    //   until the number of nodes becomes n
    // - To copy a node, ensure that the distances to the special vertices
    //   are the same
    // - Choose a node that is not on the path s1 <-> s2,
    //   then duplicate the parent edge of that node w.r.t. the path as "root"
    TestData makeCopiedNodesTC(int n, TestData test_case) {
        auto [s1, s2, prv_edges] = test_case;
        int prv_n = static_cast<int>(prv_edges.size()) + 1;

        // root old tree at s1 and find parents
        vector<vector<pair<int, int>>> adj(prv_n + 1);
        for (auto [u, v, w] : prv_edges) {
            adj[u].push_back({v, w});
            adj[v].push_back({u, w});
        }
        vector<int> parent(prv_n + 1), parent_edge_w(prv_n + 1);
        function<void(int, int)> dfs = [&](int cur, int par) {
            for (auto [nxt, w] : adj[cur]) {
                if (nxt == par) continue;
                parent[nxt] = cur;
                parent_edge_w[nxt] = w;
                dfs(nxt, cur);
            }
        };
        parent[s1] = -1;
        dfs(s1, -1);

        // find nodes that are allowed (not on s1 <-> s2 path)
        vector<bool> allowed(prv_n + 1, true);
        for (int i = s2; i != -1; i = parent[i]) {
            allowed[i] = false;
        }
        vector<int> allowed_v;
        for (int i = 1; i <= prv_n; i++) if (allowed[i]) allowed_v.push_back(i);
        int n_allowed = static_cast<int>(allowed_v.size());

        // copy nodes until the requested number of nodes
        vector<Edge> edges = prv_edges;
        for (int t = prv_n + 1; t <= n; t++) {
            int to_dupe = allowed_v[rnd.nextInt(0, n_allowed - 1)];
            edges.push_back({parent[to_dupe], t, parent_edge_w[to_dupe]});
        }

        return {s1, s2, edges};
    }

    // Anything below is weird stuff...

    // Heuristic tree:
    // We want to get two special nodes such that d(u, S1) and d(u, S2)
    // both are as 'evenly spread' as possible
    // i.e. the absolute value of correlation is as low as possible
    // Choose S1 and S2 randomly, then do simulated annealing
    // to move to better candidates
    struct HeuristicTree {
        using i128 = __int128;

        int n;
        vector<vector<pair<int, int>>> adj;

        HeuristicTree(const vector<Edge> &edges) {
            n = static_cast<int>(edges.size()) + 1;
            adj.assign(n + 1, {});

            for (auto [u, v, w] : edges) {
                adj[u].push_back({v, w});
                adj[v].push_back({u, w});
            }
        }

        // Get distances of all nodes from a node
        vector<int> getDistancesFrom(int s) {
            vector<int> dist(n + 1, -1);
            queue<int> q;
            q.push(s);
            dist[s] = 0;
            while (!q.empty()){
                int u = q.front(); q.pop();
                for (auto [nxt, w] : adj[u]) if (dist[nxt] == -1) {
                    dist[nxt] = dist[u] + w;
                    q.push(nxt);
                }
            }
            return dist;
        }

        // Calculate the correlation between x (dist(u, S1)) and y (dist(u, S2))
        long double corr_xy(const vector<int> &x, const vector<int> &y) {
            long double sx = 0, sy = 0;
            i128 sxx = 0, syy = 0, sxy = 0;

            for (int i = 1; i <= n; i++){
                sx += x[i];
                sy += y[i];
                sxx += static_cast<i128>(x[i]) * x[i];
                syy += static_cast<i128>(y[i]) * y[i];
                sxy += static_cast<i128>(x[i]) * y[i];
            }

            long double nld = static_cast<long double>(n);
            long double ex = sx / nld, ey = sy / nld;

            // Var(x) = E[x^2] - (E[x])^2
            long double Exx = static_cast<long double>(sxx) / nld;
            long double Eyy = static_cast<long double>(syy) / nld;
            long double Exy = static_cast<long double>(sxy) / nld;

            long double varx = max(0.0L, Exx - ex*ex);
            long double vary = max(0.0L, Eyy - ey*ey);
            long double cov  = Exy - ex*ey;

            // Degenerate case
            if (varx == 0 || vary == 0) return 0.0;

            long double corr = cov / sqrtl(varx * vary);
            corr = min(1.0L, max(-1.0L, corr));
            return fabsl(corr);
        }

        // Get score of a special node pair
        long double scorePair(int a, int b) {
            vector<int> da = getDistancesFrom(a);
            vector<int> db = getDistancesFrom(b);
            long double corr = corr_xy(da, db);
            return corr;
        }

        // Main function to find the special nodes
        pair<int, int> execute(
            Random &rnd,
            int steps = 1000,
            int logging_steps = 100,    // log every this number of steps
            long double T0 = 0.25L,     // initial temperature
            long double Tend = 1e-4L,   // final temperature floor
            long double alpha = 0.997L  // cooling factor per step
        ) {
            auto [a, b] = randomIntPair(rnd, 1, n);
            long double cur = scorePair(a, b);

            // best so far
            int best_a = a, best_b = b;
            long double best = cur;

            long double T = T0;
            const long double log_tiny = -40.0L; // guard for underflow in exp

            for (int step = 0; step < steps; ++step) {
                if ((step + 1) % logging_steps == 0) {
                    cerr << "Doing step " << (step + 1);
                    cerr << " of " << steps << "\n";
                }
                
                // Cooling
                T = max(Tend, T * alpha);

                int na = a, nb = b;
                bool have_move = false;
                
                // choose whether to move a or b
                bool move_a = rnd.nextInt(0, 1);
                if (move_a) {
                    if (!adj[a].empty()) {
                        if (
                            static_cast<int>(adj[a].size()) == 1
                            && adj[a][0].first == b
                        ) {
                            // no valid move for a
                        } else {
                            while (true) {
                                int idx = rnd.nextInt(
                                    0,
                                    static_cast<int>(adj[a].size()) - 1
                                );
                                int nxt = adj[a][idx].first;
                                if (nxt != b) {
                                    na = nxt;
                                    have_move = true;
                                    break;
                                }
                            }
                        }
                    }
                } else {
                    if (!adj[b].empty()) {
                        if (
                            static_cast<int>(adj[b].size()) == 1
                            && adj[b][0].first == a
                        ) {
                            // no valid move for b
                        } else {
                            while (true) {
                                int idx = rnd.nextInt(
                                    0,
                                    static_cast<int>(adj[b].size()) - 1
                                );
                                int nxt = adj[b][idx].first;
                                if (nxt != a) {
                                    nb = nxt;
                                    have_move = true;
                                    break;
                                }
                            }
                        }
                    }
                }

                if (!have_move) continue;

                long double nxtScore = scorePair(na, nb);
                long double delta = nxtScore - cur;

                bool accept = false;
                if (delta <= 0) {
                    // score already improved
                    accept = true;
                } else {
                    // accept with probability exp(-delta/T)
                    long double expo = -delta / max(T, 1e-18L);
                    if (expo > log_tiny) {
                        long double p = expl(expo);
                        if (rnd.nextDouble(0.0, 1.0) < p) accept = true;
                    }
                }

                if (accept) {
                    a = na; b = nb; cur = nxtScore;
                    if (cur + 1e-15L < best) {
                        best = cur; best_a = a; best_b = b;
                    }
                }
            }

            cerr << "Best |corr|: " << best;
            cerr << " at (" << best_a << ", " << best_b << ")\n";
            return {best_a, best_b};
        }
    };
};
