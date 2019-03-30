/* [eulerpath.cpp] */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
set<int> E[MAXN];
bool bio[MAXN];
// Undirected
void euler(int x) {
	while (!E[x].empty()) {
		int y = *E[x].begin();
		E[x].erase(y);
		E[y].erase(x);
		euler(y);
	}
	printf("%d ", x);
}
// Directed
void euler(int x) {
	while (!E[x].empty()) {
		int y = E[x].back();
		E[x].pop_back();
		euler(y);
	}
	printf("%d ", x);
}
int main() {
}


