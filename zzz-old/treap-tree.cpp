/* [treap-tree.cpp] */
// Treap
// Finds last vertex with distance K from root in dfs sequence
// Finds distance between u and v
// Disconnect a vertex from parent and connect to h-th ancestor (h >= 2)
#include <bits/stdc++.h>
using namespace std;
const int MAXN = 100005;
const int INF = 1000000000;
struct Node {
	Node *l, *r, *p;
	int v, val, siz, prior, sumval, minsum, maxsum;
	Node() {
		siz = sumval = 0;
		minsum = INF;
		maxsum = -INF;
	}
	inline int preval() {
		return val + l->sumval;
	}
	inline int key() {
		return 1 + l->siz;
	}
	int index(Node *);
	int fullval();
};
Node *nullNode = new Node, first[MAXN], last[MAXN];
Node *root = nullNode;
vector<int> child[MAXN];
int parent[MAXN];
int Node::index(Node *r = root) {
	int ret = key();
	for (Node *x = this; x != r; x = x->p)
		if (x->p->l != x) ret += x->p->key();
	return ret;
}
// Find exact position of the node (sum up deltas to left)
int Node::fullval() {
	int ret = preval();
	for (Node *x = this; x != root; x = x->p)
		if (x->p->l != x) ret += x->p->preval();
	return ret;
}
void update(Node *x) {
	x->siz = x->l->siz + x->r->siz + 1;
	x->sumval = x->l->sumval + x->r->sumval + x->val;
	int xsum = x->preval();
	x->minsum = min(xsum, min(x->l->minsum, x->r->minsum + xsum));
	x->maxsum = max(xsum, max(x->l->maxsum, x->r->maxsum + xsum));
	x->l->p = x->r->p = x;
}
void split(Node *x, int p, Node *&retl, Node *&retr) {
	// retl -> < p; retr -> >= p
	if (x == nullNode) {
		retl = retr = nullNode;
		return;
	}
	if (p <= x->key()) {
		retr = x;
		split(x->l, p, retl, retr->l);
	} else {
		retl = x;
		split(x->r, p - x->key(), retl->r, retr);
	}
	update(x);
}
void merge(Node *&x, Node *l, Node *r) {
	if (l == nullNode || r == nullNode) {
		x = l == nullNode ? r : l;
		return;
	}
	if (l->prior > r->prior) {
		x = l;
		merge(x->r, l->r, r);
	} else {
		x = r;
		merge(x->l, l, r->l);
	}
	update(x);
}
void insert(Node *x, int v, int val) {
	x->v = v;
	x->val = val;
	x->prior = (rand() << 15) | rand();
	x->l = x->r = nullNode;
	update(x);
	merge(root, root, x);
}
int getDistK(Node *p, int k) {
	for (;;) {
		k -= p->preval();
		if (p->r->minsum <= k && k <= p->r->maxsum) p = p->r;
		else if (!k) break;
		else {
			k += p->preval();
			p = p->l;
		}
	}
	if (p->val == -1) return parent[p->v];
	return p->v;
}
int getMin(Node *p, int l, int r) {
	if (l <= 1 && p->siz <= r) return p->minsum;
	int m = p->key(), ret = INF;
	if (l <= m && m <= r) ret = min(ret, p->preval());
	if (l < m) ret = min(ret, getMin(p->l, l, r));
	if (r > m) ret = min(ret, p->preval() + getMin(p->r, l - m, r - m));
	return ret;
}
void dfs(int x) {
	insert(&first[x], x, 1);
	for (auto i = child[x].begin(); i != child[x].end(); ++i)
		dfs(*i);
	insert(&last[x], x, -1);
}
int main() {
	srand(1337);
	int N, M;
	scanf("%d%d", &N, &M);
	for (int i = 1; i <= N; ++i) {
		int l, a;
		scanf("%d", &l);
		for (int j = 0; j < l; ++j) {
			scanf("%d", &a);
			child[i].push_back(a);
			parent[a] = i;
		}
	}
	dfs(1);
	while (M--) {
		int a, b, c;
		scanf("%d%d", &a, &b);
		if (a == 3) {
			printf("%d\n", getDistK(root, b + 1));
		} else {
			scanf("%d", &c);
			if (a == 1) {
				int x = first[b].index(), y = first[c].index();
				if (x > y) swap(x, y);
				printf("%d\n", first[b].fullval() + first[c].fullval() - 2 * getMin(root, x, y));
			} else {
				Node *beforeCut, *cut, *afterCut, *afterIns;
				int cutStart = first[b].index(), cutEnd = last[b].index();
				int ancestorDepth = first[b].fullval() - c;
				split(root, cutStart, beforeCut, cut);
				split(cut, cutEnd - cutStart + 2, cut, afterCut);
				int ancestor = getDistK(beforeCut, ancestorDepth);
				parent[b] = ancestor;
				int insertPos = last[ancestor].index(afterCut);
				split(afterCut, insertPos, afterCut, afterIns);
				merge(root, beforeCut, afterCut);
				merge(root, root, cut);
				merge(root, root, afterIns);
			}
		}
	}
}


