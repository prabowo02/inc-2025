#include <bits/stdc++.h>

using namespace std;

int main()
{
	int T;
	scanf("%d", &T);
	
	for(int tc = 0; tc < T; tc++)
	{
		string S;
		int len;
		cin >> S;
		len = S.length();
		
		string ans = "";
		for(int i = 0; i < len; i++)
		{
			if(S[i] == 'c')
			{
				if(i + 1 < len && (S[i + 1] == 'e' || S[i + 1] == 'i' || S[i + 1] == 'y'))
				{
					ans += 's';
				}
				else if(i + 1 < len && (S[i + 1] == 'h'))
				{
					ans += 'c';
					i++;
				}
				else
				{
					ans += 'k';
				}
			}
			else
			{
				ans += S[i];
			}
		}
		
		printf("%s\n", ans.c_str());
	}
}
