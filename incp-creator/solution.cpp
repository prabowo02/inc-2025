#include <bits/stdc++.h>

using namespace std;

#define mp make_pair
#define fr first
#define sc second

const int MXN = 2e3;

bitset<MXN + 69> vis, vis2;
vector<int> adj[MXN + 69];
vector<pair<int, int>> ans;

void dfs(int u, int last = -1)
{
	vis[u] = true;
	vis2[u] = true;
	
	for(int i = 0; i < adj[u].size(); i++)
	{
		int v = adj[u][i];
		if(!vis[v])
		{
			ans.push_back({u, v});
			dfs(v, u);
		}
		else if(vis2[v] && v != last)
		{
			ans.push_back({u, v});
		}
	}
	
	vis2[u] = false;
}

int dfs2(int u)
{
	vis[u] = true;
	
	int cnt = 1;
	for(int i = 0; i < adj[u].size(); i++)
	{
		int v = adj[u][i];
		if(!vis[v])
		{
			cnt += dfs2(v);
		}
	}
	
	return cnt;
}

int main()
{
	int N, M;
	scanf("%d%d", &N, &M);
	
	pair<int, int> edges[M + 69];
	for(int i = 1; i <= M; i++)
	{
		int U, V;
		scanf("%d%d", &U, &V);
		edges[i] = {U, V};
	}
	
	int A[N + 69];
	for(int i = 1; i <= N; i++)
	{
		scanf("%d", A + i);
	}
	
	for(int i = 1; i <= M; i++)
	{
		int u, v;
		u = edges[i].fr;
		v = edges[i].sc;
		
		if(A[u] != A[v])
		{
			if(A[u] < A[v])
			{
				swap(u, v);
			}
			ans.push_back({u, v});
		}
		else
		{
			adj[u].push_back(v);
			adj[v].push_back(u);
		}
	}
	
	for(int i = 1; i <= N; i++)
	{
		if(!vis[i])
		{
			dfs(i);
		}
	}
	
	for(int i = 1; i <= N; i++)
	{
		adj[i].clear();
	}
	
	for(int i = 0; i < ans.size(); i++)
	{
		int u, v;
		u = ans[i].fr;
		v = ans[i].sc;
		adj[u].push_back(v);
	}
	
	for(int i = 1; i <= N; i++)
	{
		vis.reset();
		if(dfs2(i) != A[i])
		{
			printf("-1\n");
			return 0;
		}
	}
	
	for(int i = 0; i < ans.size(); i++)
	{
		int u, v;
		u = ans[i].fr;
		v = ans[i].sc;
		printf("%d %d\n", u, v);
	}
}
