/* [lca.cpp] */
#include <bits/stdc++.h>
using namespace std;
const int MAXN = 200005;
const int MAXP = 19; // 2^MAXP > 2*MAXN
vector<int> E[MAXN];
int H[MAXN], n, fir[MAXN], las[MAXN];
int table[MAXP][MAXN * 2];
int anc[MAXP][MAXN];
void go(int x, int p = -1) {
	fir[x] = n;
	table[0][n++] = x;
	for (auto y : E[x]) {
		if (y == p) continue;
		H[y] = H[x] + 1;
		go(y, x);
		table[0][n++] = x;
		anc[0][y] = x;
	}
	las[x] = n - 1;
}
int getLCA(int a, int b) {
	a = fir[a];
	b = fir[b];
	if (a > b) swap(a, b);
	int d = b - a + 1;
	int p = 31 - __builtin_clz(d);
	int x = table[p][a];
	int y = table[p][b - (1 << p) + 1];
	return H[x] < H[y] ? x : y;
}
// Power jumping function
int getMinDist(int a, int b) {
	int dh = H[a] - H[b];
	for (int p = MAXP - 1; p >= 0; --p) {
		if ((1 << p) <= dh) {
			dh -= 1 << p;
			a = anc[p][a];
		}
	}
	return ret;
}

// Tree helper functions (sum of subtree, is-ancestor, get-child-towards)
int pre[MAXN], post[MAXN], pp;
vector<pair<int, int>> ch[MAXN];
ll sum[MAXS], prop[MAXS];
void go(int x, int p = -1) {
	fir[x] = n;
	table[0][n++] = x;
	siz[x] = 1;
	pre[x] = ++pp;
	for (auto y : E[x]) {
		if (y == p) continue;
		H[y] = H[x] + 1;
		go(y, x);
		siz[x] += siz[y];
		table[0][n++] = x;
		ch[x].push_back({las[y], y});
	}
	las[x] = n - 1;
	post[x] = pp;
}
void update(int sn, int s, int e, int qs, int qe, ll x) {
	if (qs <= s && e <= qe) {
		sum[sn] += x * (e - s + 1);
		prop[sn] += x;
		return;
	}
	int m = (s + e) / 2, lsn = sn * 2, rsn = sn * 2 + 1;
	if (prop[sn]) {
		update(lsn, s, m, s, m, prop[sn]);
		update(rsn, m + 1, e, m + 1, e, prop[sn]);
		prop[sn] = 0;
	}
	if (qs <= m) update(lsn, s, m, qs, qe, x);
	if (qe > m) update(rsn, m + 1, e, qs, qe, x);
	sum[sn] = sum[lsn] + sum[rsn];
}
ll query(int sn, int s, int e, int qs, int qe) {
	if (qs <= s && e <= qe) {
		return sum[sn];
	}
	int m = (s + e) / 2, lsn = sn * 2, rsn = sn * 2 + 1;
	if (prop[sn]) {
		update(lsn, s, m, s, m, prop[sn]);
		update(rsn, m + 1, e, m + 1, e, prop[sn]);
		prop[sn] = 0;
	}
	ll ret = 0;
	if (qs <= m) ret += query(lsn, s, m, qs, qe);
	if (qe > m) ret += query(rsn, m + 1, e, qs, qe);
	return ret;
}
bool isAnc(int a, int b) {
	return fir[a] <= fir[b] && las[a] >= las[b];
}
int getChildTowards(int a, int sub) {
	auto it = lower_bound(ch[a].begin(), ch[a].end(), make_pair(las[sub], -1));
	return it->second;
}
// End tree helper functions

int main() {
	int N;
	scanf("%d", &N);
	for (int i = 1; i < N; ++i) {
		int a, b;
		scanf("%d%d", &a, &b);
		E[a].push_back(b);
		E[b].push_back(a);
	}
	go(1);
	for (int p = 1; p < MAXP; ++p)
		for (int i = 0; i < n; ++i)
			if (i + (1 << p) - 1 < n) {
				int x = table[p - 1][i];
				int y = table[p - 1][i + (1 << (p - 1))];
				table[p][i] = H[x] < H[y] ? x : y;
			}
	// Below is for power jumping
	for (int p = 1; p < MAXP; ++p)
		for (int i = 1; i <= N; ++i) {
			int a = anc[p - 1][i];
			if (a) {
				anc[p][i] = anc[p - 1][a];
			}
		}
	long long ans = 0;
	for (int i = 1; i <= N; ++i)
		for (int j = i + i; j <= N; j += i) {
			ans += H[i] + H[j] - 2 * H[getLCA(i, j)] + 1;
		}
	printf("%lld\n", ans);

	// Get the mini-tree from a subset of vertices vs
	// We only care about lca(vs[i - 1], vs[i]). Sort all the vertices, then add the edges
	bool cmpfir(int a, int b) {
		return fir[a] < fir[b];
	}
	vector<int> vs = {1, 2, 4};
	sort(vs.begin(), vs.end(), cmpfir);
	int origsiz = vs.size();
	for (int j = 0; j + 1 < origsiz; ++j)
		vs.push_back(getLCA(vs[j], vs[j + 1]));
	sort(vs.begin(), vs.end(), cmpfir);
	vs.resize(unique(vs.begin(), vs.end()) - vs.begin());
	stack<int> st;
	vector<pair<int, pair<int, int>>> es;
	for (int v : vs) {
		while (!st.empty() && las[st.top()] < las[v]) st.pop();
		if (!st.empty()) {
			int cost = getMaxWeight(v, st.top());
			es.push_back({cost, {v, st.top()}});
			cur -= cost;
		}
		st.push(v);
	}
}
/*
// Iterative DFS
void goIter() {
	stack<pair<int, int>> st;
	st.push({1, 0});
	while (!st.empty()) {
		auto z = st.top();
		int x = z.first, p = z.second;
		if (!cnt[x]) {
			fir[x] = n;
			seq[n++] = x;
		}
		if (cnt[x] == E[x].size()) {
			st.pop();
		} else {
			int y = E[x][cnt[x]];
			if (y != p) {
				seq[n++] = x;
				H[y] = H[x] + 1;
				st.push({y, x});
			}
			++cnt[x];
		}
	}
}
*/

