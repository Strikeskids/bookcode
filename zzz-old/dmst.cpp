/* [dmst.cpp] */
#include <bits/stdc++.h>
using namespace std;
struct Edge {
	int u, v, w;
};
// O(VE) algorithm. Find the cheapest parent of each vertex.
// If there is a cycle, contract the cycle, and repeat.
int dmst(int n, vector<Edge> E, int root) {
	vector<int> label(n), used(n), cost(n), par(n);
	int ret = 0;
	for (;;) {
		int tot = 0;
		for (int i = 0; i < n; ++i) cost[i] = INT_MAX;
		for (auto e : E) {
			if (e.u == e.v) continue;
			if (e.w < cost[e.v]) {
				cost[e.v] = e.w;
				par[e.v] = e.u;
			}
		}
		cost[root] = 0;
		for (int i = 0; i < n; ++i) {
			if (cost[i] == INT_MAX) return INT_MAX;
			ret += cost[i];
			label[i] = used[i] = -1;
		}
		for (int i = 0; i < n; ++i) {
			int v = i;
			for (; v != root && used[v] == -1; v = par[v]) used[v] = i;
			if (used[v] == i) {
				// Cycle found.
				for (; label[v] == -1; v = par[v]) label[v] = tot;
				++tot;
				break;
			}
		}
		if (!tot) return ret;
		for (int i = 0; i < n; ++i)
			if (label[i] == -1)
				label[i] = tot++;
		for (auto &e : E) {
			if (e.u != e.v) e.w -= cost[e.v];
			e.u = label[e.u];
			e.v = label[e.v];
		}
		root = label[root];
		n = tot;
	}
}
void solve(int cn) {
	int N, M;
	scanf("%d%d", &N, &M);
	vector<Edge> E;
	for (int i = 0; i < M; ++i) {
		int u, v, w;
		scanf("%d%d%d", &u, &v, &w);
		E.push_back({u, v, w});
	}
	int ans = dmst(N, E, 0);
	printf("Case #%d: ", cn);
	if (ans == INT_MAX) printf("Possums!\n");
	else printf("%d\n", ans);
}
int main() {
	int T;
	scanf("%d", &T);
	for (int cn = 1; cn <= T; ++cn) {
		solve(cn);
	}
}


