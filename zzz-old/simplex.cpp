/* [simplex.cpp] */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
const ld EPS = 1e-11;
const int MAXM = 5, MAXN = 100005; // constraints, variables
struct Simplex {
	ld D[MAXM][MAXN] = {{}}; // D is B^-1 * nonbasic
	ld x[MAXN] = {};
	int B[MAXM], N[MAXN]; // basis, normal
	int m, n;
	bool feq(ld a, ld b) {
		return a - EPS < b && b < a + EPS;
	}
	void pivot(int r, int c) {
		ld inv = 1 / D[r][c];
		for (int j = 0; j <= n + 1; ++j) D[r][j] *= inv;
		for (int i = 0; i <= m; ++i)
			if (r != i)
				for (int j = 0; j <= n + 1; ++j)
					if (c != j) D[i][j] -= D[r][j] * D[i][c];
		for (int i = 0; i <= m; ++i)
			if (r != i) D[i][c] *= -inv;
			else D[i][c] = inv;
		swap(B[r], N[c]);
	}
	bool simplex(int phase) {
		for (;;) {
			// Find entering variable by Brand's rule
			int c = -1;
			for (int j = 0; j <= n; ++j) {
				if (phase == 2 && N[j] == m + n) continue;
				if (D[m][j] < -EPS && (c == -1 || N[j] < N[c]))
					c = j;
			}
			if (c == -1) return true; // Optimal
			// Find leaving variable by Brand's rule
			int r = -1;
			ld minRatio;
			for (int i = 0; i < m; ++i) {
				if (D[i][c] < EPS) continue;
				ld ratio = D[i][n + 1] / D[i][c];
				if (r == -1 || ratio < minRatio - EPS || (feq(ratio, minRatio) && B[i] < B[r])) {
					r = i;
					minRatio = ratio;
				}
			}
			if (r == -1) return false; // Unbounded
			pivot(r, c);
		}
	}
	// max c*x, s.t. Ax <= b, x >= 0
	bool init(ld A[MAXM][MAXN], ld b[MAXM], int _m, int _n) {
		m = _m;
		n = _n;
		// First compute a feasible solution from phase 1
		int smallB = 0;
		for (int i = 0; i < m; ++i) {
			for (int j = 0; j < n; ++j)
				D[i][j] = A[i][j];
			D[i][n] = -1;
			D[i][n + 1] = b[i];
			if (b[i] < b[smallB]) smallB = i;
			B[i] = n + i;
		}
		for (int j = 0; j < n; ++j) N[j] = j;
		D[m][n] = 1; // eps var
		N[n] = m + n; // eps var
		// Don't do phase 1 if b >= 0 already
		if (b[smallB] < -EPS) {
			pivot(smallB, n);
			simplex(1);
			if (D[m][n + 1] < -EPS) return false; // Infeasible
			// Force out eps var
			for (int i = 0; i < m; ++i)
				if (B[i] == m + n) {
					for (int j = 0; j <= n; ++j)
						if (!feq(D[i][j], 0)) {
							pivot(i, j);
							break;
						}
					break;
				}
		}
		return true;
	}
	ld solve(ld c[MAXN]) {
		for (int j = 0; j <= n + 1; ++j) {
			if (j <= n && N[j] < n) D[m][j] = -c[N[j]];
			else D[m][j] = 0;
			for (int i = 0; i < m; ++i)
				if (B[i] < n) D[m][j] += c[B[i]] * D[i][j];
		}
		if (!simplex(2)) return 1e100; // Unbounded
		for (int i = 0; i < m; ++i)
			if (B[i] < n) x[B[i]] = D[i][n + 1];
		return D[m][n + 1];
	}
};
ld A[MAXM][MAXN], B[MAXM], C[MAXN];
int main() {
	Simplex S;
	int n, p, q;
	scanf("%d%d%d", &n, &p, &q);
	for (int i = 0; i < n; ++i) {
		int a, b;
		scanf("%d%d", &a, &b);
		A[0][i] = -a;
		A[1][i] = -b;
		C[i] = -1;
	}
	B[0] = -p;
	B[1] = -q;
	S.init(A, B, 2, n);
	ld ans = -S.solve(C);
	printf("%.9lf\n", ans);
}


