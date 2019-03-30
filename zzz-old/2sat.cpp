/* [2sat.cpp] */
#include <bits/stdc++.h>
using namespace std;
const int MAXN = 12005; // This is 2 * N
struct SAT2 {
	// Assume variables are 2i and 2i + 1
	vector<int> E[MAXN], ER[MAXN];
	int comp[MAXN], ans[MAXN], topo[MAXN];
	bool used[MAXN];
	stack<int> S;
	void reset(int N) {
		for (int i = 0; i < 2 * N; ++i) {
			E[i].clear();
			ER[i].clear();
		}
		memset(used, 0, sizeof(used));
	}
	void addConstraint(int a, int b) {
		E[a ^ 1].push_back(b);
		E[b ^ 1].push_back(a);
	}
	void dfs(int x) {
		if (used[x]) return;
		used[x] = true;
		for (int y : E[x]) dfs(y);
		S.push(x);
	}
	void dfs2(int x, int c) {
		if (comp[x] != -1) return;
		comp[x] = c;
		for (int y : ER[x]) dfs2(y, c);
	}
	bool solve(int N) {
		for (int x = 0; x < 2 * N; ++x) dfs(x);
		for (int x = 0; x < 2 * N; ++x)
			for (int y : E[x]) ER[y].push_back(x);
		memset(comp, -1, sizeof(comp));
		int tn = 0;
		while (!S.empty()) {
			int x = S.top();
			S.pop();
			dfs2(x, x);
			topo[x] = tn++;
		}
		for (int i = 0; i < N; ++i) {
			if (comp[2 * i] == comp[2 * i + 1])
				return false;
			ans[i] = topo[comp[2 * i]] < topo[comp[2 * i + 1]];
		}
		return true;
	}
} sat2;
int main() {
	int N, M;
	scanf("%d%d", &N, &M);
	sat2.addConstraint(a * 2, b * 2 + 1);
	sat2.solve();
}


