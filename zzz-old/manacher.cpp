/* [manacher.cpp] */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
// @ a b a b b $
// 0121414123210
vector<int> manacher(string s) {
	string t = "@#";
	for (int i = 0; i < s.length(); ++i) {
		t += s[i];
		t += "#";
	}
	t += "$";
	int n = t.length();
	vector<int> len(n);
	int l = 1, r = 1;
	for (int i = 1; i < n - 1; ++i) {
		len[i] = min(r - i, len[l + (r - i)]);
		while (t[i - len[i]] == t[i + len[i]]) ++len[i];
		if (i + len[i] > r) {
			l = i - len[i];
			r = i + len[i];
		}
	}
	return len;
}
int main() {
	auto vec = manacher("ababb");
	for (int x : vec) printf("%d", x);
	printf("\n");
}


