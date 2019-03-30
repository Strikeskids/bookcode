#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
int main() {
	// Monotonic deque, query maximum, so front is big, back is small
	deque<pair<ll, int>> deq;
	for (int i = 0; i < n; ++i) {
		int lef, rig = i;
		if (!deq.empty() && deq.front().second == lef) deq.pop_front();
		ll val = 10;
		while (!deq.empty() && deq.back().first <= val) deq.pop_back();
		deq.push_back({val, i});
	}
}