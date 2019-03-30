/* [mcmf-adjmatrix.cpp] */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
const ll INF = 1LL << 60;
const int MAXN = 465;
// Assumes vertices are labelled 0...nodes - 1.
int src, sink, nodes;
ll cap[MAXN][MAXN], flow[MAXN][MAXN], C[MAXN][MAXN];
ll pi[MAXN], dist[MAXN], pre[MAXN], width[MAXN];
bool mark[MAXN], used[MAXN];
// F * V^2 Dijkstra implementation
// Bounds:
// c is max{C[i][j]} at the start
// -cN <= pi[v] <= cN
// -2cN <= dist[v] <= 2cN
// -2cN <= cost[i][j] <= 2cN
// Allows edges with different costs in both directions
void addEdge(int from, int to, ll c, ll cost) {
	cap[from][to] = c;
	C[from][to] = cost;
}
inline void relax(int v, int i, ll cost, ll amt, bool m) {
	if (!amt) return;
	ll val = dist[v] + cost + pi[v] - pi[i];
	if (val < dist[i]) {
		dist[i] = val;
		pre[i] = v;
		mark[i] = m;
		width[i] = min(amt, width[v]);
	}
}
pair<ll, ll> dijkstra() {
	fill(dist, dist + nodes, INF);
	fill(width, width + nodes, 0);
	fill(used, used + nodes, false);
	dist[src] = 0;
	width[src] = INF;
	for (;;) {
		int v = -1;
		for (int i = 0; i < nodes; ++i) {
			if (used[i]) continue;
			if (v == -1 || dist[i] < dist[v]) v = i;
		}
		if (v == -1 || dist[v] == INF) break;
		used[v] = true;
		for (int i = 0; i < nodes; ++i) {
			relax(v, i, C[v][i], cap[v][i] - flow[v][i], true);
			relax(v, i, -C[i][v], flow[i][v], false);
		}
	}
	ll cost = width[sink] * (dist[sink] + pi[sink]);
	for (int i = 0; i < nodes; ++i)
		pi[i] = min(INF, pi[i] + dist[i]);
	return {cost, width[sink]};
}
pair<ll, ll> maxFlow() {
	ll totcost = 0, totflow = 0;
	for (;;) {
		auto x = dijkstra();
		// Use this only when wanting to find min-cost flow (not maxflow)
		// if (x.second >= 0) break;
		ll amt = x.second;
		if (!amt) break;
		totcost += x.first;
		totflow += amt;
		for (int v = sink; v != src; v = pre[v]) {
			if (mark[v]) {
				flow[pre[v]][v] += amt;
			} else {
				flow[v][pre[v]] -= amt;
			}
		}
	}
	return {totcost, totflow};
}
int main() {

}


