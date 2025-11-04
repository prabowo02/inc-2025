#include <bits/stdc++.h>
#include <tcframe/spec.hpp>
#include <tcrand/graph.hpp>

using namespace std;
using namespace tcframe;
using namespace tcrand;

#define mp make_pair
#define fr first
#define sc second

const int MXN = 2e3, MXM = 4e3;

class ProblemSpec : public BaseProblemSpec
{
	protected:
		int N, M, ans;
		vector<int> U, V, A;
		vector<int> ansU, ansV;

		void InputFormat()
		{
			LINE(N, M);
			LINES(U, V) % SIZE(M);
			LINE(A % SIZE(N));
		}
		void OutputFormat1()
		{
			LINE(ans);
		}
		void OutputFormat2()
		{
			LINES(ansU, ansV) % SIZE(M);
		}
		void GradingConfig()
		{
			TimeLimit(1);
			MemoryLimit(256);
		}
		void Constraints()
		{
			CONS(2 <= N && N <= MXN);
			CONS(1 <= M && M <= MXM);
			CONS(eachElementBetween(U, 1, N));
			CONS(eachElementBetween(V, 1, N));
			CONS(eachElementBetween(A, 1, N));
			CONS(noSelfLoops(U, V));
			CONS(noMultiEdges(U, V));
		}
		void StyleConfig()
		{
			CustomScorer();
		}
		void Subtask1()
		{
			Points(100);
		}

	private:
		bool eachElementBetween(vector<int> &curV, int lo, int hi)
		{
			for(int i = 0; i < curV.size(); i++)
			{
				if(!(lo <= curV[i] && curV[i] <= hi))
				{
					return false;
				}
			}
			return true;
		}
		bool noSelfLoops(vector<int> &curU, vector<int> &curV)
		{
			for(int i = 0; i < curU.size(); i++)
			{
				int u, v;
				u = curU[i];
				v = curV[i];
				if(u == v)
				{
					return false;
				}
			}
			return true;
		}
		bool noMultiEdges(vector<int> &curU, vector<int> &curV)
		{
			vector<pair<int, int>> edges;
			for(int i = 0; i < curU.size(); i++)
			{
				int u, v;
				u = curU[i];
				v = curV[i];
				if(u > v)
				{
					swap(u, v);
				}
				edges.push_back({u, v});
			}
			
			sort(edges.begin(), edges.end());
			for(int i = 0; i < (int)edges.size() - 1; i++)
			{
				if(edges[i] == edges[i + 1])
				{
					return false;
				}
			}
			return true;
		}
};

class TestSpec : public BaseTestSpec<ProblemSpec>
{
	protected:
		void SampleTestCase1()
		{
			Subtasks({1});
			Input
			({
				"6 7",
				"3 1",
				"1 4",
				"1 5",
				"3 2",
				"2 1",
				"3 4",
				"5 6",
				"6 1 6 6 2 1",
			});
			Output
			({
				"1 5",
				"3 2",
				"1 2",
				"5 6",
				"1 3",
				"3 4",
				"4 1",
			});
		}
		void SampleTestCase2()
		{
			Subtasks({1});
			Input
			({
				"3 2",
				"1 2",
				"2 3",
				"2 1 3",
			});
			Output
			({
				"-1",
			});
		}
		void SampleTestCase3()
		{
			Subtasks({1});
			Input
			({
				"3 1",
				"1 2",
				"2 1 1",
			});
			Output
			({
				"1 2",
			});
		}
		
