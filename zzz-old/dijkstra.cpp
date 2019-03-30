/* [dijkstra.cpp] */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
const ll INF = 1LL << 60;
vector<pair<int, int>> E[100005];
int pre[100005];
ll dist[100005];
int main() {
	int N, M;
	scanf("%d%d", &N, &M);
	while (M--) {
		int a, b, w;
		scanf("%d%d%d", &a, &b, &w);
		E[a].push_back({b, w});
		E[b].push_back({a, w});
	}
	int src = 1, dest = N;
	typedef pair<ll, int> Pair;
	priority_queue<Pair, vector<Pair>, greater<Pair>> pq;
	fill(dist + 1, dist + 1 + N, INF);
	dist[src] = 0;
	pq.emplace(dist[src], src);
	while (!pq.empty()) {
		int u = pq.top().second;
		ll cdist = pq.top().first;
		pq.pop();
		if (cdist > dist[u]) continue;
		for (auto e : E[u]) {
			int v = e.first;
			ll val = dist[u] + e.second;
			if (val < dist[v]) {
				dist[v] = val;
				pq.emplace(val, v);
				// pre[v] = u;
			}
		}
	}
	if (dist[dest] == INF) printf("-1\n");
	else {
		vector<int> ans;
		for (int v = dest; v != src; v = pre[v]) {
			ans.push_back(v);
		}
		ans.push_back(1);
		reverse(ans.begin(), ans.end());
		for (int a : ans) printf("%d ", a);
	}
}


