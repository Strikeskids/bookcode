/* [mpm-blocking-flow.cpp] */
// MPM algorithm for blocking flow.
// 1) Find vertex v with minimum f = min(inflow, outflow)
// 2) We can push f flow from v to sink and pull f flow from src to v
// 3) Every edge is saturated, except for 1 per each vertex
// 4) O(n^2)
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double Double;
int cap[1505][1505], incap[1505], outcap[1505], fw[1505], bk[1505];
int level[1505], flow[1505], ans[300005], id[1505][1505];
bool used[1505];
vector<int> LV[1505];
void reduceCap(int a, int b, int c) {
	cap[a][b] -= c;
	incap[b] -= c;
	outcap[a] -= c;
}
int main() {
	freopen("data.in", "r", stdin);
	freopen("data.out", "w", stdout);
	int N, M, L, src, sink;
	scanf("%d%d%d", &N, &M, &L);
	for (int i = 1; i <= N; ++i) {
		scanf("%d", &level[i]);
		if (level[i] == 1) src = i;
		else if (level[i] == L) sink = i;
		LV[level[i]].push_back(i);
	}
	used[src] = used[sink] = true;
	for (int i = 1; i <= M; ++i) {
		int a, b, c;
		scanf("%d%d%d", &a, &b, &c);
		cap[a][b] = c;
		incap[b] += c;
		outcap[a] += c;
		id[a][b] = i;
		if (a == src && b == sink) ans[i] = c;
	}
	for (int i = 1; i <= N; ++i) fw[i] = bk[i] = 1;
	for (int i = 0; i < N - 2; ++i) {
		int totflow = INT_MAX, v;
		for (int j = 1; j <= N; ++j) {
			if (used[j]) continue;
			int mincap = min(incap[j], outcap[j]);
			if (mincap < totflow) {
				totflow = mincap;
				v = j;
			}
		}
		fill(flow + 1, flow + 1 + N, 0);
		// Push
		flow[v] = totflow;
		for (int l = level[v]; l < L; ++l) {
			for (int w : LV[l]) {
				int amt = flow[w];
				while (amt && fw[w] <= N) {
					int toadd = min(cap[w][fw[w]], amt);
					amt -= toadd;
					reduceCap(w, fw[w], toadd);
					ans[id[w][fw[w]]] += toadd;
					flow[fw[w]] += toadd;
					if (!cap[w][fw[w]]) ++fw[w];
				}
			}
		}
		// Pull
		for (int l = level[v]; l > 1; --l) {
			for (int w : LV[l]) {
				int amt = flow[w];
				while (amt && bk[w] <= N) {
					int toadd = min(cap[bk[w]][w], amt);
					amt -= toadd;
					reduceCap(bk[w], w, toadd);
					ans[id[bk[w]][w]] += toadd;
					flow[bk[w]] += toadd;
					if (!cap[bk[w]][w]) ++bk[w];
				}
			}
		}
		// Remove v and update neighbors
		used[v] = true;
		for (int i = 1; i <= N; ++i)
			reduceCap(v, i, cap[v][i]);
	}
	for (int i = 1; i <= M; ++i) printf("%d\n", ans[i]);
}


