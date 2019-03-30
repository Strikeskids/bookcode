/* [persistent-dynamic-segtree.cpp] */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
struct Node {
	Node *l, *r;
	ll num;
};
const int MAXV = 1000000000;
Node *V[100005], *M[100005];
ll getnum(Node *x) {
	return x ? x->num : 0;
}
Node *update(Node *x, int s, int e, int q, int c) {
	Node *z = new Node();
	if (x) *z = *x;
	if (s == e) {
		z->num += c;
		return z;
	}
	int m = (s + e) / 2;
	if (q <= m) z->l = update(z->l, s, m, q, c);
	else z->r = update(z->r, m + 1, e, q, c);
	z->num = getnum(z->l) + getnum(z->r);
	return z;
}
int query(Node *x, int s, int e, int qs, int qe) {
	if (!getnum(x)) return 0;
	if (qs <= s && e <= qe) return x->num;
	int m = (s + e) / 2;
	ll ret = 0;
	if (qs <= m) ret += query(x->l, s, m, qs, qe);
	if (qe > m) ret += query(x->r, m + 1, e, qs, qe);
	return ret;
}
map<string, int> id;
int getid(string s) {
	if (!id.count(s)) id[s] = id.size() + 1;
	return id[s];
}
int main() {
	int Q;
	scanf("%d", &Q);
	for (int q = 1; q <= Q; ++q) {
		char op[15], s[25];
		scanf("%s", op);
		V[q] = V[q - 1];
		M[q] = M[q - 1];
		if (op[0] == 's') {
			int x;
			scanf("%s%d", s, &x);
			int t = getid(s);
			int v = query(M[q], 1, Q, t, t);
			if (v) {
				V[q] = update(V[q], 1, MAXV, v, -1);
			}
			M[q] = update(M[q], 1, Q, t, -v + x);
			V[q] = update(V[q], 1, MAXV, x, 1);
		} else if (op[0] == 'r') {
			scanf("%s", s);
			int t = getid(s);
			int v = query(M[q], 1, Q, t, t);
			if (v) {
				M[q] = update(M[q], 1, Q, t, -v);
				V[q] = update(V[q], 1, MAXV, v, -1);
			}
		} else if (op[0] == 'q') {
			scanf("%s", s);
			int t = getid(s);
			int v = query(M[q], 1, Q, t, t);
			if (v) {
				int ans = query(V[q], 1, MAXV, 1, v - 1);
				printf("%d\n", ans);
			} else {
				printf("-1\n");
			}
			fflush(stdout);
		} else {
			int d;
			scanf("%d", &d);
			M[q] = M[q - d - 1];
			V[q] = V[q - d - 1];
		}
	}
}


