/* [block-cut.cpp] */
// Block-decomposition: in a block, any 2 edges have a cycle going through them
// Blocks + cut vertices form a block-cut tree
// Note: Bridges are also blocks! Graph assumed connected!
#include <bits/stdc++.h>
using namespace std;
#define db(x) cerr << #x << "=" << x << endl
#define db2(x, y) cerr << #x << "=" << x << "," << #y << "=" << y << endl
#define db3(x, y, z) cerr << #x << "=" << x << "," << #y << "=" << y << "," << #z << "=" << z << endl
typedef long long ll;
typedef long double ld;
const int MAXN = 200005;
vector<int> E[MAXN]; // neighbor, edge id
int low[MAXN], H[MAXN], N;
vector<int> preorder;
bool cut[MAXN]; // cut[x] -> x is a cut vertex
int parent[MAXN];
int vblock[MAXN]; // root-edge for the block of the parent-edge
const int MAXM = 200005;
pair<int, int> edges[MAXM];
int eblock[MAXM]; // block of edge
vector<int> blocks[MAXN];
void go(int x) {
	preorder.push_back(x);
	low[x] = H[x];
	int totch = 0;
	for (auto y : E[x]) {
		if (H[y] == -1) {
			H[y] = H[x] + 1;
			parent[y] = x;
			++totch;
			go(y);
			low[x] = min(low[x], low[y]);
			if (low[y] >= H[x]) cut[x] = true;
		} else if (H[y] < H[x] - 1) {
			// This is a backedge
			low[x] = min(low[x], H[y]);
		}
	}
	if (x == 1 && totch > 1) cut[x] = true;
}
int main() {
	int M;
	scanf("%d%d", &N, &M);
	for (int i = 0; i < M; ++i) {
		int a, b;
		scanf("%d%d", &a, &b);
		E[a].push_back(b);
		E[b].push_back(a);
		edges[i] = {a, b};
	}
	memset(H, -1, sizeof(H));
	H[1] = 0;
	go(1);
	for (int x : preorder) {
		if (!parent[x]) continue;
		if (low[x] < H[parent[x]]) vblock[x] = vblock[parent[x]];
		else vblock[x] = x;
	}
	for (int i = 0; i < M; ++i) {
		auto e = edges[i];
		int deeper = H[e.first] > H[e.second] ? e.first : e.second;
		eblock[i] = vblock[deeper];
	}
	for (int i = 0; i < M; ++i) blocks[eblock[i]].push_back(i);
}


