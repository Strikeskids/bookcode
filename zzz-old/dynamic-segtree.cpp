/* [dynamic-segtree.cpp] */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
struct Node {
	Node *l, *r;
	ll num;
};
ll getnum(Node *x) {
	return x ? x->num : 0;
}
void update(Node *&x, int s, int e, int q, ll c) {
	if (!x) x = new Node();
	if (s == e) {
		x->num += c;
		return;
	}
	int m = (s + e) / 2;
	if (q <= m) update(x->l, s, m, q, c);
	else update(x->r, m + 1, e, q, c);
	x->num = getnum(x->l) + getnum(x->r);
}
// Finds first p in [s,e] for which getnum(s..p) >= c (or e+1 if not exist)
int walk(Node *x, int s, int e, ll c) {
	if (getnum(x) < c) return e + 1;
	if (s == e) return s;
	int m = (s + e) / 2;
	ll nn = getnum(x->l);
	if (nn >= c) {
		return walk(x->l, s, m, c);
	} else {
		return walk(x->r, m + 1, e, c - nn);
	}
}
// With lazy propagation
void update(Node *&x, ll s, ll e, ll qs, ll qe, ll c) {
	if (!x) x = new Node();
	if (qs <= s && e <= qe) {
		x->num += c;
		x->prop += c;
		return;
	}
	ll m = s < 0 ? (s + e - 1) / 2 : (s + e) / 2;
	if (x->prop) {
		update(x->l, s, m, s, m, x->prop);
		update(x->r, m + 1, e, m + 1, e, x->prop);
		x->prop = 0;
	}
	if (qs <= m) update(x->l, s, m, qs, qe, c);
	if (qe > m) update(x->r, m + 1, e, qs, qe, c);
	x->num = max(getnum(x->l), getnum(x->r));
}
int main() {
}


