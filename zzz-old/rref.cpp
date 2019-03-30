/* [rref.cpp] */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double Double;
int M[205][205];
const int RELMOD = 99991;
const int MOD = 1000000007;
int modex(int a, int p) {
	if (!p) return 1;
	if (p & 1) return ((ll)a * modex(a, p - 1)) % RELMOD;
	int v = modex(a, p >> 1);
	return ((ll)v * v) % RELMOD;
}
int main() {
	int N;
	scanf("%d", &N);
	for (int i = 1; i <= N; ++i) {
		int k;
		scanf("%d", &k);
		M[i][i] = 1;
		while (k--) {
			int a;
			scanf("%d", &a);
			M[i][a] = -1;
		}
	}
	ll ans = 1;
	// rref
	// N variables, with column N + 1 being the value
	for (int r = 1, c = 1; c <= N && r <= N; ++c) {
		int rr = r;
		for (int i = r + 1; i <= N; ++i)
			if (M[i][c]) rr = i;
		if (!M[rr][c]) {
			ans = (ans * RELMOD) % MOD;
			continue;
		}
		for (int j = 1; j <= N + 1; ++j) swap(M[rr][j], M[r][j]);
		// Subtract from bottom
		ll mult = modex(M[r][c], RELMOD - 2);
		for (int j = c; j <= N + 1; ++j) M[r][j] = (mult * M[r][j]) % RELMOD;
		for (int i = 1; i <= N; ++i) {
			if (i == r) continue;
			// We subtract from everything to get rref form
			int t = M[i][c];
			for (int j = 1; j <= N + 1; ++j) {
				M[i][j] = (M[i][j] - t * M[r][j]) % RELMOD;
				if (M[i][j] < 0) M[i][j] += RELMOD;
			}
		}
		++r;
	}
	printf("%lld\n", ans);
}


