/* [set-of-intervals.cpp] */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double Double;
const int MOD = 1000000009;
const int INF = 1100000000;
map<int, int> shape; // Maintains starts of intervals and their values
// Gets interval [a, b] such that a <= pos <= b
int getval(int pos) {
	return prev(shape.upper_bound(pos))->second;
}
// Adds inc to pos, and merge intervals accordingly
void addval(int pos, int inc) {
	int old = getval(pos);
	shape[pos] = old + inc;
	if (!shape.count(pos + 1)) {
		shape[pos + 1] = old;
	}
	// Merge right interval with left interval if necessary.
	if (shape[pos + 1] == shape[pos]) shape.erase(pos + 1);
	if (shape[pos] == prev(shape.find(pos))->second) shape.erase(pos);
}
int main() {
	int TC;
	scanf("%d", &TC);
	while (TC--) {
		int N;
		scanf("%d", &N);
		shape.clear();
		shape[-INF] = 0;
		ll ans = 0;
		for (int i = 1; i <= N; ++i) {
			int p, e;
			int delta = 1;
			scanf("%d%d", &p, &e);
			int l = getval(p) != e ? p : prev(shape.upper_bound(p))->first - 1;
			addval(l, e);
			delta += p - l;
			int r = getval(p + 1) != -e ? p + 1 : shape.upper_bound(p + 1)->first;
			addval(r, -e);
			delta += r - p - 1;
			(ans += (ll)i * delta) %= MOD;
		}
		printf("%lld\n", ans);
	}
}


