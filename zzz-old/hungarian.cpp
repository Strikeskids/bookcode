/* [hungarian.cpp] */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double Double;
const ll INF = 1LL << 60;
struct Hungarian {
	// Finds potentials >= 0 such that x_i + y_j = e_ij
	// Note labels are always >= 0 and < MAXC
	// Hence slack < 2 * MAXC - 1
	// Note unmatched LT[.] are always 0
	// Hence invariant min_S(LS) >= min_{unmatched T}(slack) holds
	// Runtime: O(n^3)
	vector<int> matchS, matchT, slackx;
	vector<bool> S, T;
	vector<vector<ll>> C;
	vector<ll> slack, LS, LT;
	int N;
	Hungarian(int N) : N(N), matchS(N + 1), matchT(N + 1),
		slackx(N + 1), S(N + 1), T(N + 1),
		C(N + 1, vector<ll>(N + 1)),
		slack(N + 1), LS(N + 1), LT(N + 1) {}
	ll solve() {
		for (int i = 1; i <= N; ++i)
			for (int j = 1; j <= N; ++j)
				LS[i] = max(LS[i], C[i][j]);
		for (int matches = 0; matches < N; ++matches) {
			int root;
			fill(slack.begin(), slack.end(), 0);
			fill(S.begin(), S.end(), false);
			fill(T.begin(), T.end(), false);
			for (int i = 1; i <= N; ++i)
				if (!matchS[i]) root = i;
			S[root] = true;
			for (int j = 1; j <= N; ++j) {
				slack[j] = LS[root] + LT[j] - C[root][j];
				slackx[j] = root;
			}
			for (;;) {
				// Find something with zero slack but not in T
				int y = 0;
				for (int i = 1; i <= N; ++i)
					if (!slack[i] && !T[i]) y = i;
				if (!y) {
					ll delta = INF;
					for (int i = 1; i <= N; ++i)
						if (slack[i]) delta = min(delta, slack[i]);
					for (int i = 1; i <= N; ++i) {
						if (T[i]) LT[i] += delta;
						if (S[i]) LS[i] -= delta;
						if (slack[i]) slack[i] -= delta;
						if (!slack[i] && !T[i]) y = i;
					}
				}
				if (!matchT[y]) {
					// Augment path from y..root
					for (int z = y;;) {
						int x = matchS[slackx[z]];
						matchT[z] = slackx[z];
						matchS[slackx[z]] = z;
						if (slackx[z] == root) break;
						z = x;
					}
					break;
				}
				int z = matchT[y];
				S[z] = T[y] = true;
				for (int i = 1; i <= N; ++i) {
					ll v = LS[z] + LT[i] - C[z][i];
					if (v < slack[i]) {
						slack[i] = v;
						slackx[i] = z;
					}
				}
			}
		}
		ll ans = 0;
		for (int i = 1; i <= N; ++i) ans += C[i][matchS[i]];
		return ans;
	}
};
int main() {

}


