/* [max-flow.cpp] */
// Max flow
#include <bits/stdc++.h>
using namespace std;
const int INF = 1000000000;
int A[105], B[105], clean[105][105], E[205][205], req[105], src, sink;
bool used[205];
int path(int x, int flow) {
	if (used[x]) return 0;
	used[x] = true;
	if (x == sink) return flow;
	for (int i = 0; i <= sink; ++i)
		if (E[x][i] > 0) {
			int resflow = path(i, min(flow, E[x][i]));
			if (!resflow) continue;
			E[x][i] -= resflow;
			E[i][x] += resflow;
			return resflow;
		}
	return 0;
}
int main() {
	int T;
	scanf("%d", &T);
	for (int cn = 1; cn <= T; ++cn) {
		int N, M, ans = 0;
		scanf("%d%d", &N, &M);
		src = N * 2;
		sink = N * 2 + 1;
		memset(E, 0, sizeof(E));
		for (int i = 0; i < N; ++i) {
			int s;
			scanf("%d%d%d", &A[i], &B[i], &s);
			req[i] = (s + M - 1) / M;
			ans += req[i];
			E[src][i * 2] = E[i * 2 + 1][sink] = req[i];
		}
		for (int i = 0; i < N; ++i)
			for (int j = 0; j < N; ++j)
				scanf("%d", &clean[i][j]);
		for (int i = 0; i < N; ++i)
			for (int j = 0; j < N; ++j)
				if (B[i] + clean[i][j] < A[j]) {
					E[i * 2][j * 2 + 1] = INF;
				}
		for (;;) {
			memset(used, 0, sizeof(used));
			int flow = path(src, INF);
			if (!flow) break;
			ans -= flow;
		}
		printf("Case %d: %d\n", cn, ans);
	}
}


