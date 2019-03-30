/* [dinic.cpp] */
// Runtime: O(mn^2)
// Find n blocking flows. For each vertex maintain an index of ok-edge.
// If we are lucky and don't hit a fail vertex: O(n) for each flow.
// Each flow saturates an edge, so O(mn)
// If we are unlucky and hit a fail vertex, total O(sumdeg + n^2 = mn)
// For unit capacity, blocking flow takes O(m) to find
// At most O(min(sqrt m, n^(1/3))) iterations of blocking flow, by looking at level graph
// when distance becomes >= sqrt m (or >= 2n^(2/3))
// Impose f min-flow requirement on edge uv: add u->T, S->v cap f, t->s cap inf where s,t original src/sink
#include <bits/stdc++.h>
using namespace std;
const int MAXN = 5000;
int nodes = MAXN, src, sink;
int dist[MAXN], Q[MAXN], work[MAXN];
struct Edge {
	int to, rev;
	int f, cap;
};
vector<Edge> G[MAXN];
// Adds bidirectional edge
void addEdge(int s, int t, int cap) {
	Edge a = {t, (int)G[t].size(), 0, cap};
	Edge b = {s, (int)G[s].size(), 0, cap};
	G[s].push_back(a);
	G[t].push_back(b);
}
bool dinicBfs() {
	fill(dist, dist + nodes, -1);
	dist[src] = 0;
	int qt = 0;
	Q[qt++] = src;
	for (int qh = 0; qh < qt; qh++) {
		int u = Q[qh];
		for (int j = 0; j < G[u].size(); ++j) {
			Edge &e = G[u][j];
			int v = e.to;
			if (dist[v] < 0 && e.f < e.cap) {
				dist[v] = dist[u] + 1;
				Q[qt++] = v;
			}
		}
	}
	return dist[sink] >= 0;
}
int dinicDfs(int u, int f) {
	if (u == sink) return f;
	// Edges 0..work[u] - 1 have been saturated
	for (int &i = work[u]; i < G[u].size(); ++i) {
		Edge &e = G[u][i];
		if (e.cap <= e.f) continue;
		int v = e.to;
		if (dist[v] == dist[u] + 1) {
			int df = dinicDfs(v, min(f, e.cap - e.f));
			if (df > 0) {
				e.f += df;
				G[v][e.rev].f -= df;
				return df;
			}
		}
	}
	return 0;
}
int maxFlow() {
	int result = 0;
	while (dinicBfs()) {
		fill(work, work + nodes, 0);
		while (int delta = dinicDfs(src, INT_MAX))
			result += delta;
	}
	return result;
}
// To obtain cut
bool vis[MAXN];
void dfs(int x) {
	if (vis[x]) return;
	vis[x] = true;
	for (Edge &e : G[x]) {
		int v = e.to;
		if (e.f < e.cap) dfs(v);
	}
}
int main() {
	for (Edge &e : G[u]) {
		int v = e.to;
		if (e.f == e.cap && e.cap == 1) {
			// There is flow from u to v
		}
	}
	// To obtain the cut
	dfs(src);
	for (Edge &e : G[u]) {
		int v = e.to;
		if (vis[u] && !vis[v]) {
			// Cut edge (u,v)
		}
	}
	// Dilworth's theorem: max size of antichain = min chain-decomp
	// Get transitive closure of DAG, find min chains with bipartite graph
	// Antichain is vertices not in vertex cover
}

