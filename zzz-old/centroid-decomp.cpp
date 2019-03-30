/* [centroid-decomp.cpp] */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
const int MAXN = 100005;
vector<int> E[MAXN];
int siz[MAXN];
void getSizes(int x, int p = -1) {
	siz[x] = 1;
	for (int y : E[x]) {
		if (y == p) continue;
		getSizes(y, x);
		siz[x] += siz[y];
	}
}
int findCentroid(int x, int n, int p = -1) {
	for (int y : E[x]) {
		if (y == p) continue;
		if (siz[y] > n / 2)
			return findCentroid(y, n, x);
	}
	return x;
}
void go(int x, int p = -1) {
	for (int y : E[x]) {
		if (y == p) continue;
		go(y, x);
	}
}
void decompose(int x) {
	getSizes(x);
	int c = findCentroid(x, siz[x]);
	for (int y : E[c]) {
		// deal with child subtree y
		// e.g. go(y, c); // c - parent to not visit
	}
	for (int y : E[c]) {
		for (auto it = E[y].begin();; ++it)
			if (*it == c) {
				E[y].erase(it);
				break;
			}
		decompose(y);
	}
}
int main() {
	int N;
	scanf("%d", &N);
	for (int i = 1; i < N; ++i) {
		int x, y;
		scanf("%d%d", &x, &y);
		E[x].push_back(y);
		E[y].push_back(x);
	}
	decompose(1);
}


