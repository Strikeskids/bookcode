/* [real-rref.cpp] */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
const ld EPS = 1e-8;
ld M[205][205];
// N variables, with column N + 1 being the value
void rref(int N) {
	for (int r = 1, c = 1; c <= N && r <= N; ++c) {
		int rr = r;
		for (int i = r + 1; i <= N; ++i)
			if (abs(M[i][c]) > EPS) rr = i;
		if (abs(M[rr][c]) <= EPS) {
			continue;
		}
		for (int j = 1; j <= N + 1; ++j) swap(M[rr][j], M[r][j]);
		// Subtract from bottom
		ld mult = 1 / M[r][c];
		for (int j = c; j <= N + 1; ++j) M[r][j] *= mult;
		for (int i = 1; i <= N; ++i) {
			if (i == r) continue;
			// We subtract from everything to get rref form
			ld t = M[i][c];
			for (int j = 1; j <= N + 1; ++j) {
				M[i][j] -= t * M[r][j];
			}
		}
		++r;
	}
}
int main() {
	M[1][1] = 2;
	M[1][2] = 0;
	M[1][3] = 1;
	M[2][1] = 4;
	M[2][2] = 4;
	M[2][3] = 2;
	M[3][1] = 4;
	M[3][2] = 4;
	M[3][3] = 2;
	rref(3);
	for (int i = 1; i <= 3; ++i, printf("\n"))
		for (int j = 1; j <= 3; ++j)
			printf("%Lf ", M[i][j]);
}


