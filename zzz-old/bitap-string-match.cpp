/* [bitap-string-match.cpp] */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
char S[100005], T[100005];
bitset<100005> X[26], matches;
int main() {
	int Q;
	scanf("%s%d", S, &Q);
	int n = strlen(S);
	for (int i = 0; i < n; ++i) X[S[i] - 'a'][i] = 1;
	while (Q--) {
		int q;
		scanf("%d", &q);
		if (q == 1) {
			int a;
			char b;
			scanf("%d %c", &a, &b);
			--a;
			X[S[a] - 'a'][a] = 0;
			X[b - 'a'][a] = 1;
			S[a] = b;
		} else {
			int a, b;
			scanf("%d%d%s", &a, &b, T);
			int m = strlen(T);
			matches.set();
			for (int i = 0; i < m; ++i) {
				matches &= X[T[i] - 'a'] >> i;
			}
			--a;
			matches >>= a;
			int ans = matches.count();
			matches >>= max(0, b - a - m + 1);
			ans -= matches.count();
			printf("%d\n", ans);
		}
	}
}


