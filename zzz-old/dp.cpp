/* [dp.cpp] */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
// Divide-and-conquer optimization
void calc(int k, int l, int r, int start, int stop) {
	if (l > r) return;
	int m = (l + r) / 2;
	int best = -1, besti;
	for (int i = start; i <= min(stop, m); ++i) {
		int cnt = 12345;
		if (F[k - 1][i - 1] + cnt > best) {
			best = F[k - 1][i - 1] + cnt;
			besti = i;
		}
	}
	F[k][m] = best;
	calc(k, l, m - 1, start, besti);
	calc(k, m + 1, r, besti, stop);
}

// Knuth optimization
// Suppose best[i][j-1] <= best[i][j] <= best[i+1][j]
// Process in order of j-i
for (int diag = -N; diag <= N; ++diag)
	for (int j = 0; j <= N; ++j) {
		int i = j - diag;
		if (i < 0 || i > N) continue;
		for (int k = bestk[i][j - 1]; k <= bestk[i + 1][j]; ++k) {
			int cnt = 12345;
			if (cnt > best) {
				best = cnt;
				bestk[i][j] = k;
			}
		}
	}
int main() {
	for (int k = 2; k <= K; ++k) {
		calc(k, 1, N, 1, N);
	}

	// sum-over-subsets DP
	for (int k = 0; k < (1 << N); ++k)
			sos[k] = A[k];
		for (int j = 0; j < N; ++j)
			for (int k = (1 << N) - 1; k >= 0; --k)
				if (k & (1 << j)) sos[k] += sos[k ^ (1 << j)];
}


