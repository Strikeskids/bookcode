/* [distinct-in-range.cpp] */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double Double;
const int MAXN = 35005;
const int MAXS = 30 * MAXN;
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
	int update(int id, int s, int e, int p) {
		int cid = ++nid;
		if (s == e) {
			++seg[cid];
			return cid;
		}
		int m = (s + e) / 2;
		L[cid] = L[id];
		R[cid] = R[id];
		if (p <= m) {
			L[cid] = update(L[id], s, m, p);
		} else {
			R[cid] = update(R[id], m + 1, e, p);
		}
		seg[cid] = seg[L[cid]] + seg[R[cid]];
		return cid;
	}
	ll query(int id, int s, int e, int qs, int qe) {
		if (qs <= s && e <= qe) {
			return seg[id];
		}
		int m = (s + e) / 2, ret = 0;
		if (qs <= m) ret += query(L[id], s, m, qs, qe);
		if (qe > m) ret += query(R[id], m + 1, e, qs, qe);
		return ret;
	}
} pseg;
struct RangeDistinct {
	int tree[MAXN], N;
	map<int, int> pre;
	vector<int> ends[MAXN];
	int count(int l, int r) {
		if (l > r) return 0;
		return pseg.query(tree[l], 1, N, l, r);
	}
	void init(int N, int *A) {
		this->N = N;
		for (int i = 1; i <= N; ++i) {
			ends[pre[A[i]] + 1].push_back(i);
			pre[A[i]] = i;
		}
		pseg.build(0, 1, N);
		for (int i = 1; i <= N; ++i) {
			tree[i] = tree[i - 1];
			for (int e : ends[i]) {
				tree[i] = pseg.update(tree[i], 1, N, e);
			}
		}
	}
} distinct;
int main() {
}


