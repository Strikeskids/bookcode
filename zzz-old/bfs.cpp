/* [bfs.cpp] */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double Double;
const int DR[4] = {0, 1, 0, -1};
const int DC[4] = {1, 0, -1, 0};
char G[55][55];
int sp[55][55];
int main() {
	for (int i = 0; i < N; ++i)
		scanf("%s", G[i]);
	queue<pair<int, int>> Q;
	Q.push({0, 0});
	memset(sp, -1, sizeof(sp));
	sp[0][0] = 0;
	while (!Q.empty()) {
		auto x = Q.front();
		Q.pop();
		for (int d = 0; d < 4; ++d) {
			int nr = x.first + DR[d];
			int nc = x.second + DC[d];
			if (nr < 0 || nc < 0 || nr >= N || nc >= M) continue;
			if (sp[nr][nc] != -1) continue;
			sp[nr][nc] = sp[x.first][x.second] + 1;
			Q.push({nr, nc});
		}
	}
}


