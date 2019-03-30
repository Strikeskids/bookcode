/* [seg-tree.cpp] */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
void update(int sn, int s, int e, int p, int v) {
	if (s == e) {
		seg[sn] = v;
		return;
	}
	int m = (s + e) / 2, lsn = sn * 2, rsn = sn * 2 + 1;
	if (p <= m) update(lsn, s, m, p, v);
	else update(rsn, m + 1, e, p, v);
	seg[sn] = __gcd(seg[lsn], seg[rsn]);
}
int query(int sn, int s, int e, int qs, int qe) {
	if (qs <= s && e <= qe) {
		return seg[sn];
	}
	int m = (s + e) / 2, lsn = sn * 2, rsn = sn * 2 + 1, g = 0;
	if (qs <= m) g = query(lsn, s, m, p, v);
	if (qe > m) g = __gcd(g, query(rsn, m + 1, e, p, v));
	return g;
}
// Returns p such that qs..p-1 fulfills pred(x)
int walk(int sn, int s, int e, int qs, int qe) {
	if (qs <= s && e <= qe) {
		if (pred(s..e)) return e + 1;
	}
	if (s == e) return s;
	int m = (s + e) / 2, lsn = sn * 2, rsn = sn * 2 + 1, p;
	if (qs <= m) {
		p = walk(lsn, s, m, qs, qe);
		if (p <= m) return p;
	}
	if (qe > m) {
		p = walk(rsn, m + 1, e, qs, qe);
	}
	return p;
}
int main() {
}


