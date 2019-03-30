/* [dp-profile.cpp] */
#include <bits/stdc++.h>
using namespace std;
#define db(x) cerr << #x << "=" << x << endl
#define db2(x, y) cerr << #x << "=" << x << "," << #y << "=" << y << endl
#define db3(x, y, z) cerr << #x << "=" << x << "," << #y << "=" << y << "," << #z << "=" << z << endl
typedef long long ll;
typedef long double ld;
typedef unsigned long long ull;
map<pair<int, ull>, int> dp, ndp;
const int MOD = 1000000007;
void add(int &v, int x) {
	v += x;
	if (v >= MOD) v -= MOD;
}
int main() {
	int n, m;
	scanf("%d%d", &n, &m);
	dp[{0, (1ULL << ((1 << n) - 1))}] = 1;
	for (int i = 0; i < m; ++i)
		for (int j = 0; j < n; ++j) {
			ndp.clear();
			for (int t = 0; t < 2; ++t) {
				// Try putting a t at cell (i, j)
				for (auto x : dp) {
					int colors = x.first.first;
					ull masks = x.first.second;
					ull nmasks = 0;
					for (int mask = 0; mask < (1 << n); ++mask) {
						if (!(masks & (1ULL << mask))) continue;
						if (!(mask & (1 << j))) {
							// There is no covering in prev column
							if (bool(colors & (1 << j)) == t) continue;
							// We must put a horizontal domino
							nmasks |= (1ULL << (mask ^ (1 << j)));
						} else {
							// Don't put a vertical domino
							nmasks |= (1ULL << (mask ^ (1 << j)));
							// Put a vertical domino
							if (!j) continue;
							if (mask & (1 << (j - 1))) continue;
							if (bool(colors & (1 << (j - 1))) == t) continue;
							nmasks |= (1ULL << (mask ^ (1 << (j - 1))));
						}
					}
					int ncolors = (colors & ~(1 << j)) | (t << j);
					add(ndp[{ncolors, nmasks}], x.second);
				}
			}
			swap(ndp, dp);
		}
	int ans = 0;
	for (auto x : dp) {
		if (x.first.second & (1ULL << ((1 << n) - 1)))
			add(ans, x.second);
	}
	printf("%d\n", ans);
}


