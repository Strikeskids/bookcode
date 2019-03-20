#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

const int maxn = 1000;

#include "sa.cpp"

const char *CASES[] = {
	"ab",
	"aa",
	"aaa",
	"aaab",
	"aaaa",
	"aaaaa",
	"abacaba",
	"abacadabra",
	"cabaccbabcaacbbccbacbcbccbbaaababaabbcbcccabccacaccabccbaacaababcbcbbcbcbbababbccaacbabaacbccbcbaaca",
	"bcaabbbccbcbbaabbbccaaccabcaacbacbbcabbabbbcabccaaabbabbccacbbabacbaabbbbacbcbabcccbbccbacbabaacaaab",
	"bcacccbcbacbbccbbaababcccccaabbbbcaabaababcacaaaacccaacacaccccbacbaaacacacbbbbacbbbabbbabbccbabaccba",
	"bcbaabccabaaaccacccccbabbaccacaacabcbbaaacbbcabaaaacabcccaaabbcccabcbaaaacaacacbbccccacbaaababbaacab",
	"cbbbaacbcbaabbabacaaacbcacacbcccccbcbaacababababbbcabbaacaccabaabccbcabbaaaaaacbcbacbbbcbcbcbaaaabcb",
	"ccbcbbbbabbacbcccaaabbabcaaaabbaaabaccbcaabababcbacbbbbaabaacacabacbbcbaccccccbbabcbbcbcbaaccbcbcbcb",
	"bbcbbcbacccccaccbaccbcbabbbcccacaaccabcabcbacbbbacaabbbaaaaacabcbaaaaacacbacbbccabbacaacacabcabcbcbc",
	"abaacbcbbcaacbacabccccacbcbaabaaababcbcbcbbcbacaccaaaaaaccbaabacbacaccbccacaaababbababababbbcabcacab",
	"aabcacbabcbcbaaccaabbaaaaccbbcabaabacbbcbcbccacacbcacaaaccccbacaaacccabcbcaaaabbcbcaaabbbbbacccbccab",
	"abbccaabaccabbaacccabbaabaacbcabbaccbbaccacccaababababccccccaabbaaabacbbcbcbccaaaccbaabcacbbcbbacbcb",
	"abaaaabbbbacaaaabcaaaaccaaaaccbaccabcccbabcbbbcacbbaabbccbbaccbccccacbbcbbbbccbbcabacccaaacacabbacbc",
	"cbbbaabbbaabbbaaabccbcabccaccccabaabbccaaccbabbaaabbcaabbbccbaaaacccbbbbacbababbbbaabcbcbcababccccba",
	"bbccabbbcbbcbcbbabbbbbcccccaabbacacababbcabbccbcbcaaabaacccbaccacabbacbacaccbcbcbbccaccabbccbbabaaba",
	"bbacaaabaabacbbbabaabaaaaaabacabbccbabaaaaabbcacabaabbacaacaaaccacbbbaccaacbabbacaacbbbcabcabbaaacbc",
	"acbbbbabccabaabacaaaaabaabcbcbcbcbaaacabacaacaaacbabcabcccbccaaabbcccaabacabcaccabccbcaccacccaabbbca",
	"bababbbaaccccaabbaccbcbbcbbabcbbbbbbccbaabccabcbcbcaabbcaabbccccaabbcbacccabaaacabbccacbbccabccbacca",
	"cbcbaacabbcaaccbbabbbaababcabaacbcbcccacccccccbcbcabcaabbacbcaaaabacacbbcbacaccabbbabaaaaacbaaacccca",
	"caaccbaacabbbcabcaabaaabcabcaaacbbacacbabccbccccaccbaccabbbcbcbaccccbabbaabbcaabbaccccabccccaababbab",
	"bbcabbcbbbcbbbbabcccaabacbabbcbcaacbaabccbcccbaaabcccbbcbabcbbccbaaaccbcbabbbaaaaacbbbabcbccacbbaacb",
	"cbbacbbcabaccbbaaabbbbbbcbbabcccbcbbbccbbcaccbbbcaaabaacaabaacbbabbcbbacabaabaabcbcbbaaabcbbababbbca",
	"acacbbcccaababcbccbccabcacbaaccaabaababcacaabcabbcbabcaabbbacacbaacbbaacbccbacbabcbabacaaaaabacbcaac",
	"accccaabcaccbcbbcbcabacabbbbcccbccbbbaaaabbbaaacacaccbaccccbcaabbbbbacaccaabbcbacbbbacbbcccbcbbacbca",
	"cccbcbbcabcbbaaacbaabccaaabcbbaaacacccbaccccccacccacabbcbaacaaaaaaacacbcbaccbacabbabaaababaaaaabcabc",
};

int str[maxn];
int sa[maxn];
int lcp[maxn];

bool suffixlt(int n, int *r, int a, int b) {
	for (int i = 0; i + a < n && i + b < n; ++i) {
		if (r[a+i] < r[b+i]) return true;
		else if (r[b+i] < r[a+i]) return false;
	}
	return a > b;
}

void check_sa(int n, int *r, int *sa) {
	for (int i = 1; i < n; ++i) {
		assert(suffixlt(n, r, sa[i-1], sa[i]));
	}
}

void check_lcp(int n, int *r, int *sa, int *lcp) {
	for (int i = 1; i < n; ++i) {
		int a = sa[i-1], b = sa[i];
		int k = lcp[i-1];
		assert(a + k <= n && b + k <= n);
		assert(a + k == n || b + k == n || r[a+k] != r[b+k]);
		while (k-- > 0) assert(r[a+k]==r[b+k]);
	}
}

void print_sa(int n, const char *s, const int *sa) {
	for (int i = 0 ; i < n; ++i) {
		printf("%s\n", s+sa[i]);
	}
}

int main() {
	for (const char *c : CASES) {
		int n = 0;
		for (; c[n]; ++n) { str[n] = c[n] - 'a'; }
		str[n] = -1;
		compute_sa(n, 26, str, sa);
		check_sa(n, str, sa);
		compute_lcp(n, str, sa, lcp);
		check_lcp(n, str, sa, lcp);
	}
}