		bool shufVertices, shufEdges, flipEdges;
		void BeforeTestCase()
		{
			U.clear();
			V.clear();
			A.clear();
			shufVertices = true;
			shufEdges = true;
			flipEdges = true;
		}
		void AfterTestCase()
		{
			if(shufVertices)
			{
				int perm[N + 69];
				for(int i = 1; i <= N; i++)
				{
					perm[i] = i;
				}
				rnd.shuffle(perm + 1, perm + N + 1);
				
				vector<int> curA(N);
				for(int i = 1; i <= N; i++)
				{
					curA[perm[i] - 1] = A[i - 1];
				}
				A = curA;
				
				for(int i = 0; i < U.size(); i++)
				{
					U[i] = perm[U[i]];
					V[i] = perm[V[i]];
				}
			}
			if(shufEdges)
			{
				for(int i = 0; i < U.size(); i++)
				{
					int p = rnd.nextInt(0, i);
					swap(U[i], U[p]);
					swap(V[i], V[p]);
				}
			}
			if(flipEdges)
			{
				for(int i = 0; i < U.size(); i++)
				{
					if(rnd.nextInt(0, 1))
					{
						swap(U[i], V[i]);
					}
				}
			}
		}
		void TestGroup1()
		{
			Subtasks({1});
			
			// Case (1-5) small N
			CASE(N = 4, M = 4, U = {4, 1, 4, 1}, V = {2, 3, 3, 2}, A = {2, 1, 4, 2}, shufVertices = false, shufEdges = false, flipEdges = false);
			CASE(N = 2, M = 1, U = {1}, V = {2}, A = {1, 2}, shufVertices = false, shufEdges = false, flipEdges = false);
			CASE(N = 3, M = 3, U = {2, 3, 2}, V = {1, 1, 3}, A = {3, 3, 3}, shufVertices = false, shufEdges = false, flipEdges = false);
			CASE(N = 4, M = 4, U = {1, 4, 1, 4}, V = {2, 2, 3, 3}, A = {2, 1, 3, 2}, shufVertices = false, shufEdges = false, flipEdges = false);
			CASE(N = 4, M = 4, U = {1, 2, 3, 2}, V = {2, 3, 1, 4}, A = {4, 4, 4, 4}, shufVertices = false, shufEdges = false, flipEdges = false);
			
			// Case (6-7) rand graph
			CASE(N = rnd.nextInt(2, MXN), M = rnd.nextInt(N - 1, MXM), rndConnectedGraph(N, M), A = rndArray(N, 1, N));
			CASE(N = MXN, M = MXM, rndConnectedGraph(N, M), A = rndArray(N, 1, N));
			
			// Case (8-10) rand graph yes
			CASE(N = rnd.nextInt(2, MXN), M = rnd.nextInt(1, MXM), rndConnectedGraph(N, M), genA(N, U, V));
			CASE(N = MXN, M = MXM, rndConnectedGraph(N, M), genA(N, U, V));
			CASE(N = MXN, M = MXM, rndGraph(N, M), genA(N, U, V));
			
			// Case (11-16) rand DAG
			CASE(N = MXN, M = MXM, rndDAG(N, M), genA(N, U, V));
			CASE(N = MXN, M = MXM, wrndDAG(N, M, -3), genA(N, U, V));
			CASE(N = MXN, M = MXM, wrndDAG(N, M, 3), genA(N, U, V));
			CASE(N = MXN, M = MXM, minLengthDAG(N, M), genA(N, U, V));
			CASE(N = MXN, M = MXM, maxLengthDAG(N, M), genA(N, U, V));
			CASE(N = MXN, M = MXM, minLengthDAG(N, M, 2), genA(N, U, V));
			
			// Case (17) line graph
			CASE(N = MXN, M = N - 1, lineGraph(N), genA(N, U, V));
			
			// Case (18) star graph
			CASE(N = MXN, M = N - 1, starGraph(N), genA(N, U, V));
			
			// Case (19) complete graph
			CASE(N = MXN, M = 63 * 62 / 2, completeGraph(63), genA(N, U, V));
			
			// Case (20) all A same
			CASE(N = MXN, M = MXM, rndConnectedGraph(N, M), A = rndArray(N, N, N));
			
			// Case (21) biconnected component
			CASE(N = MXN, M = MXM, rndBCC(N, M), A = rndArray(N, N, N));
			
			// Case (22) almost biconnected component
			CASE(N = MXN, M = MXM, rndAlmostBCC(N, M), A = rndArray(N, N, N));
			
			// Case (23) DAG of connected components
			CASE(N = MXN, M = MXM, DAGOfCCs(N, M - M / 3, M / 3, N / 10, -3), genA(N, U, V, true));
			
			// Case (24-28) DAG of biconnected components
			CASE(N = MXN, M = MXM, DAGOfBCCs(N, M - M / 3, M / 3, N / 3, -3), genA(N, U, V, true));
			CASE(N = MXN, M = MXM, DAGOfBCCs(N, M - M / 3, M / 3, N / 10, -3), genA(N, U, V, true));
			CASE(N = MXN, M = MXM, DAGOfBCCs(N, M - M / 3, M / 3, sqrt(N), -3), genA(N, U, V, true));
			CASE(N = MXN, M = MXM, DAGOfBCCs(N, M - M / 3, M / 3, 10, -3), genA(N, U, V, true));
			CASE(N = MXN, M = MXM, DAGOfBCCs(N, M - M / 3, M / 3, 2, -3), genA(N, U, V, true));
			
			// Case (29) DAG of biconnected components except one
			CASE(N = MXN, M = MXM, DAGOfBCCsExceptOne(N, M - M / 3, M / 3, N / 10, -3), genA(N, U, V, true));
			
			// Case (30-33) rand DAG almost yes
			CASE(N = MXN, M = MXM, wrndDAG(N, M, -3), genA(N, U, V), updateA(N, -1, -1));
			CASE(N = MXN, M = MXM, wrndDAG(N, M, -3), genA(N, U, V), updateA(N, -1, 1));
			CASE(N = MXN, M = MXM, wrndDAG(N, M, -3), genA(N, U, V), updateA(N, 1, -1));
			CASE(N = MXN, M = MXM, wrndDAG(N, M, -3), genA(N, U, V), updateA(N, 1, 1));
			
			// Case (34-37) DAG of biconnected components almost yes
			CASE(N = MXN, M = MXM, DAGOfBCCs(N, M - M / 3, M / 3, N / 10, -3), genA(N, U, V, true), updateA(N, -1, -1, true));
			CASE(N = MXN, M = MXM, DAGOfBCCs(N, M - M / 3, M / 3, N / 10, -3), genA(N, U, V, true), updateA(N, -1, 1, true));
			CASE(N = MXN, M = MXM, DAGOfBCCs(N, M - M / 3, M / 3, N / 10, -3), genA(N, U, V, true), updateA(N, 1, -1, true));
			CASE(N = MXN, M = MXM, DAGOfBCCs(N, M - M / 3, M / 3, N / 10, -3), genA(N, U, V, true), updateA(N, 1, 1, true));
			
			// Case (38) flower graph
			CASE(N = (MXN - 1) / 2 * 2 + 1, M = N / 2 * 3, flowerGraph(N), A = rndArray(N, N, N));
			
			// Case (39) diamond graph
			CASE(N = MXN, M = N * 2 - 3, diamondGraph(N), A = rndArray(N, N, N));
			
			// Case (40) cycle chain graph
			CASE(N = (MXN - 1) / 2 * 2 + 1, M = N / 2 * 3, cycleChainGraph(N), A = rndArray(N, N, N));
			
			// Case (41) line of articulation points
			CASE(N = MXN, M = MXM, lineOfArticulationPoints(N, M, N / 10), A = rndArray(N, N, N));
			
			// Case (42) tree of articulation points
			CASE(N = MXN, M = MXM, treeOfArticulationPoints(N, M, N / 10), A = rndArray(N, N, N));
			
			// Case (43-44) biconnected component made by intersecting intervals
			CASE(N = MXN, M = N - 1 + N / 2 - 1, nestedIntervalBCC(N), A = rndArray(N, N, N), shufVertices = false, shufEdges = false, flipEdges = false);
			CASE(N = MXN, M = N - 1 + N / 2, commonIntersectionIntervalBCC(N), A = rndArray(N, N, N), shufVertices = false, shufEdges = false, flipEdges = false);
			
			// Case (45) 1 big cycle
			CASE(N = MXN, M = N, cycleGraph(N), genA(N, U, V));
			
			// Case (46-47) small M
			CASE(N = MXN, M = 1, rndGraph(N, M), genA(N, U, V));
			CASE(N = MXN, M = N / 2, rndGraph(N, M), genA(N, U, V));
		}

