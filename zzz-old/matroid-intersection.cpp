/* [matroid-intersection.cpp] */
// Start with emptyset, augment one element at a time
// Get smallest-length path
// Exchange graph: (x,y) exists if S-x+y in M1, S-y+x in M2
#include <bits/stdc++.h>
using namespace std;
#define db(x) cerr << #x << "=" << x << endl
#define db2(x, y) cerr << #x << "=" << x << "," << #y << "=" << y << endl
#define db3(x, y, z) cerr << #x << "=" << x << "," << #y << "=" << y << "," << #z << "=" << z << endl
typedef long long ll;
typedef long double ld;
bool inb[55], ina[55], inset[55];
char M[55][55];
int dist[55], pre[55];
vector<int> E[55];
// Invariant: basis is in rref form from bottom to top
bool add(ll x, vector<ll> &basis) {
	int siz = basis.size();
	for (int i = siz - 1; i >= 0; --i) {
		x = min(x, x ^ basis[i]);
	}
	if (!x) return false;
	basis.insert(lower_bound(basis.begin(), basis.end(), x), x);
	return true;
}
int n;
bool ok1() {
	vector<ll> basis;
	for (int i = 1; i <= n; ++i) {
		if (!inb[i]) continue;
		ll x = 0;
		for (int j = 1; j <= n; ++j) {
			if (ina[j]) x = x * 2 + (M[j][i] == '1');
		}
		add(x, basis);
	}
	for (int i = 1; i <= n; ++i) {
		if (!inset[i]) continue;
		ll x = 0;
		for (int j = 1; j <= n; ++j) {
			if (ina[j]) x = x * 2 + (M[j][i] == '1');
		}
		if (!add(x, basis)) return false;
	}
	return true;
}
bool ok2() {
	vector<ll> basis;
	for (int i = 1; i <= n; ++i) {
		if (ina[i]) continue;
		ll x = 0;
		for (int j = 1; j <= n; ++j) {
			if (!inb[j] && !inset[j]) x = x * 2 + (M[i][j] == '1');
		}
		if (!add(x, basis)) return false;
	}
	return true;
}
int main() {
	int a, b;
	scanf("%d%d%d", &n, &a, &b);
	for (int i = 0; i < a; ++i) {
		int x;
		scanf("%d", &x);
		ina[x] = true;
	}
	for (int i = 0; i < b; ++i) {
		int x;
		scanf("%d", &x);
		inb[x] = true;
	}
	for (int i = 1; i <= n; ++i) scanf("%s", M[i] + 1);
	if (!ok2()) {
		printf("-1\n");
		return 0;
	}
	int rank;
	for (rank = 0; rank < n - b; ++rank) {
		for (int x = 0; x <= n + 1; ++x) E[x].clear();
		for (int x = 1; x <= n; ++x)
			if (inset[x])
				for (int y = 1; y <= n; ++y)
					if (!inset[y] && !inb[y]) {
						inset[x] = false;
						inset[y] = true;
						if (ok1()) E[x].push_back(y);
						if (ok2()) E[y].push_back(x);
						inset[y] = false;
						inset[x] = true;
					}
		int src = 0, sink = n + 1;
		for (int y = 1; y <= n; ++y) {
			if (inset[y] || inb[y]) continue;
			inset[y] = true;
			if (ok1()) E[src].push_back(y);
			if (ok2()) E[y].push_back(sink);
			inset[y] = false;
		}
		queue<int> Q;
		fill(dist, dist + n + 2, -1);
		dist[src] = 0;
		Q.push(src);
		while (!Q.empty()) {
			int x = Q.front();
			Q.pop();
			for (int y : E[x]) {
				if (dist[y] != -1) continue;
				dist[y] = dist[x] + 1;
				pre[y] = x;
				Q.push(y);
			}
		}
		if (dist[sink] == -1) break;
		for (int x = sink; x != src; x = pre[x])
			if (x != sink) inset[x] = !inset[x];
	}
	vector<ll> basis;
	for (int i = 1; i <= n; ++i) {
		if (!inset[i] && !inb[i]) continue;
		ll x = 0;
		for (int j = 1; j <= n; ++j) {
			if (ina[j]) x = x * 2 + (M[j][i] == '1');
		}
		add(x, basis);
	}
	if (basis.size() < a) printf("-1\n");
	else {
		printf("%d\n", rank);
		bool first = true;
		for (int i = 1; i <= n; ++i)
			if (inset[i]) {
				printf(first ? "%d" : " %d", i);
				first = false;
			}
		printf("\n");
	}
}


