#include <bits/stdc++.h>

using namespace std;

const int MXN = 1e3;

int N, M, pref[MXN + 69][MXN + 69];

inline int prefQry(int r, int c, int r2, int c2)
{
	r = max(r, 1);
	c = max(c, 1);
	r2 = min(r2, N);
	c2 = min(c2, M);
	
	return pref[r2][c2] - pref[r - 1][c2] - pref[r2][c - 1] + pref[r - 1][c - 1];
}

int main()
{
	scanf("%d%d", &N, &M);
	
	bitset<MXN + 69> A[N + 69];
	for(int i = 1; i <= N; i++)
	{
		for(int j = 1; j <= M; j++)
		{
			char ch;
			scanf(" %c", &ch);
			A[i][j] = ch - '0';
			pref[i][j] = pref[i - 1][j] + pref[i][j - 1] - pref[i - 1][j - 1] + A[i][j];
		}
	}
	
	int Q;
	scanf("%d", &Q);
	for(int qr = 0; qr < Q; qr++)
	{
		int R, C, K;
		scanf("%d%d%d", &R, &C, &K);
		
		int freq[max(N, M) + 69];
		for(int i = 0; i <= max(N, M); i++)
		{
			freq[i] = prefQry(R - i, C - i, R + i, C + i);
		}
		for(int i = max(N, M); i > 0; i--)
		{
			freq[i] -= freq[i - 1];
		}
		
		int ans = 0;
		for(int i = 0; i <= max(N, M); i++)
		{
			int w = min(freq[i], K);
			ans += w * i * 2;
			K -= w;
			if(K == 0)
			{
				break;
			}
		}
		printf("%d\n", ans);
	}
}
