/* [pbds.cpp] */
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
typedef __gnu_pbds::tree<pair<int, int>, __gnu_pbds::null_type,
	less<pair<int, int>>,
	__gnu_pbds::rb_tree_tag,
	__gnu_pbds::tree_order_statistics_node_update> oset;

int ans[200005];
long long H[200005], A[200005];
vector<pair<int, int>> E[200005];
oset *S[200005];

void go(int x) {
	S[x] = new oset();
	for (auto e : E[x]) {
		int y = e.first, w = e.second;
		H[y] = H[x] + w;
		go(y);
		ans[x] += S[y]->order_of_key({H[x], INT_MAX}); // count # strictly smaller
	}
	for (auto e : E[x]) {
		int y = e.first, w = e.second;
		if (S[y]->size() > S[x]->size()) swap(S[x], S[y]);
		// Insert S[y] into S[x]
		for (auto v : *S[y]) S[x]->insert(v);
	}
	S[x]->insert({H[x] - A[x], x});
}

int main() {
	int N;
	scanf("%d", &N);
	for (int i = 1; i <= N; ++i) scanf("%I64d", &A[i]);
	for (int i = 2; i <= N; ++i) {
		int p, w;
		scanf("%d%d", &p, &w);
		E[p].push_back({i, w});
	}
	go(1);
	for (int i = 1; i <= N; ++i) printf("%d ", ans[i]);
	printf("\n");
}
/*
ordered_set X;
X.insert(1);
X.insert(2);
X.insert(4);
X.insert(8);
X.insert(16);

cout<<*X.find_by_order(1)<<endl; // 2
cout<<*X.find_by_order(2)<<endl; // 4
cout<<*X.find_by_order(4)<<endl; // 16
cout<<(end(X)==X.find_by_order(6))<<endl; // true

cout<<X.order_of_key(-5)<<endl;  // 0
cout<<X.order_of_key(1)<<endl;   // 0
cout<<X.order_of_key(3)<<endl;   // 2
cout<<X.order_of_key(4)<<endl;   // 2
cout<<X.order_of_key(400)<<endl; // 5
*/

