/* [heavy-light.cpp] */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
const int MAXN = 100005;
const int MAXP = 17; // 2^MAXP >= MAXN
const int MAXS = 1 << 18; // MAXS >= 2*MAXN
int N;
int chainRoot[MAXN], heavy[MAXN], parent[MAXN], depth[MAXN], pos[MAXN], n;
int lcaSeq[MAXN * 2], lcaPos[MAXN], lcaN, lcaTable[MAXP + 1][MAXN * 2];
int curw, ans;
int mx[MAXS]; // Reset this per testcase
vector<int> adj[MAXN]; // Reset this per testcase
void update(int sn, int s, int e, int q) {
	if (s == e) {
		mx[sn] = curw;
		return;
	}
	int m = (s + e) / 2, lsn = sn * 2, rsn = sn * 2 + 1;
	if (q <= m) update(lsn, s, m, q);
	else update(rsn, m + 1, e, q);
	mx[sn] = max(mx[lsn], mx[rsn]);
}
void query(int sn, int s, int e, int qs, int qe) {
	if (qs <= s && e <= qe) {
		ans = max(ans, mx[sn]);
		return;
	}
	int m = (s + e) / 2, lsn = sn * 2, rsn = sn * 2 + 1, ret = 0;
	if (qs <= m) query(lsn, s, m, qs, qe);
	if (qe > m) query(rsn, m + 1, e, qs, qe);
}
// Move upwards from x to y (ancestor)
// A segment consists of contiguous edges. pos[x] stores the position in the
// heavy-light segment tree of the edge (x, parent[x]).
int moveUp(int x, int y, bool u = false) {
	for (;;) {
		bool sameChain = chainRoot[x] == chainRoot[y];
		int s = pos[chainRoot[x]];
		int e = pos[x];
		if (sameChain) s = pos[y] + 1;
		if (s <= e) {
			if (u) {
				update(1, 0, n - 1, s);
			} else {
				query(1, 0, n - 1, s, e);
			}
		}
		if (sameChain) break;
		x = parent[chainRoot[x]];
	}
}
int dfs(int x, int p = -1) {
	parent[x] = p;
	lcaPos[x] = lcaN;
	lcaSeq[lcaN++] = x;
	heavy[x] = -1;
	int siz = 1, maxsub = 0;
	for (auto y : adj[x]) {
		if (y == p) continue;
		depth[y] = depth[x] + 1;
		int subsize = dfs(y, x);
		if (subsize > maxsub) {
			maxsub = subsize;
			heavy[x] = y;
		}
		lcaSeq[lcaN++] = x;
		siz += subsize;
	}
	return siz;
}
// Chains are contiguous substrings of seg array.
void initHLD() {
	lcaN = n = 0;
	dfs(0);
	for (int x = 0; x < N; ++x)
		if (parent[x] == -1 || heavy[parent[x]] != x)
			for (int y = x; y != -1; y = heavy[y]) {
				chainRoot[y] = x;
				pos[y] = n++;
			}
	// Init LCA.
	for (int i = 0; i < lcaN; ++i) lcaTable[0][i] = lcaSeq[i];
	for (int p = 1; p <= MAXP; ++p)
		for (int i = 0; i < lcaN; ++i)
			if (i + (1 << p) - 1 < lcaN) {
				int x = lcaTable[p - 1][i];
				int y = lcaTable[p - 1][i + (1 << (p - 1))];
				lcaTable[p][i] = depth[x] < depth[y] ? x : y;
			}
}
int getLCA(int a, int b) {
	a = lcaPos[a];
	b = lcaPos[b];
	if (a > b) swap(a, b);
	int d = b - a + 1, p = 0;
	for (; (1 << (p + 1)) <= d; ++p);
	int x = lcaTable[p][a];
	int y = lcaTable[p][b - (1 << p) + 1];
	return depth[x] < depth[y] ? x : y;
}
pair<pair<int, int>, int> edges[MAXN];
int main() {
	int t;
	scanf("%d", &t);
	for (int asd = 0; asd < t; ++asd) {
		scanf("%d", &N);
		for (int i = 0; i < N; ++i) adj[i].clear();
		for (int i = 1; i < N; ++i) {
			int a, b, c;
			scanf("%d%d%d", &a, &b, &c);
			--a;
			--b;
			adj[a].push_back(b);
			adj[b].push_back(a);
			edges[i] = {{a, b}, c};
		}
		initHLD();
		memset(mx, 0, sizeof(mx));
		for (int i = 1; i < N; ++i) {
			int s = edges[i].first.first, t = edges[i].first.second;
			if (depth[s] > depth[t]) swap(s, t);
			curw = edges[i].second;
			moveUp(t, s, true);
		}
		int Q;
		scanf("%d", &Q);
		while (Q--) {
			int type, a, b;
			scanf("%d%d%d", &type, &a, &b);
			if (type == 1) {
				int s = edges[a].first.first, t = edges[a].first.second;
				if (depth[s] > depth[t]) swap(s, t);
				curw = b;
				moveUp(t, s, true);
			} else {
				--a;
				--b;
				ans = 0;
				int lca = getLCA(a, b);
				moveUp(a, lca);
				moveUp(b, lca);
				printf("%lld\n", ans);
			}
		}
	}
}

