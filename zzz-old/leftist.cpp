/* [leftist.cpp] */
#include <bits/stdc++.h>
using namespace std;
const int MAXN = 600005;
struct Node {
	Node *l, *r;
	int id, x, rank;
	Node() : l(NULL), r(NULL), rank(1) {}
} *F[MAXN];
int facid[MAXN];
bool gone[MAXN];
int root(int a) {
	if (facid[a] == a) return a;
	return facid[a] = root(facid[a]);
}
int getRank(Node *x) {
	return x == NULL ? 0 : x->rank;
}
Node *merge(Node *a, Node *b) {
	if (a == NULL) return b;
	if (b == NULL) return a;
	if (a->x > b->x) swap(a, b);
	b->r = merge(a, b->r);
	if (getRank(b->l) < getRank(b->r)) swap(b->l, b->r);
	b->rank = getRank(b->r) + 1;
	return b;
}
int getAns(int a, int b) {
	if (a == -1) {
		if (gone[b]) return -1;
		b = root(b);
		Node *tree = F[b];
		if (tree->l == NULL && tree->r == NULL) return -1;
		gone[tree->id] = true;
		F[b] = merge(tree->l, tree->r);
		return F[b]->x;
	}
	if (gone[a] || gone[b]) return -1;
	a = root(a);
	b = root(b);
	if (a == b) return -1;
	Node *nroot = merge(F[a], F[b]);
	if (F[a] == nroot) facid[b] = a;
	else facid[a] = b;
	return root(a);
}
int main() {
	int N, M;
	scanf("%d%d", &N, &M);
	for (int i = 1; i <= N; ++i) {
		F[i] = new Node;
		scanf("%d", &F[i]->x);
		facid[i] = F[i]->id = i;
	}
	while (M--) {
		int a, b;
		scanf("%d%d", &a, &b);
		printf("%d\n", getAns(a, b));
	}
}


