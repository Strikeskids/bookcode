#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

const int MAXN = 1000;
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

int str[MAXN];
int sa[MAXN];
int lcp[MAXN];
int sainv[MAXN];

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

int check_st_topo(int N, int j, node *n) {
	assert(sttopo[j] == n);
	for (int c = MAXC - 1; c >= 0; --c) {
		if (!n->c[c]) continue;
		j = check_st_topo(N, j - 1, n->c[c]);
		assert(n->c[c]->dpt == n->dpt + n->c[c]->hi - n->c[c]->lo);
	}
	return j;
}

void check_st(int N, int *r, node *root) {
	int leaves = 0;
	for (int i = nst-1; i >= 0; --i) {
		node *n = sttopo[i];
		leaves += n->term;
		assert(0 <= n->dpt && n->dpt <= N);
		if (n != root)
			assert(0 <= n->lo && n->lo < n->hi && n->hi <= N && n->dpt > 0);
	}
	assert(leaves == N);

	int last = check_st_topo(N, nst-1, root);
	assert(last == 0);

	for (int st = 0; st < N; ++st) {
		node *n = root; int i, j;
		for (i = st, j = 0; i < N; ++i, ++j) {
			assert(n->lo + j <= n->hi);
			if (n->lo + j == n->hi) {
				assert(n->c[r[i]]);
				n = n->c[r[i]];
				j = 0;
				assert(n->lo + j < n->hi);
			}
			assert(r[n->lo + j] == r[i]);
		}
		assert(n->lo + j == n->hi && n->term);
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
		check_st(n, str, root);
	}
}