#include <bits/stdc++.h>

using namespace std;

#define mp make_pair
#define fr first
#define sc second

void ac()
{
	printf("AC\n");
	exit(0);
}

void wa()
{
	printf("WA\n");
	exit(0);
}

const int MXN = 2e3;

bitset<MXN + 69> vis;
vector<int> adj[MXN + 69];

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

int main(int argc, char* argv[])
{
	ifstream inp(argv[1]);
	ifstream out(argv[2]);
	ifstream con(argv[3]);
	
	int N, M;
	inp >> N >> M;
	
	map<pair<int, int>, bool> isEdge;
	for(int i = 0; i < M; i++)
	{
		int u, v;
		inp >> u >> v;
		if(u > v)
		{
			swap(u, v);
		}
		isEdge[{u, v}] = true;
	}
	
	int A[N + 69];
	for(int i = 1; i <= N; i++)
	{
		inp >> A[i];
	}
	
	int ans;
	out >> ans;
	if(ans == -1)
	{
		int conAns;
		if(!(con >> conAns))
		{
			wa();
		}
		if(conAns != -1)
		{
			wa();
		}
		if(con >> conAns)
		{
			wa();
		}
		ac();
	}
	
	for(int i = 0; i < M; i++)
	{
		int u, v;
		if(!(con >> u >> v))
		{
			wa();
		}
		if(!(1 <= min(u, v) && max(u, v) <= N))
		{
			wa();
		}
		
		adj[u].push_back(v);
		
		if(u > v)
		{
			swap(u, v);
		}
		if(!isEdge[{u, v}])
		{
			wa();
		}
		isEdge[{u, v}] = false;
	}
	
	for(int i = 1; i <= N; i++)
	{
		vis.reset();
		if(dfs(i) != A[i])
		{
			wa();
		}
	}
	
	ac();
}
