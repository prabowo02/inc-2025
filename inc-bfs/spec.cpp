#include <bits/stdc++.h>
#include <tcframe/spec.hpp>

using namespace std;
using namespace tcframe;

using ll = long long;
#define mp make_pair
#define fr first
#define sc second

const int MXN = 200000;
const int MXM = 200000;

class ProblemSpec : public BaseProblemSpec {
protected:
	int N, M;
	vector<int> A;

	vector<int> U, V;
	int m1, m2;

	void InputFormat() {
		LINE(N, M);
		LINE(A % SIZE(N));
	}

	void OutputFormat1() {
		LINES(U, V) % SIZE(M);
	}

	void OutputFormat2() {
		LINE(m1, m2);
	}

	void GradingConfig() {
		TimeLimit(1);
		MemoryLimit(256);
	}

	void StyleConfig() {
		CustomScorer();
	}

	void Constraints() {
		CONS(1 <= N && N <= MXN);
		CONS(1 <= M && M <= MXM);
		CONS(validOrder(A));
	}

	void Subtask1() {
		Points(100);
	}

private:
	bool validOrder(vector<int> &a) {
		if (a[0] != 1) return false;
		set<int> s;
		for (int i : a) {
			if (i < 1 || i > N) return false;
			s.insert(i);
		}
		return (s.size() == N);
	}
};

class TestSpec : public BaseTestSpec<ProblemSpec> {
protected:
	void SampleTestCase1() {
		Subtasks({1});
		Input({
			"5 6",
			"1 5 2 3 4",
		});
		Output({
			"1 5",
			"2 3",
			"5 2",
			"4 3",
			"3 5",
			"4 5",
		});
	}

	void SampleTestCase2() {
		Subtasks({1});
		Input({
			"5 10",
			"1 5 2 3 4",
		});
		Output({
			"-1 -1",
		});
	}

	void BeforeTestCase() {
		A.clear();
	}

	void TestGroup1() {
		Subtasks({1});

		CASE({N = 1, A = {1}, M = 1;});
		CASE({N = 2, A = {1, 2}, M = 1;});
		CASE({N = 2, A = {1, 2}, M = 2;});

		CASE({sortDec(MXN); forceMin();});
		CASE({sortDec(MXN); forceMin(1);}); // -1

		CASE({sortInc(MXN); forceMin();});
		CASE({sortInc(MXN); forceMin(1);}); // -1
		CASE({sortInc(632); forceMax();});
		CASE({sortInc(630); forceMax(0, 4);});
		CASE({sortInc(628); forceMax(1);}); // -1

		CASE({evenLayers(100, 5); forceMax();});
		CASE({evenLayers(2000, 99); forceMin();});
		CASE({evenLayers(75, 8); forceMax(0, 10);});
		CASE({evenLayers(90, 6); forceMax(1);}); // -1
		
		CASE({evenLayersReverse(1000, 199); forceMin(1);}); // -1
		CASE({evenLayersReverse(100, 40); forceMax();});
		CASE({evenLayersReverse(67, 90); forceMax(0, 10);});
		CASE({evenLayersReverse(400, 2); forceMax();});
		CASE({evenLayersReverse(10, 100); forceMax();});
		CASE({evenLayersReverse(10, 100); forceMax(1);}); // -1

		CASE({randomLayers(MXN - 1, 447); M = MXM;});
		CASE({randomLayers(MXN - 1, 447); forceMin(1);}); // -1
		CASE({randomLayers(450, 2); forceMax();});
		CASE({randomLayers(500, 4); forceMax();});
		CASE({randomLayers(550, 5); forceMax(1);}); // -1
		CASE({randomLayers(600, 10); forceMax(0, 10);});

		CASE({randomLayersReverse(30000, 10); forceMax();});
		CASE({randomLayersReverse(3000, 100); forceMax();});
		CASE({randomLayersReverse(6767, 67); forceMax(0, 10);});
		CASE({randomLayersReverse(100000, 2); forceMax(0, 20);});
		CASE({randomLayersReverse(42069, 5); forceMax(1);}); // -1
		CASE({randomLayersReverse(68265, 3); forceMax();});

		CASE({largeLayer(MXN - 1, 447, 265); M = MXM;});
		CASE({largeLayer(400, 3, 2); forceMax();});
		CASE({largeLayer(525, 7, 0); forceMax();});

		CASE({largeLayerReverse(30000, 10, 4); forceMax();});
		CASE({largeLayerReverse(3000, 100, 99); forceMax();});
		CASE({largeLayerReverse(6767, 67, 21); forceMax(0, 10);});
		CASE({largeLayerReverse(100000, 2, 0); forceMax(0, 20);});
		CASE({largeLayerReverse(42069, 5, 1); forceMax(1);}); // -1
		CASE({largeLayerReverse(68265, 3, 2); forceMax();});
	}

private:
	void sortDec(int n) { /* MAX M = (n-1) */
		N = n;
		A.push_back(1);
		for (int i = N; i >= 2; i--)
			A.push_back(i);
	}

