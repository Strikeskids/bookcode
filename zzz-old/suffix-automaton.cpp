/* [suffix-automaton.cpp] */
// Max transitions is 3n. (3) is O(n) in total
// States are equivalence classes of occurence sets
// Suffixes are just link^k[last], k=0,1,...
#include <bits/stdc++.h>
using namespace std;
#define db(x) cerr << #x << "=" << x << endl
#define db2(x, y) cerr << #x << "=" << x << "," << #y << "=" << y << endl
#define db3(x, y, z) cerr << #x << "=" << x << "," << #y << "=" << y << "," << #z << "=" << z << endl
typedef long long ll;
typedef long double ld;
const int MAXN = 400005; // Suffix automaton has max vertices 2n
map<char, int> to[MAXN]; // Transitions
int link[MAXN]; // Suffix links
int len[MAXN]; // Lengths of largest strings in states
int last = 0; // State corresponding to the whole string
int sz = 1; // Current amount of states
void addLetter(char c) { // Add character to end
	int p = last; // State of string s
	last = sz++; // Create state for string sc
	len[last] = len[p] + 1;
	for (; !to[p].count(c); p = link[p])
		to[p][c] = last; // Jumps which add new suffixes
	if (to[p][c] == last) {
		// This is the first occurrence of c if we are here
		link[last] = 0;
		return;
	}
	int q = to[p][c]; // First vertex containing suffix that appeared
	if (len[q] == len[p] + 1) {
		link[last] = q;
		return;
	}
	// We split off qq from q here. qq has the smaller suffixes that appear
	int qq = sz++;
	to[qq] = to[q];
	link[qq] = link[q];
	len[qq] = len[p] + 1;
	link[last] = link[q] = qq;
	for (; to[p][c] == q; p = link[p]) // (3)
		to[p][c] = qq; // Redirect transitions where needed
}
char S[MAXN];
int main() {
	// Counts distinct substrings
	// Note: this is sum{len[v]-len[link[v]]}
	scanf("%s", S);
	int n = strlen(S);
	ll ans = 0;
	for (int i = 0; i < n; ++i) {
		addLetter(S[i]);
		ans += len[last] - len[link[last]];
		printf("%lld\n", ans);
	}
}


