#include <bits/stdc++.h>

using namespace std;

int main()
{
	int N;
	scanf("%d", &N);
	
	int ans = 0;
	int y[N + 69];
	bool hasChange = false;
	bool direction;
	for(int i = 1; i <= N; i++)
	{
		scanf("%d", y + i);
		if(i > 1)
		{
			if(!hasChange)
			{
				if(y[i - 1] != y[i])
				{
					ans = i - 1;
					hasChange = true;
					direction = y[i - 1] < y[i];
				}
			}
			else
			{
				if((direction && y[i - 1] >= y[i]) || (!direction && y[i - 1] <= y[i]))
				{
					ans++;
					direction = !direction;
				}
			}
		}
	}
	printf("%d\n", ans);
}
