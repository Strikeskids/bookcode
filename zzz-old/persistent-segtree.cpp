/* [persistent-segtree.cpp] */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double Double;
const int MAXS = 35 * 400005;
struct PersistentSegTree {
	int nid = 0;
	ll seg[MAXS];
	int L[MAXS], R[MAXS];
	void build(int id, int s, int e) {
		if (s == e) return;
		int m = (s + e) / 2;
		L[id] = ++nid;
		R[id] = ++nid;
		build(L[id], s, m);
		build(R[id], m + 1, e);
	}
	int update(int id, int s, int e, int p, int v) {
		int cid = ++nid;
		if (s == e) {
			seg[cid] = v;
			return cid;
		}
		int m = (s + e) / 2;
		L[cid] = L[id];
		R[cid] = R[id];
		if (p <= m) {
			L[cid] = update(L[id], s, m, p, v);
		} else {
			R[cid] = update(R[id], m + 1, e, p, v);
		}
		seg[cid] = seg[L[cid]] + seg[R[cid]];
		return cid;
	}
	ll query(int id, int s, int e, int qs, int qe) {
		if (qs <= s && e <= qe) {
			return seg[id];
		}
		int m = (s + e) / 2;
		ll ret = 0;
		if (qs <= m) ret += query(L[id], s, m, qs, qe);
		if (qe > m) ret += query(R[id], m + 1, e, qs, qe);
		return ret;
	}
} ptree;
int main() {

}