	void sortInc(int n) { /* MAX M = n(n-1)/2 */
		N = n;
		for (int i = 1; i <= N; i++)
			A.push_back(i);
	}

	void evenLayers(int n, int k) { /* N = nk, MAX M = n^2 * k(k-1)/2 */
		n *= k;
		A.push_back(1);
		vector<int> vt(n);
		iota(vt.begin(), vt.end(), 1);
		rnd.shuffle(vt.begin(), vt.end());
		vector<vector<int>> v(n / k, vector<int>());
		for (int i = 0; i < n; i++) {
			v[i / k].push_back(vt[i]);
		}
		for (int i = 0; i < n / k; i++) {
			sort(v[i].begin(), v[i].end());
		}
		for (int i = 1; i < n / k; i++) {
			if (v[i - 1][k - 1] < v[i][0]) {
				swap(v[i - 1][k - 1], v[i][0]);
			}
		}
		for (int i = 0; i < n / k; i++) {
			for (int j = 0; j < k; j++) {
				A.push_back(v[i][j] + 1);
			}
		}
		N = A.size();
	}

	void evenLayersReverse(int n, int k) { /* N = nk, MAX M = n^2 * k/2 */
		n *= k;
		A.push_back(1);
		vector<int> vt(n);
		iota(vt.begin(), vt.end(), 1);
		rnd.shuffle(vt.begin(), vt.end());
		vector<vector<int>> v(n / k, vector<int>());
		for (int i = 0; i < n; i++) {
			v[i / k].push_back(vt[i]);
		}
		for (int i = 0; i < n / k; i++) {
			sort(v[i].rbegin(), v[i].rend());
		}
		for (int i = 1; i < n / k; i++) {
			if (v[i - 1].back() > v[i][0]) {
				swap(v[i - 1].back(), v[i][0]);
			}
		}
		for (int i = 0; i < n / k; i++) {
			for (int j = 0; j < k; j++) {
				A.push_back(v[i][j] + 1);
			}
		}
		N = A.size();
	}

	/* ly = N is equal to normal randomization */
	void randomLayers(int n, int ly) { // close to n(n-1)/2 when few layers, closer to n(n-1)/4 when more layers
		A.push_back(1);
		vector<int> vt(n);
		iota(vt.begin(), vt.end(), 1);
		rnd.shuffle(vt.begin(), vt.end());
		vector<vector<int>> v(ly, vector<int>());
		for (int i = 0; i < vt.size(); i++) {
			v[rnd.nextInt(0, ly - 1)].push_back(vt[i]);
		}
		for (int i = 0; i < ly; i++) sort(v[i].begin(), v[i].end());
		for (int i = 1; i < ly; i++) {
			if (v[i - 1].size() && v[i].size() && v[i - 1].back() < v[i][0]) {
				swap(v[i - 1][(int)v[i - 1].size() - 1], v[i][0]);
			}
		}
		for (auto e : v) for (auto l : e) A.push_back(l + 1);
		N = A.size();
	}

