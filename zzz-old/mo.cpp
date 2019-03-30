/* [mo.cpp] */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double Double;
int A[200005];
pair<pair<int, int>, int> queries[200005];
const int DIV = 450;
bool cmp(pair<pair<int, int>, int> a, pair<pair<int, int>, int> b) {
	int aa = a.first.first / DIV, bb = b.first.first / DIV;
	if (aa != bb) return aa < bb;
	return a.first.second < b.first.second;
}
ll power, ans[200005];
int freq[1000005];
void add(int p) {
	int v = A[p];
	power -= (ll)freq[v] * freq[v] * v;
	++freq[v];
	power += (ll)freq[v] * freq[v] * v;
}
void rem(int p) {
	int v = A[p];
	power -= (ll)freq[v] * freq[v] * v;
	--freq[v];
	power += (ll)freq[v] * freq[v] * v;
}
int main() {
	int N, Q;
	scanf("%d%d", &N, &Q);
	for (int i = 1; i <= N; ++i) scanf("%d", &A[i]);
	for (int q = 0; q < Q; ++q) {
		scanf("%d%d", &queries[q].first.first, &queries[q].first.second);
		queries[q].second = q;
	}
	sort(queries, queries + Q, cmp);
	int lef = 1, rig = 0;
	for (int q = 0; q < Q; ++q) {
		int l = queries[q].first.first, r = queries[q].first.second;
		while (rig < r) add(++rig);
		while (rig > r) rem(rig--);
		while (lef < l) rem(lef++);
		while (lef > l) add(--lef);
		ans[queries[q].second] = power;
	}
	for (int q = 0; q < Q; ++q) printf("%lld\n", ans[q]);
}


