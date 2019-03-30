/* [dp-tree.cpp] */
#include <bits/stdc++.h>
using namespace std;
vector<int> ch[2505];
int F[2505][2505];
int G[2505];
const int INF = 1000000000;
int siz[2505];
void go(int x) {
	siz[x] = 1;
	for (auto y : ch[x]) {
		go(y);
		siz[x] += siz[y];
	}
	for (int s = 0; s <= siz[x]; ++s) G[s] = -INF;
	G[0] = 0;
	int csiz = 0;
	for (auto y : ch[x]) {
		for (int s = csiz; s >= 0; --s)
			for (int t = 1; t <= siz[y]; ++t) // Assumes subtree size >= 1
				G[s + t] = max(G[s] + F[y][t], G[s + t]);
		csiz += siz[y];
	}
	F[x][0] = 0;
	for (int s = 0; s <= csiz; ++s)
		F[x][s + 1] = G[s] + val;
}
int main() {
	int K, N;
	scanf("%d%d", &K, &N);
	for (int i = 1; i <= N; ++i) {
		int r;
		scanf("%d%d%d", &S[i], &P[i], &r);
		ch[r].push_back(i);
	}
	go(1);
}


