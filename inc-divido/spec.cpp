#include <bits/stdc++.h>
#include <tcframe/spec.hpp>
#include <tcrand/graph.hpp>

using namespace std;
using namespace tcframe;
using namespace tcrand;

using ll = long long;
#define mp make_pair
#define fr first
#define sc second

const int MXN = 100000;
const int MXM = 200000;
const int MXW = 1000000000;

class ProblemSpec : public BaseProblemSpec {
protected:
	int N, M;
	vector<int> U, V, W;

	ll ans;

	void InputFormat() {
		LINE(N, M);
		LINES(U, V, W) % SIZE(M);
	}

	void OutputFormat() {
		LINE(ans);
	}

	void GradingConfig() {
		TimeLimit(1);
		MemoryLimit(256);
	}

	void Constraints() {
		CONS(3 <= N && N <= MXN);
		CONS(N - 1 <= M && M <= MXM);
		CONS(eachElementOf(U).isBetween(1, N));
		CONS(eachElementOf(V).isBetween(1, N));
		CONS(eachElementOf(W).isBetween(1, MXW));
		CONS(noSelfLoopsMultiEdges(U, V));
		CONS(isConnected(N, U, V));
	}

	void Subtask1() {
		Points(100);
	}

private:
	bool noSelfLoopsMultiEdges(vector<int> &u, vector<int> &v) {
		set<pair<int, int> > edges;
		for (int i = 0; i < u.size(); i++) {
			if (u[i] >= v[i]) return false;
			if (edges.count(mp(u[i], v[i]))) return false;
			edges.insert(mp(u[i], v[i]));
		}
		return true;
	}
	
	int par[MXN + 5];
	int f(int x) {
		return par[x] = (x == par[x] ? x : f(par[x]));
	}
	bool isConnected(int n, vector<int> &u, vector<int> &v) {
		for(int i = 1; i <= n; i++) par[i] = i;
		
		int cnt = n;
		for(int i = 0; i < u.size(); i++) {
			cnt -= f(u[i]) != f(v[i]);
			par[f(v[i])] = f(u[i]);
		}

		return cnt == 1;
	}
};

class TestSpec : public BaseTestSpec<ProblemSpec> {
protected:
	void SampleTestCase1() {
		Subtasks({1});
		Input({
			"6 7",
			"5 6 1",
			"4 5 1",
			"3 4 1",
			"1 4 5",
			"2 5 2",
			"3 6 1",
			"2 6 2",
		});
		Output({
			"11",
		});
	}

	void SampleTestCase2() {
		Subtasks({1});
		Input({
			"3 2",
			"1 2 265",
			"1 3 265",
		});
		Output({
			"663",
		});
	}

	void BeforeTestCase() {
		U.clear();
		V.clear();
		W.clear();
	}

	void AfterTestCase() {
		int perm[N + 5];
		for (int i = 1; i <= N; i++) perm[i] = i;
		for (int i = 4; i <= N; i++) // Don't swap the positions of vertices 1, 2, and 3
			swap(perm[i], perm[rnd.nextInt(4, i)]);

		for (int i = 0; i < M; i++) {
			U[i] = perm[U[i]];
			V[i] = perm[V[i]];
			if (U[i] >= V[i]) swap(U[i], V[i]);
			int p = rnd.nextInt(0, i);
			swap(U[i], U[p]);
			swap(V[i], V[p]);
			swap(W[i], W[p]);
		}
	}

