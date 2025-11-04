#include <bits/stdc++.h>

using namespace std;

int main()
{
	int N;
	scanf("%d", &N);
	
	int A[N * 3 + 69];
	for(int i = 1; i <= N * 3; i++)
	{
		scanf("%d", A + i);
	}
	sort(A + 1, A + N * 3 + 1);
	printf("%d\n", A[N + 1]);
}