	private:
		int wrnd(int lo, int hi, int w)
		{
			int ret = rnd.nextInt(lo, hi);
			
			if(w > 0)
			{
				for(int i = 0; i < w; i++)
				{
					ret = max(ret, rnd.nextInt(lo, hi));
				}
			}
			else
			{
				for(int i = 0; i < -w; i++)
				{
					ret = min(ret, rnd.nextInt(lo, hi));
				}
			}
			return ret;
		}
		vector<int> rndArray(int sz, int lo, int hi)
		{
			vector<int> ret;
			for(int i = 0; i < sz; i++)
			{
				ret.push_back(rnd.nextInt(lo, hi));
			}
			return ret;
		}
		vector<int> nonDecreasingArray(int sz, int lo, int hi)
		{
			vector<int> ret = rndArray(sz, lo, hi);
			sort(ret.begin(), ret.end());
			return ret;
		}
		vector<int> increasingArray(int sz, int lo, int hi)
		{
			vector<int> ret = nonDecreasingArray(sz, lo, hi - (sz - 1));
			for(int i = 0; i < sz; i++)
			{
				ret[i] += i;
			}
			return ret;
		}
		vector<int> distinctArray(int sz, int lo, int hi)
		{
			vector<int> ret = increasingArray(sz, lo, hi);
			rnd.shuffle(ret.begin(), ret.end());
			return ret;
		}
		