	void TestGroup1() {
		Subtasks({1});

		CASE({N = 3, M = 2, U = {1, 2}, V = {2, 3}, W = {1, 1};});
		CASE({N = 3, M = 2, U = {1, 2}, V = {3, 3}, W = {1, 1};});
		CASE({N = 3, M = 3, U = {1, 1, 2}, V = {2, 3, 3}, W = {1, 1, 1};});
		CASE({N = 4, M = 4, U = {1, 3, 3, 4}, V = {3, 2, 4, 2}, W = {1, 6, 3, 3};});

		CASE(pureRandomGraph(MXN, MXM, 100));
		CASE(pureRandomGraph(MXN, MXM, MXW));
		CASE({pureRandomGraph(MXN, MXM, MXW); W.assign(MXM, MXW);});
		CASE(lineGraph(MXN, 3, 1, 2));
		CASE(lineGraph(MXN, 3, 1, 2); W.assign(M, MXW););
		CASE(lineGraph(MXN, 1, 3, 2));
		CASE(lineGraph(MXN, 1, 3, 2); W.assign(M, MXW););
		CASE(lineGraph(MXN, 2, 1, 3));
		CASE(lineGraph(MXN, 2, 1, 3); W.assign(M, MXW););

		CASE(VGraph(MXN - 1));

		CASE(ribbonGraph(MXN - 1, 1));
		CASE(ribbonGraph(MXN - 1, 2));
		CASE(ribbonGraph(MXN - 1, 3));

		CASE({
			ribbonGraph(MXN - 3, 4);
			N++;
			for (int i = 0; i < M; i++) {
				if (U[i] == 3) U[i] = N;
				if (V[i] == 3) V[i] = N;
			}
			U.push_back(1); V.push_back(3); W.push_back(MXW);
			M = U.size();
		});

		CASE({
			ribbonGraph(MXN - 3, 4);
			N++;
			for (int i = 0; i < M; i++) {
				if (U[i] == 3) U[i] = N;
				if (V[i] == 3) V[i] = N;
			}
			for (int i = 0; i < M; i++) {
				if (U[i] == 2) U[i] = 3;
				if (V[i] == 2) V[i] = 3;
			}
			U.push_back(1); V.push_back(2); W.push_back(MXW);
			M = U.size();
		});

		CASE({
			N = MXN;
			vector<int> vt(N - 3); iota(vt.begin(), vt.end(), 4);
			U.push_back(1); V.push_back(3); W.push_back(MXW);
			U.push_back(1); V.push_back(vt[0]); W.push_back(2 * rnd.nextInt(1, (MXW - 1) / 2) + 1);
			for (int i = 0; i + 1 < vt.size(); i++) {
				U.push_back(vt[i]); V.push_back(vt[i + 1]); W.push_back(2 * rnd.nextInt(1, (MXW - 1) / 2) + 1);
			}
			U.push_back(vt.back()); V.push_back(2); W.push_back(MXW - 1);
			M = U.size();
		});

		CASE({
			N = MXN;
			vector<int> vt(N - 3); iota(vt.begin(), vt.end(), 4);
			U.push_back(1); V.push_back(2); W.push_back(MXW);
			U.push_back(1); V.push_back(vt[0]); W.push_back(2 * rnd.nextInt(1, (MXW - 1) / 2) + 1);
			for (int i = 0; i + 1 < vt.size(); i++) {
				U.push_back(vt[i]); V.push_back(vt[i + 1]); W.push_back(2 * rnd.nextInt(1, (MXW - 1) / 2) + 1);
			}
			U.push_back(vt.back()); V.push_back(3); W.push_back(MXW - 1);
			M = U.size();
		});

		CASE({
			pureRandomGraph(MXN - 1, MXM - 1, MXW);
			N++;
			for (int i = 0; i < M; i++) {
				if (U[i] == 3) U[i] = N;
				if (V[i] == 3) V[i] = N;
			}
			U.push_back(1); V.push_back(3); W.push_back(MXW);
			M = U.size();
		});

		CASE({
			pureRandomGraph(MXN - 1, MXM - 1, MXW);
			N++;
			for (int i = 0; i < M; i++) {
				if (U[i] == 3) U[i] = N;
				if (V[i] == 3) V[i] = N;
			}
			for (int i = 0; i < M; i++) {
				if (U[i] == 2) U[i] = 3;
				if (V[i] == 2) V[i] = 3;
			}
			U.push_back(1); V.push_back(2); W.push_back(MXW);
			M = U.size();
		});

		CASE({
			N = 66669;
			vector<int> vt(N - 3); iota(vt.begin(), vt.end(), 4);
			for (int i : vt) {
				for (int j = 1; j <= 3; j++) {
					U.push_back(j); V.push_back(i); W.push_back(2 * rnd.nextInt(1, (MXW - 1) / 2) + 1);
				}
			}
			M = U.size();
		});
	}

