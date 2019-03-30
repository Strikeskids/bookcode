/* [scc.cpp] */
#include <bits/stdc++.h>
using namespace std;
vector<int> E[105], ER[105];
int comp[105];
bool used[105];
stack<int> S;
void dfs(int x) {
	if (used[x]) return;
	used[x] = true;
	for (int y : E[x]) dfs(y);
	S.push(x);
}
void dfs2(int x, int c) {
	if (comp[x]) return;
	comp[x] = c;
	for (int y : ER[x]) dfs2(y, c);
}
int main() {
	for (int x = 1; x <= N; ++x) dfs(x);
	for (int x = 1; x <= N; ++x)
		for (int y : E[x]) ER[y].push_back(x);
	while (!S.empty()) {
		int x = S.top();
		S.pop();
		dfs2(x, x);
	}
}