		void rndGraph(int curN, int curM)
		{
			map<pair<int, int>, bool> isEdge;
			for(int i = 0; i < curM; i++)
			{
				int u, v;
				while(true)
				{
					u = rnd.nextInt(1, curN);
					v = rnd.nextInt(1, curN - 1);
					if(v >= u)
					{
						v++;
					}
					if(!isEdge[{u, v}])
					{
						break;
					}
				}
				
				U.push_back(u);
				V.push_back(v);
				isEdge[{u, v}] = true;
				isEdge[{v, u}] = true;
			}
		}
		void rndConnectedGraph(int curN, int curM)
		{
			vector<int> curU, curV;
			
			GraphRandomizer graph_rand;
			tie(curU, curV) = graph_rand.node_count(curN).edge_count(curM).index_base(1).next().edges();
			U.insert(U.end(), curU.begin(), curU.end());
			V.insert(V.end(), curV.begin(), curV.end());
		}
		void rndDAG(int curN, int curM)
		{
			map<pair<int, int>, bool> isEdge;
			for(int i = 0; i < curM; i++)
			{
				int u, v;
				while(true)
				{
					u = rnd.nextInt(1, curN);
					v = rnd.nextInt(1, curN - 1);
					if(v >= u)
					{
						v++;
					}
					else
					{
						swap(u, v);
					}
					if(!isEdge[{u, v}])
					{
						break;
					}
				}
				
				U.push_back(u);
				V.push_back(v);
				isEdge[{u, v}] = true;
			}
		}
		void wrndDAG(int curN, int curM, int w)
		{
			map<pair<int, int>, bool> isEdge;
			for(int i = 0; i < curM; i++)
			{
				int u, v;
				while(true)
				{
					int len = wrnd(1, curN - 1, w);
					u = rnd.nextInt(1, curN - len);
					v = u + len;
					if(!isEdge[{u, v}])
					{
						break;
					}
				}
				
				U.push_back(u);
				V.push_back(v);
				isEdge[{u, v}] = true;
			}
		}
		void minLengthDAG(int curN, int curM, int minLen = 1)
		{
			int cntEdges = 0;
			for(int len = minLen; true; len++)
			{
				for(int i = 1; i <= curN - len; i++)
				{
					U.push_back(i);
					V.push_back(i + len);
					cntEdges++;
					if(cntEdges >= curM)
					{
						return;
					}
				}
			}
		}
		void maxLengthDAG(int curN, int curM)
		{
			int cntEdges = 0;
			for(int len = curN - 1; true; len--)
			{
				for(int i = 1; i <= curN - len; i++)
				{
					U.push_back(i);
					V.push_back(i + len);
					cntEdges++;
					if(cntEdges >= curM)
					{
						return;
					}
				}
			}
		}
		void lineGraph(int curN)
		{
			for(int i = 1; i <= curN - 1; i++)
			{
				U.push_back(i);
				V.push_back(i + 1);
			}
		}
		void starGraph(int curN)
		{
			for(int i = 2; i <= curN; i++)
			{
				U.push_back(1);
				V.push_back(i);
			}
		}
		void completeGraph(int curN)
		{
			for(int i = 1; i <= curN; i++)
			{
				for(int j = i + 1; j <= curN; j++)
				{
					int u, v;
					u = i;
					v = j;
					if(rnd.nextInt(0, 1))
					{
						swap(u, v);
					}
					U.push_back(u);
					V.push_back(v);
				}
			}
		}
		void rndBCC(int curN, int curM)
		{
			int cntCycles = 1, extra = 0;
			for(int i = 0; i < curM - curN; i++)
			{
				if(rnd.nextInt(0, 1) && cntCycles < curN - 2)
				{
					cntCycles++;
				}
				else
				{
					extra++;
				}
			}
			
			vector<int> cyclePos = increasingArray(cntCycles - 1, 3, curN - 1);
			cyclePos.push_back(1);
			cyclePos.push_back(curN);
			sort(cyclePos.begin(), cyclePos.end());
			
			vector<pair<int, int>> edges;
			for(int i = 1; i <= cntCycles; i++)
			{
				int cur, last;
				cur = cyclePos[i];
				last = cyclePos[i - 1];
				
				int p, p2;
				p = rnd.nextInt(1, last);
				p2 = rnd.nextInt(1, last);
				if(cur == last + 1)
				{
					p2 = rnd.nextInt(1, last - 1);
					if(p2 >= p)
					{
						p2++;
					}
				}
				
				edges.push_back({p, last + 1});
				for(int j = last + 1; j <= cur - 1; j++)
				{
					edges.push_back({j, j + 1});
				}
				edges.push_back({cur, p2});
			}
			
			map<pair<int, int>, bool> isEdge;
			for(int i = 0; i < edges.size(); i++)
			{
				int u, v;
				u = edges[i].fr;
				v = edges[i].sc;
				isEdge[{u, v}] = true;
				isEdge[{v, u}] = true;
			}
			
			for(int i = 0; i < extra; i++)
			{
				int u, v;
				while(true)
				{
					u = rnd.nextInt(1, curN);
					v = rnd.nextInt(1, curN - 1);
					if(v >= u)
					{
						v++;
					}
					if(!isEdge[{u, v}])
					{
						break;
					}
				}
				
				edges.push_back({u, v});
				isEdge[{u, v}] = true;
				isEdge[{v, u}] = true;
			}
			
			for(int i = 0; i < edges.size(); i++)
			{
				int u, v;
				u = edges[i].fr;
				v = edges[i].sc;
				U.push_back(u);
				V.push_back(v);
			}
		}
		void rndAlmostBCC(int curN, int curM)
		{
			int cntVertices = 3;
			for(int i = 3; i < curN - 3; i++)
			{
				if(rnd.nextInt(0, 1))
				{
					cntVertices++;
				}
			}
			
			int cntEdges = cntVertices;
			for(int i = cntVertices; i < curM - 1 - (curN - cntVertices); i++)
			{
				if(rnd.nextInt(0, 1))
				{
					cntEdges++;
				}
			}
			
			cntEdges = min(cntEdges, cntVertices * (cntVertices - 1) / 2);
			cntEdges = max(cntEdges, curM - 1 - (curN - cntVertices) * (curN - cntVertices - 1) / 2);
			
			rndBCC(cntVertices, cntEdges);
			rndBCC(curN - cntVertices, curM - 1 - cntEdges);
			for(int i = 0; i < curM - 1 - cntEdges; i++)
			{
				U[U.size() - 1 - i] += cntVertices;
				V[V.size() - 1 - i] += cntVertices;
			}
			U.push_back(cntVertices);
			V.push_back(cntVertices + 1);
		}
		void flowerGraph(int curN)
		{
			for(int i = 2; i <= curN; i += 2)
			{
				U.push_back(1);
				V.push_back(i);
				U.push_back(i);
				V.push_back(i + 1);
				U.push_back(i + 1);
				V.push_back(1);
			}
		}
		void diamondGraph(int curN)
		{
			U.push_back(1);
			V.push_back(2);
			for(int i = 3; i <= curN; i++)
			{
				U.push_back(1);
				V.push_back(i);
				U.push_back(i);
				V.push_back(2);
			}
		}
		void cycleChainGraph(int curN)
		{
			for(int i = 2; i <= curN; i += 2)
			{
				U.push_back(i - 1);
				V.push_back(i);
				U.push_back(i);
				V.push_back(i + 1);
				U.push_back(i + 1);
				V.push_back(i - 1);
			}
		}
		void nestedIntervalBCC(int curN)
		{
			for(int i = 1; i <= curN - 1; i++)
			{
				U.push_back(i);
				V.push_back(i + 1);
			}
			
			for(int i = 1; i <= curN / 2 - 1; i++)
			{
				U.push_back(curN + 1 - i);
				V.push_back(i);
			}
		}
		void commonIntersectionIntervalBCC(int curN)
		{
			for(int i = 1; i <= curN - 1; i++)
			{
				U.push_back(i);
				V.push_back(i + 1);
			}
			
			vector<int> curU, curV;
			curU = distinctArray(curN / 2, curN + 1 - curN / 2, curN);
			curV = distinctArray(curN / 2, 1, curN / 2);
			
			U.insert(U.end(), curU.begin(), curU.end());
			V.insert(V.end(), curV.begin(), curV.end());
		}
		void cycleGraph(int curN)
		{
			for(int i = 1; i <= curN; i++)
			{
				U.push_back(i);
				V.push_back(i % curN + 1);
			}
		}
		