	void pureRandomGraph(int n, int m, int wmx) {
		N = n; M = m;
		vector<int> u, v;
		GraphRandomizer graph_rand;
		tie(u, v) = graph_rand.node_count(n).edge_count(m).index_base(1).next().edges();
		U.insert(U.end(), u.begin(), u.end());
		V.insert(V.end(), v.begin(), v.end());
		for (int i = 0; i < m; i++) // odd weight to trigger ceil(k/2)
			W.push_back(2 * rnd.nextInt(1, (wmx - 1) / 2) + 1);
	}

	void lineGraph(int n, int v1, int v2, int vn) {
		N = n; M = n - 1;
		U.push_back(v1); V.push_back(v2); W.push_back(2 * rnd.nextInt(1, (MXW - 1) / 2) + 1);
		U.push_back(v2); V.push_back(4); W.push_back(2 * rnd.nextInt(1, (MXW - 1) / 2) + 1);
		for (int i = 4; i < n; i++) {
			U.push_back(i); V.push_back(i + 1); W.push_back(2 * rnd.nextInt(1, (MXW - 1) / 2) + 1);
		}
		U.push_back(n); V.push_back(vn); W.push_back(2 * rnd.nextInt(1, (MXW - 1) / 2) + 1);
	}

	void VGraph(int n) {
		N = n;
		int k = (N - 3) / 2;
		M = 3 * k + 2;

		vector<int> vt(n - 3);
		iota(vt.begin(), vt.end(), 4);
		rnd.shuffle(vt.begin(), vt.end());
		vector<int> vt2;
		for (int i = 0; i < k; i++) {
			vt2.push_back(vt.back());
			vt.pop_back();
		}

		assert(vt.size() == k && vt2.size() == k);

		U.push_back(1); V.push_back(vt[0]); W.push_back(1);
		for (int i = 0; i + 1 < vt.size(); i++) {
			U.push_back(vt[i]); V.push_back(vt[i + 1]); W.push_back(1);
		}
		U.push_back(vt.back()); V.push_back(2); W.push_back(MXW - 1);

		U.push_back(1); V.push_back(vt2[0]); W.push_back(MXW - k);
		for (int i = 0; i + 1 < vt2.size(); i++) {
			U.push_back(vt2[i]); V.push_back(vt2[i + 1]); W.push_back(1);
		}
		U.push_back(vt2.back()); V.push_back(3); W.push_back(MXW);

		for (int i = 0; i < vt.size(); i++) {
			U.push_back(vt[i]); V.push_back(vt2[i]); W.push_back(MXW - 1);
		}

		assert(U.size() == M);
	}

	void ribbonGraph(int n, int mid) {
		N = n; int lf = 1, rg = 2;
		if (lf == mid) lf = 3;
		if (rg == mid) rg = 3;

		int ly = (n - 3) / 2;
		vector<int> vt(n - 3);
		iota(vt.begin(), vt.end(), 4);
		if (mid == 4) vt[0] = 3;
		int id = 0;
		for (int i = 0; i < ly; i++) {
			U.push_back(lf); V.push_back(vt[id]); W.push_back(i + 2);
			U.push_back(vt[id]); V.push_back(mid); W.push_back(MXW - (i + 2) * 2);
			id++;
		}
		for (int i = 0; i < ly; i++) {
			U.push_back(rg); V.push_back(vt[id]); W.push_back(i + ly + 3);
			U.push_back(vt[id]); V.push_back(mid); W.push_back(MXW - (i + ly + 3) * 2);
			id++;
		}

		M = U.size();
	}
};