	void randomLayersReverse(int n, int ly) { // M = O(n * ly)
		A.push_back(1);
		vector<int> vt(n);
		iota(vt.begin(), vt.end(), 1);
		rnd.shuffle(vt.begin(), vt.end());
		vector<vector<int>> v(ly, vector<int>());
		for (int i = 0; i < vt.size(); i++) {
			v[rnd.nextInt(0, ly - 1)].push_back(vt[i]);
		}
		for (int i = 0; i < ly; i++) sort(v[i].rbegin(), v[i].rend());
		for (int i = 1; i < ly; i++) {
			if (v[i - 1].size() && v[i].size() && v[i - 1].back() > v[i][0]) {
				swap(v[i - 1][(int)v[i - 1].size() - 1], v[i][0]);
			}
		}
		for (auto e : v) for (auto l : e) A.push_back(l + 1);
		N = A.size();
	}

	void largeLayer(int n, int ly, int tp) { // same ev as randomLayers
		A.push_back(1);
		vector<int> vt(n);
		iota(vt.begin(), vt.end(), 1);
		rnd.shuffle(vt.begin(), vt.end());
		vector<vector<int>> v(ly, vector<int>());
		for (int i = 0; i < vt.size(); i++) {
			int x = rnd.nextInt(0, 2 * ly - 1);
			if (x < ly) x = tp;
			else {
				x = rnd.nextInt(0, ly - 2);
				if (x >= tp) x++;
			}
			v[x].push_back(vt[i]);
		}
		for (int i = 0; i < ly; i++) sort(v[i].begin(), v[i].end());
		for (int i = 1; i < ly; i++) {
			if (v[i - 1].size() && v[i].size() && v[i - 1].back() < v[i][0]) {
				swap(v[i - 1][(int)v[i - 1].size() - 1], v[i][0]);
			}
		}
		for (auto e : v) for (auto l : e) A.push_back(l + 1);
		N = A.size();
	}

	void largeLayerReverse(int n, int ly, int tp) { // same ev as randomLayersReverse
		A.push_back(1);
		vector<int> vt(n);
		iota(vt.begin(), vt.end(), 1);
		rnd.shuffle(vt.begin(), vt.end());
		vector<vector<int>> v(ly, vector<int>());
		for (int i = 0; i < vt.size(); i++) {
			int x = rnd.nextInt(0, 2 * ly - 1);
			if (x < ly) x = tp;
			else {
				x = rnd.nextInt(0, ly - 2);
				if (x >= tp) x++;
			}
			v[x].push_back(vt[i]);
		}
		for (int i = 0; i < ly; i++) sort(v[i].rbegin(), v[i].rend());
		for (int i = 1; i < ly; i++) {
			if (v[i - 1].size() && v[i].size() && v[i - 1].back() > v[i][0]) {
				swap(v[i - 1][(int)v[i - 1].size() - 1], v[i][0]);
			}
		}
		for (auto e : v) for (auto l : e) A.push_back(l + 1);
		N = A.size();

	}

	/* Set M parameters */
	void forceMin(bool imp = 0) {
		M = N - 1;
		if (imp) M--; // Force output -1
	}

	void forceMax(bool imp = 0, int win = -1) {
		int m = 0;
		bool isFirst = true;
		int inQueue = 1;
		for (int i = 1; i < N; i++) {
			if (!isFirst && (A[i] < A[i - 1])) {
				isFirst = true;
				inQueue -= 1;
			}

			m++;
			m += max(inQueue - 1, 0);
			if (m > MXM) break;
			isFirst = false;
			inQueue++;
		}

		m = min(m, MXM);

		if (imp) {
			if (m == MXM) M = MXM;
			else M = m + 1;
		} else {
			M = m;
		}

		if (win != -1) {
			M = rnd.nextInt(max(N - 1, m * (win - 1) / win), m);
		}
	}
};