		int SCCSizes[MXN + 69], SCCEdges[MXN + 69], prefSizes[MXN + 69], SCCLeft[MXN + 69], SCCRight[MXN + 69];
		void genSCCSizes(int cnt, int cntVertices)
		{
			for(int i = 1; i <= cnt; i++)
			{
				SCCSizes[i] = 1;
			}
			
			int c = cntVertices - cnt;
			while(c > 0)
			{
				int p = rnd.nextInt(1, cnt);
				if(SCCSizes[p] == 1)
				{
					if(c < 2)
					{
						continue;
					}
					SCCSizes[p] += 2;
					c -= 2;
				}
				else
				{
					SCCSizes[p]++;
					c--;
				}
			}
			
			prefSizes[0] = 0;
			for(int i = 1; i <= cnt; i++)
			{
				prefSizes[i] = prefSizes[i - 1] + SCCSizes[i];
			}
		}
		void genBlockCutSizes(int cnt, int cntVertices)
		{
			for(int i = 1; i <= cnt; i++)
			{
				SCCSizes[i] = 3;
			}
			
			int c = cntVertices - 1 - cnt * 2;
			for(int i = 0; i < c; i++)
			{
				int p = rnd.nextInt(1, cnt);
				SCCSizes[p]++;
			}
		}
		void genSCCEdges(int cnt, int cntVertices, int cntEdges, bool isBCC = true)
		{
			int c = cntEdges;
			for(int i = 1; i <= cnt; i++)
			{
				if(SCCSizes[i] == 1)
				{
					SCCEdges[i] = 0;
				}
				else
				{
					SCCEdges[i] = SCCSizes[i];
					if(!isBCC)
					{
						SCCEdges[i]--;
					}
				}
				c -= SCCEdges[i];
			}
			
			while(c > 0)
			{
				int p, p2;
				p = rnd.nextInt(1, cntVertices);
				p2 = lower_bound(prefSizes + 1, prefSizes + cnt + 1, p) - prefSizes;
				if(SCCEdges[p2] >= SCCSizes[p2] * (SCCSizes[p2] - 1) / 2)
				{
					continue;
				}
				SCCEdges[p2]++;
				c--;
			}
		}
		void genSCCRanges(int cnt)
		{
			int p = 0;
			for(int i = 1; i <= cnt; i++)
			{
				for(int j = 1; j <= SCCSizes[i]; j++)
				{
					SCCLeft[p + j] = p + 1;
					SCCRight[p + j] = p + SCCSizes[i];
				}
				p += SCCSizes[i];
			}
		}
		void genDAGOfSCCs(int cntVertices, int cntEdges, int w)
		{
			map<pair<int, int>, bool> isEdge;
			for(int i = 0; i < cntEdges; i++)
			{
				int u, v;
				while(true)
				{
					int len = wrnd(1, cntVertices - 1, w);
					u = rnd.nextInt(1, cntVertices - len);
					v = u + len;
					if(SCCLeft[u] != SCCLeft[v] && !isEdge[{u, v}])
					{
						break;
					}
				}
				
				U.push_back(u);
				V.push_back(v);
				isEdge[{u, v}] = true;
			}
		}
		void DAGOfCCs(int curN, int curM, int curM2, int cnt, int w)
		{
			genSCCSizes(cnt, curN);
			genSCCEdges(cnt, curN, curM, false);
			genSCCRanges(cnt);
			genDAGOfSCCs(curN, curM2, w);
			
			for(int i = 1; i <= cnt; i++)
			{
				if(SCCSizes[i] > 1)
				{
					rndConnectedGraph(SCCSizes[i], SCCEdges[i]);
					for(int j = 0; j < SCCEdges[i]; j++)
					{
						U[U.size() - 1 - j] += prefSizes[i - 1];
						V[V.size() - 1 - j] += prefSizes[i - 1];
					}
				}
			}
		}
		void DAGOfBCCs(int curN, int curM, int curM2, int cnt, int w)
		{
			genSCCSizes(cnt, curN);
			genSCCEdges(cnt, curN, curM);
			genSCCRanges(cnt);
			genDAGOfSCCs(curN, curM2, w);
			
			for(int i = 1; i <= cnt; i++)
			{
				if(SCCSizes[i] > 1)
				{
					rndBCC(SCCSizes[i], SCCEdges[i]);
					for(int j = 0; j < SCCEdges[i]; j++)
					{
						U[U.size() - 1 - j] += prefSizes[i - 1];
						V[V.size() - 1 - j] += prefSizes[i - 1];
					}
				}
			}
		}
		void DAGOfBCCsExceptOne(int curN, int curM, int curM2, int cnt, int w)
		{
			genSCCSizes(cnt, curN);
			genSCCEdges(cnt, curN, curM);
			genSCCRanges(cnt);
			genDAGOfSCCs(curN, curM2, w);
			
			int maxSize = 0;
			for(int i = 1; i <= cnt; i++)
			{
				maxSize = max(maxSize, SCCSizes[i]);
			}
			
			bool isNo = false;
			for(int i = 1; i <= cnt; i++)
			{
				if(SCCSizes[i] > 1)
				{
					if(SCCSizes[i] == maxSize && !isNo)
					{
						isNo = true;
						rndAlmostBCC(SCCSizes[i], SCCEdges[i]);
					}
					else
					{
						rndBCC(SCCSizes[i], SCCEdges[i]);
					}
					for(int j = 0; j < SCCEdges[i]; j++)
					{
						U[U.size() - 1 - j] += prefSizes[i - 1];
						V[V.size() - 1 - j] += prefSizes[i - 1];
					}
				}
			}
		}
		void lineOfArticulationPoints(int curN, int curM, int cnt)
		{
			genBlockCutSizes(cnt, curN);
			genSCCEdges(cnt, curN, curM);
			
			int p = 1;
			for(int i = 1; i <= cnt; i++)
			{
				rndBCC(SCCSizes[i], SCCEdges[i]);
				for(int j = 0; j < SCCEdges[i]; j++)
				{
					U[U.size() - 1 - j] += p - 1;
					V[V.size() - 1 - j] += p - 1;
				}
				p += SCCSizes[i] - 1;
			}
		}
		void treeOfArticulationPoints(int curN, int curM, int cnt)
		{
			genBlockCutSizes(cnt, curN);
			genSCCEdges(cnt, curN, curM);
			
			int p = 1;
			for(int i = 1; i <= cnt; i++)
			{
				rndBCC(SCCSizes[i], SCCEdges[i]);
				
				int par = rnd.nextInt(1, p);
				for(int j = 0; j < SCCEdges[i]; j++)
				{
					if(U[U.size() - 1 - j] == 1)
					{
						U[U.size() - 1 - j] = par;
					}
					else
					{
						U[U.size() - 1 - j] += p - 1;
					}
					
					if(V[V.size() - 1 - j] == 1)
					{
						V[V.size() - 1 - j] = par;
					}
					else
					{
						V[V.size() - 1 - j] += p - 1;
					}
				}
				
				p += SCCSizes[i] - 1;
			}
		}
		
