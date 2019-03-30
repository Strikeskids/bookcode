/* [mcmf.cpp] */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
// F * E log V Dijkstra implementation
// Nodes numbered 0 to nodes - 1
// Bounds:
// c is max{cost} at the start
// -cN <= pi <= cN
// -2cN <= dist <= 2cN
// -2cN <= cost <= 2cN
const ll INFC = 1LL << 60;
const int MAXN = 805;
struct Edge {
	int to;
	int f, cap;
	ll cost;
};
int nodes, src, sink;
ll pi[MAXN], dist[MAXN];
int fromEdge[MAXN];
vector<Edge> edges;
vector<int> G[MAXN];
void addEdge(int from, int to, int cap, ll cost) {
	G[from].push_back(edges.size());
	edges.push_back({to, 0, cap, cost});
	G[to].push_back(edges.size());
	edges.push_back({from, 0, 0, -cost});
}
ll cheapestPath() {
	typedef pair<ll, int> Pair;
	priority_queue<Pair, vector<Pair>, greater<Pair>> pq;
	fill(dist, dist + nodes, INFC);
	dist[src] = 0;
	pq.emplace(dist[src], src);
	while (!pq.empty()) {
		int u = pq.top().second;
		ll cdist = pq.top().first;
		pq.pop();
		if (cdist > dist[u]) continue;
		for (int x : G[u]) {
			Edge e = edges[x];
			if (e.f == e.cap) continue;
			int v = e.to;
			ll val = dist[u] + pi[u] - pi[v] + e.cost;
			if (val < dist[v]) {
				dist[v] = val;
				fromEdge[v] = x;
				pq.emplace(dist[v], v);
			}
		}
	}
	if (dist[sink] >= INFC) return INFC;
	ll cost = dist[sink] + pi[sink];
	for (int k = 0; k < nodes; ++k)
		pi[k] = min(INFC, pi[k] + dist[k]);
	return cost;
}
pair<ll, int> maxFlow() {
	ll totcost = 0;
	int totflow = 0;
	for (;;) {
		ll cost = cheapestPath();
		//	Change INFC to 0 to find min-cost flow (not maxflow)
		if (cost >= INFC) break;
		totcost += cost;
		++totflow;
		for (int x = sink; x != src;) {
			int e = fromEdge[x];
			++edges[e].f;
			--edges[e ^ 1].f;
			x = edges[e ^ 1].to;
		}
	}
	return {totcost, totflow};
}
int main() {

}

// Extras

// n^2 dijkstra
bool used[MAXN];
ll cheapestPath() {
	fill(dist, dist + nodes, INFC);
	fill(used, used + nodes, false);
	dist[src] = 0;
	for (;;) {
		int u = -1;
		for (int i = 0; i < nodes; ++i) {
			if (used[i]) continue;
			if (u == -1 || dist[i] < dist[u]) u = i;
		}
		if (u == -1 || dist[u] >= INFC) break;
		used[u] = true;
		for (int x : G[u]) {
			Edge e = edges[x];
			if (e.f == e.cap) continue;
			int v = e.to;
			ll val = dist[u] + pi[u] - pi[v] + e.cost;
			if (val < dist[v]) {
				dist[v] = val;
				fromEdge[v] = x;
			}
		}
	}
	if (dist[sink] >= INFC) return INFC;
	ll cost = dist[sink] + pi[sink];
	for (int k = 0; k < nodes; ++k)
		pi[k] = min(INFC, pi[k] + dist[k]);
	return cost;
}
// Bellman-Ford
void initPi() {
	fill(dist, dist + nodes, INFC);
	dist[src] = 0;
	for (int i = 0; i < nodes - 1; ++i)
		for (int v = 0; v < nodes; ++v)
			for (int x : G[v]) {
				Edge e = edges[x];
				int w = e.to;
				if (e.cap) dist[w] = min(dist[w], dist[v] + e.cost);
			}
	for (int i = 0; i < nodes; ++i) pi[i] = dist[i];
}
// For initially acyclic graphs, this takes O(E)
ll getDist(int x) {
	if (pi[x] < INFC) return pi[x];
	if (x == src) return pi[x] = 0;
	for (int i : G[x])
		if (edges[i].cap) {
			int w = edges[i].to;
			pi[x] = min(pi[x], edges[i].cost + getDist(w));
		}
	return pi[x];
}
void initPi() {
	fill(pi, pi + nodes, INFC);
	for (int i = 0; i < nodes; ++i) getDist(i);
}

