/* [eertree.cpp] */
#include <bits/stdc++.h>
using namespace std;
#define db(x) cerr << #x << "=" << x << endl
#define db2(x, y) cerr << #x << "=" << x << "," << #y << "=" << y << endl
#define db3(x, y, z) cerr << #x << "=" << x << "," << #y << "=" << y << "," << #z << "=" << z << endl
typedef long long ll;
typedef long double ld;
const int MAXN = 2005;
struct Node {
	int len;
	int seriesAns;
	Node *link, *slink; // slink is first link such that slink->diff != diff
	map<int, Node*> to;
	Node(int _len) : len(_len) {}
	int diff() { return len > 0 ? len - link->len : 0; } // len - len[link]
};
char S[MAXN];
int pos;
Node *last, *zero;
void init() {
	Node *neg1 = new Node(-1);
	zero = new Node(0);
	neg1->link = neg1->slink = neg1;
	zero->link = zero->slink = neg1;
	last = neg1;
	pos = 0;
}
Node *getLink(Node *v) {
	while (pos == v->len || S[pos - v->len - 1] != S[pos])
		v = v->link;
	return v;
}
void addLetter() {
	char c = S[pos];
	last = getLink(last);
	if (!last->to[c]) {
		Node *node = new Node(last->len + 2);
		node->link = getLink(last->link)->to[c];
		if (!node->link) node->link = zero;
		if (node->diff() == node->link->diff()) {
			node->slink = node->link->slink;
		} else {
			node->slink = node->link;
		}
		last->to[c] = node;
	}
	last = last->to[c];
	++pos;
}
const int INF = 1000000000;
int ans[MAXN];
int main() {
	int TC;
	scanf("%d", &TC);
	while (TC--) {
		init();
		scanf("%s", S);
		int n = strlen(S);
		// (uv)^k u -> slink -> uvu or u
		// seriesAns of x->link would have computed (uv)^2 slink .. (uv)^k u;
		// we just add extra min of (uv) slink to it
		// slinks take O(log n) iterations
		for (int i = 1; i <= n; ++i) {
			addLetter();
			ans[i] = INF;
			for (Node *x = last; x->len > 0; x = x->slink) {
				x->seriesAns = ans[i - x->slink->len - x->diff()];
				if (x->slink != x->link)
					x->seriesAns = min(x->seriesAns, x->link->seriesAns);
				ans[i] = min(ans[i], x->seriesAns + 1);
			}
		}
		printf("%d\n", ans[n]);
	}
}


