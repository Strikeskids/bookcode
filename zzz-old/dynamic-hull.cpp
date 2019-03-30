/* [dynamic-hull.cpp] */
/*
	Given x, find max ax + b.
	Check: query_x * max_a fits, max_b * max_a fits
*/
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
const ll IS_QUERY = -(1LL << 62);
struct Line {
	ll a, b;
	mutable function<const Line*()> succ;
	bool operator<(const Line &rhs) const {
		if (rhs.b != IS_QUERY) return a < rhs.a;
		const Line *s = succ();
		if (!s) return false;
		ll x = rhs.a;
		return b - s->b < (s->a - a) * x;
	}
};
struct DynamicHull : public multiset<Line> {
	bool bad(iterator y) {
		auto z = next(y);
		if (y == begin()) {
			if (z == end()) return false;
			return y->a == z->a && y->b <= z->b;
		}
		auto x = prev(y);
		if (z == end()) return y->a == x->a && y->b <= x->b;
		return (x->b - y->b) * (z->a - y->a) >= (y->b - z->b) * (y->a - x->a);
	}
	void insertLine(ll a, ll b) {
		auto y = insert({a, b});
		y->succ = [=] { return next(y) == end() ? nullptr : &*next(y); };
		if (bad(y)) {
			erase(y);
			return;
		}
		while (next(y) != end() && bad(next(y))) erase(next(y));
		while (y != begin() && bad(prev(y))) erase(prev(y));
	}
	ll query(ll x) {
		if (empty()) return 0;
		auto l = *lower_bound({x, IS_QUERY});
		return l.a * x + l.b;
	}
};
int main() {
}


