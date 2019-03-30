/* [weighted-matroid-intersection.cpp] */
// Start with emptyset, augment one element at a time
// Get smallest-weight path, break ties with smallest length
// Exchange graph: (x,y) exists if S-x+y in M1, S-y+x in M2
#include <bits/stdc++.h>
using namespace std;
#define db(x) cerr << #x << "=" << x << endl
#define db2(x, y) cerr << #x << "=" << x << "," << #y << "=" << y << endl
#define db3(x, y, z) cerr << #x << "=" << x << "," << #y << "=" << y << "," << #z << "=" << z << endl
typedef long long ll;
typedef long double ld;
bool inset[105];
char C[105];
int A[105], B[105], W[105], ans[105];
int par[105], dist[105], pre[105];
const int INF = 1000000000;
int root(int a) {
	if (a == par[a]) return a;
	return par[a] = root(par[a]);
}
int N, M;
bool ok(char t) {
	for (int i = 1; i <= N; ++i) par[i] = i;
	int cnt = 0;
	for (int x = 0; x < M; ++x) {
		if (inset[x]) continue;
		if (C[x] != 'G' && C[x] != t) continue;
		int a = root(A[x]), b = root(B[x]);
		if (a == b) continue;
		par[a] = b;
		++cnt;
	}
	return cnt == N - 1;
}
int main() {
	scanf("%d%d", &N, &M);
	int totw = 0;
	for (int i = 0; i < M; ++i) {
		scanf("%d%d%d %c", &A[i], &B[i], &W[i], &C[i]);
		totw += W[i];
	}
	memset(ans, -1, sizeof(ans));
	if (ok('R') && ok('B')) {
		ans[M] = totw;
		for (int i = 1; i < M; ++i) {
			vector<pair<int, int>> edges;
			for (int x = 0; x < M; ++x)
				if (inset[x])
					for (int y = 0; y < M; ++y)
						if (!inset[y]) {
							inset[x] = false;
							inset[y] = true;
							if (ok('R')) edges.emplace_back(x, y);
							if (ok('B')) edges.emplace_back(y, x);
							inset[y] = false;
							inset[x] = true;
						}
			int src = M, sink = M + 1;
			for (int y = 0; y < M; ++y) {
				if (inset[y]) continue;
				inset[y] = true;
				if (ok('R')) edges.emplace_back(src, y);
				if (ok('B')) edges.emplace_back(y, sink);
				inset[y] = false;
			}
			fill(dist, dist + M + 2, INF);
			fill(pre, pre + M + 2, -1);
			dist[src] = 0;
			pre[src] = 0;
			const int BIG = M + 5;
			for (int t = 1; t < M + 2; ++t) {
				for (auto e : edges) {
					int a = e.first, b = e.second;
					int w = (inset[a] ? W[a] : -W[a]) * BIG + 1;
					if (pre[a] != -1 && dist[a] + w < dist[b]) {
						dist[b] = dist[a] + w;
						pre[b] = a;
					}
				}
			}
			if (pre[sink] == -1) break;
			totw += (dist[sink] < 0 ? (dist[sink] - BIG + 1) : dist[sink]) / BIG;
			ans[M - i] = totw;
			for (int x = sink; x != src; x = pre[x])
				if (x != sink) inset[x] = !inset[x];
		}
	}
	for (int i = 1; i <= M; ++i) printf("%d\n", ans[i]);
}


