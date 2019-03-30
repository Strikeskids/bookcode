/* [binary-search-on-tree.cpp] */
// Take a vertex of every tree and destroy it. How many rounds does it take to destroy a tree?
// This is equivalent to labeling the vertices f(v) such that
// for every two vertex f(u) = f(v), there exists w on path such that f(w) > f(v).
// Root arbitrarily. For each subtree we will find the smallest visibility bitmask.
// Visibility bitmask is the set of labels that have not yet been resolved.
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double Double;
vector<int> E[300005];
int solve(int x, int p = 0) {
	int doubleMask = 0, mask = 0;
	for (int y : E[x]) {
		if (y == p) continue;
		int m = solve(y, x);
		doubleMask |= (mask & m);
		mask |= m;
	}
	int f = doubleMask ? 32 - __builtin_clz(doubleMask) : 0;
	mask |= (1 << f) - 1;
	return ++mask;
}
int main() {
	int N;
	scanf("%d", &N);
	for (int i = 1; i < N; ++i) {
		int u, v;
		scanf("%d%d", &u, &v);
		E[u].push_back(v);
		E[v].push_back(u);
	}
	int m = solve(1);
	printf("%d\n", 32 - __builtin_clz(m));
}