		vector<int> adj[MXN + 69];
		bitset<MXN + 69> vis;
		int dfs(int u)
		{
			vis[u] = true;
			
			int cnt = 1;
			for(int i = 0; i < adj[u].size(); i++)
			{
				int v = adj[u][i];
				if(!vis[v])
				{
					cnt += dfs(v);
				}
			}
			
			return cnt;
		}
		void genA(int curN, vector<int> &curU, vector<int> &curV, bool mergeRanges = false)
		{
			for(int i = 1; i <= curN; i++)
			{
				adj[i].clear();
			}
			
			for(int i = 0; i < curU.size(); i++)
			{
				int u, v;
				u = curU[i];
				v = curV[i];
				adj[u].push_back(v);
			}
			
			if(mergeRanges)
			{
				for(int i = 1; i <= curN; i++)
				{
					if(SCCLeft[i] == i)
					{
						adj[i].push_back(SCCRight[i]);
					}
					else
					{
						adj[i].push_back(i - 1);
					}
				}
			}
			
			for(int i = 1; i <= curN; i++)
			{
				vis.reset();
				A.push_back(dfs(i));
			}
		}
		void updateA(int curN, int x, int w, bool mergeRanges = false)
		{
			if(x == -1)
			{
				x = rnd.nextInt(1, curN);
			}
			
			while(!(1 <= A[x - 1] + w && A[x - 1] + w <= curN))
			{
				x = x % curN + 1;
			}
			
			if(!mergeRanges)
			{
				A[x - 1] += w;
			}
			else
			{
				for(int i = SCCLeft[x]; i <= SCCRight[x]; i++)
				{
					A[i - 1] += w;
				}
			}
		}
};
