/* [articulation.cpp] */
#include <bits/stdc++.h>
using namespace std;
vector<int> E[100005];
int low[100005], H[100005], N;
void go(int x) {
	low[x] = H[x];
	int totch = 0;
	for (auto y : E[x]) {
		if (H[y] == -1) {
			H[y] = H[x] + 1;
			++totch;
			go(y);
			low[x] = min(low[x], low[y]);
			if (low[y] >= H[x]) {
				// x is a cut vertex
				// This sub becomes disconnected
				cut = true;
			}
			siz[x] += siz[y];
		} else if (H[y] < H[x] - 1) {
			// This is a backedge
			low[x] = min(low[x], H[y]);
		}
	}
	if (cut || (x == 1 && totch > 1)) {
		// x is cut vertex
	}
}
int main() {
	memset(H, -1, sizeof(H));
	H[1] = 0;
	int M;
	scanf("%d%d", &N, &M);
	while (M--) {
		int a, b;
		scanf("%d%d", &a, &b);
		E[a].push_back(b);
		E[b].push_back(a);
	}
	go(1);
}


