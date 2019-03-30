/* [dp-convex-hull.cpp] */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
/*
	- finds maximum of A*x+B
	- double check max coords for int/long long overflow
	- set min x query in put function
	- add lines with non-descending A coefficient
*/
const int MAXN = 100005;
struct FastHull {
	int A[MAXN];
	ll B[MAXN];
	ll P[MAXN]; // P is distinct at every point
	int c;
	FastHull(): c(0) {}
	ll query(int x) {
		int pos = upper_bound(P, P + c, x) - P - 1;
		return (ll)A[pos] * x + B[pos];
	}
	ll divideCeil(ll p, ll q) {
		if (p >= 0) return (p + q - 1) / q;
		return -((-p) / q);
	}
	void insertLine(int a, ll b) {
		// assert a is >= everything in A
		for (; c; --c) {
			if (A[c - 1] == a && B[c - 1] >= b) return;
			ll pt = P[c - 1];
			if (A[c - 1] * pt + B[c - 1] > a * pt + b) break;
		}
		P[c] = c ? divideCeil(B[c - 1] - b, a - A[c - 1]) : MIN_QUERY; // min x query
		if (P[c] > MAX_QUERY) return; // Don't insert this line if it will never be better
		A[c] = a;
		B[c] = b;
		++c;
	}
};
struct SlowHull {
	vector<pair<int, ll>> v;
	void insertLine(int a, ll b) {
		v.emplace_back(a, b);
	}
	ll query(ll x) {
		ll best = -1LL << 60;
		for (auto p : v)
			best = max(best, p.first * x + p.second);
		return best;
	}
};
int main() {
	FastHull hull1;
	SlowHull hull2;
	vector<int> as;
	for (int i = 0; i < 10000; ++i)
		as.push_back(rand() % int(1e8));
	sort(as.begin(), as.end());
	for (int ii = 0; ii < 10000; ++ii) {
		int b = rand() % int(1e8);
		hull1.insertLine(as[ii], b);
		hull2.insertLine(as[ii], b);
		int x = rand() % int(2e8 + 1) - int(1e8);
		assert(hull1.query(x) == hull2.query(x));
	}
}


