/* [suffix-array.cpp] */
// Suffix Array
#include <bits/stdc++.h>
using namespace std;
const int MAXP = 17;
char S[100005];
int N;
pair<pair<int, int>, int> T[100005];
int sufrank[MAXP + 1][100005];
int P[100005];
// rank[i] is the rank of i-th suffix
void getSuffixArray() {
	for (int i = 0; i < N; ++i) sufrank[0][i] = S[i];
	for (int l = 1; l <= MAXP; ++l) {
		int len = 1 << (l - 1);
		for (int i = 0; i < N; ++i) {
			int nex = i + len < N ? sufrank[l - 1][i + len] : -1;
			T[i] = {{sufrank[l - 1][i], nex}, i};
		}
		sort(T, T + N);
		for (int i = 0; i < N; ++i) {
			if (i && T[i].first == T[i - 1].first)
				sufrank[l][T[i].second] = sufrank[l][T[i - 1].second];
			else sufrank[l][T[i].second] = i;
		}
	}
}
int getLcp(int a, int b) {
	int len = 0;
	for (int l = MAXP; l >= 0; --l) {
		if (a + len >= N || b + len >= N) break;
		if (sufrank[l][a + len] == sufrank[l][b + len])
			len += 1 << l;
	}
	return len;
}
int main() {
	while (scanf("%s", S) == 1) {
		if (S[0] == '0') break;
		N = strlen(S);
		getSuffixArray(sufrank);
		// Gets suffixes sorted lexicographically in P
		for (int i = 0; i < N; ++i) {
			P[sufrank[MAXP][i]] = i;
		}
	}
}


