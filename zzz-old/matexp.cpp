/* [matexp.cpp] */
#include <bits/stdc++.h>
using namespace std;
// Mod 2 version
const int MAXN = 128;
void mmult(vector<bitset<MAXN>> &A, vector<bitset<MAXN>> &B) {
	vector<bitset<MAXN>> tmp(N);
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j) {
			tmp[i][j] = 0;
			for (int k = 0; k < N; ++k)
				tmp[i][j] = tmp[i][j] ^ (A[i][k] & B[k][j]);
		}
	A = tmp;
}
void mexp(vector<bitset<MAXN>> &ret, vector<bitset<MAXN>> &M, ll p) {
	if (!p) {
		for (int i = 0; i < N; ++i)
			for (int j = 0; j < N; ++j)
				ret[i][j] = i == j;
		return;
	}
	if (p & 1) {
		mexp(ret, M, p - 1);
		mmult(ret, M);
		return;
	}
	mexp(ret, M, p >> 1);
	mmult(ret, ret);
}

// Normal version
const int MAXN = 128;
int M[MAXN][MAXN], n, res[MAXN][MAXN], tmp[MAXN][MAXN];
const int MOD = 1000000009;
void mmult(int A[][MAXN], int B[][MAXN]) {
	memset(tmp, 0, sizeof(tmp));
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			for (int k = 0; k < n; ++k)
				tmp[i][j] = (tmp[i][j] + (ll)A[i][k] * B[k][j]) % MOD;
	memcpy(A, tmp, sizeof(tmp));
}
void mexp(int ret[][MAXN], ll p) {
	if (!p) {
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < n; ++j)
				ret[i][j] = i == j;
		return;
	}
	if (p & 1) {
		mexp(ret, p - 1);
		mmult(ret, M);
		return;
	}
	mexp(ret, p >> 1);
	mmult(ret, ret);
}
int main() {
	// Modify n * n matrix M
	for (int i = 0; i < n; ++i) M[i][i] = 1;
	mexp(res, N);
	// M^N is stored in res
	printf("%d\n", res[0][0]);
}


