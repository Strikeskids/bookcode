/* [matching.cpp] */
#include <bits/stdc++.h>
using namespace std;
bool E[1005][1005], used[1005];
int N, match[1005];
bool path(int x) {
	if (x == -1) return true;
	if (used[x]) return false;
	used[x] = true;
	for (int i = 0; i < N; ++i)
		if (E[x][i] && path(match[i])) {
			match[i] = x;
			return true;
		}
	return false;
}
int main() {
	scanf("%d", &N);
	for (int i = 0; i < N; ++i) {
		int k;
		scanf("%d", &k);
		for (int j = 0; j < k; ++j) {
			int a;
			scanf("%d", &a);
			E[i][a] = true;
		}
	}
	memset(match, -1, sizeof(match));
	int ans = N;
	for (int i = 0; i < N; ++i) {
		memset(used, 0, sizeof(used));
		if (path(i)) --ans;
	}
	printf("%d\n", ans);
}


