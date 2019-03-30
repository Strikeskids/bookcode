/* [dp-cactus.cpp] */
#include <bits/stdc++.h>
#define db(x) cerr << #x << " = " << x << endl
#define db2(x, y) cerr << #x << " = " << x << ", " << #y << " = " << y << endl
#define db3(x, y, z) cerr << #x << " = " << x << ", " << #y << " = " << y << ", " << #z << " = " << z << endl
using namespace std;
const int MAXN = 100005;
vector<int> E[MAXN], ch[MAXN];
int H[MAXN], low[MAXN];
int cycle[MAXN]; // bottom vertex in cycle
int F[MAXN][3][3][2]; // vertex, edgecolor, cyclecolor, parity
const int INF = 1000000000;
void bad() {
	printf("-1\n");
	exit(0);
}
// Check if graph is edge-cactus + compute cycle[] array
void dfs(int x) {
	low[x] = H[x];
	int up = 0; // number of cycles parent edge is in
	for (int y : E[x]) {
		if (H[y] == -1) {
			H[y] = H[x] + 1;
			dfs(y);
			low[x] = min(low[x], low[y]);
			if (low[y] < H[x]) {
				// parent edge is in cycle
				cycle[x] = cycle[y];
				++up;
			}
			ch[x].push_back(y);
		} else if (H[y] < H[x] - 1) {
			// This is a backedge to ancestor of x
			low[x] = min(low[x], H[y]);
			cycle[x] = x;
			++up;
		}
	}
	if (up > 1) bad();
}
bool badmask(int msk, int nc) {
	int other = nc < 2 ? 1 - nc : 2;
	return msk & (1 << other);
}
void go(int x, bool isroot = false) {
	for (int y : ch[x]) go(y);
	for (int e = 0; e < 3; ++e)
		for (int c = 0; c < 3; ++c)
			for (int p = 0; p < 2; ++p) {
				int G[8], GG[8];
				fill(G, G + 8, INF);
				// Check for odd cycle condition
				if (cycle[x] != x || p == 1) {
					int m = isroot ? 0 : 1 << e;
					if (cycle[x] == x) {
						if (badmask(m, c)) m = -1;
						else m |= 1 << c;
					}
					if (m != -1) G[m] = 0;
				}
				for (int y : ch[x]) {
					fill(GG, GG + 8, INF);
					for (int msk = 0; msk < 8; ++msk) {
						// msk is mask of colors adjacent to x
						// Try every color for this edge
						for (int yc = 0; yc < 3; ++yc) {
							if (badmask(msk, yc)) continue;
							// If new cycle, try every color for the cycle edge
							// Otherwise, cycle edge retains color
							for (int cc = 0; cc < 3; ++cc) {
								if (cycle[y] == cycle[x] && cc != c) continue;
								int nmsk = msk | (1 << yc);
								int np = p ^ (yc & 1);
								int add = yc;
								// If new cycle, mask gets cycle color too
								if (cycle[y] && cycle[y] != cycle[x]) {
									if (badmask(nmsk, cc)) continue;
									nmsk |= 1 << cc;
									add += cc;
									np = (yc & 1) ^ (cc & 1);
								}
								GG[nmsk] = min(GG[nmsk], G[msk] + F[y][yc][cc][np] + add);
							}
						}
					}
					swap(G, GG);
				}
				F[x][e][c][p] = INF;
				for (int i = 0; i < 8; ++i)
					F[x][e][c][p] = min(F[x][e][c][p], G[i]);
			}
}
int main() {
	int N, M;
	scanf("%d%d", &N, &M);
	while (M--) {
		int a, b;
		scanf("%d%d", &a, &b);
		E[a].push_back(b);
		E[b].push_back(a);
	}
	memset(H, -1, sizeof(H));
	vector<int> roots;
	for (int i = 1; i <= N; ++i) {
		if (H[i] != -1) continue;
		H[i] = 0;
		dfs(i);
		roots.push_back(i);
	}
	int ans = 0;
	for (int r : roots) {
		go(r, true);
		ans += F[r][0][0][0];
		if (ans >= INF) ans = INF;
	}
	if (ans == INF) printf("-1\n");
	else printf("%d\n", ans);
}

