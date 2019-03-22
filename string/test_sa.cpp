#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

const int maxn = 1000;
const int MAXC = 26;

#include "sa.cpp"
#include "suffix-tree.cpp"

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
int sainv[maxn];

bool suffixlt(int n, int *r, int a, int b) {
	for (int i = 0; i + a < n && i + b < n; ++i) {
		if (r[a+i] < r[b+i]) return true;
		else if (r[b+i] < r[a+i]) return false;
	}
	return a > b;
}

void check_sa(int n, int *r, int *sa) {
	for (int i = 0; i < n; ++i) {
		assert(0 <= sa[i] && sa[i] < n);
		sainv[sa[i]] = i;
	}
	for (int i = 0; i < n; ++i) {
		assert(0 <= sainv[i] && sainv[i] < n && sa[sainv[i]] == i);
	}
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

void check_st(int n, int *r, node *root) {
	assert(sttopo[nst-1] == root);
	int ends = 0;
	for (int i = nst-1, j = nst-1; i >= 0; --i) {
		node *n = &sttopo[i];
		ends += n->end;
		assert(j <= i);
		assert(0 <= n->len && n->len <= n);
		if (n != root)
			assert(0 <= n->lo && n->lo < n->hi && n->hi <= n && n->len > 0);
		for (int c = MAXC - 1; c >= 0; --c) {
			if (!n->c[c]) continue;
			assert(j && n->c[c] == &sttopo[--j]);
			assert(n->c[c]->len == n->len + n->hi - n->lo);
		}
	}

	assert(ends == n);

	for (int st = 0; st < n; ++st) {
		node *n = root; int i, j;
		for (i = st, j = 0; i < n; ++i) {
			assert(n->lo + j <= n->hi);
			if (n->lo + j == n->hi) {
				assert(n->c[r[i]]);
				n = n->c[r[i]];
				j = 0;
				assert(n->lo + j < n->hi);
			}
			assert(r[n->lo + j] == r[i]);
		}
		assert(n->lo + j == n->hi && n->end);
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
		node *root = compute_st(n, str, sa, lcp);
		check_st(n, r, root);
	}
}