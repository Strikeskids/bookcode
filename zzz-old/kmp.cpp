/* [kmp.cpp] */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
char str[1000005], pat[1000005];
int B[1000005];
int main() {
	int TC;
	scanf("%d", &TC);
	while (TC--) {
		scanf("%s%s", str, pat);
		int n = strlen(pat), m = strlen(str);
		// define border B[i] = length of longest border of S[0..i]
		B[0] = 0;
		for (int i = 1; i < n; ++i) {
			int pre = B[i - 1];
			while (pre && pat[pre] != pat[i]) pre = B[pre - 1];
			if (pat[pre] == pat[i]) ++pre;
			B[i] = pre;
		}
		vector<int> ans;
		for (int i = 0, j = 0; i < m; ++i) {
			while (j && pat[j] != str[i]) j = B[j - 1];
			if (pat[j] == str[i]) ++j;
			if (j == n) ans.push_back(i - n + 1);
		}
		if (ans.empty()) printf("Not Found\n");
		else {
			printf("%d\n", ans.size());
			for (int a : ans) printf("%d ", a + 1);
			printf("\n");
		}
		printf("\n");
	}
}


