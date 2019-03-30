/* [treap.cpp] */
#include <bits/stdc++.h>
using namespace std;
// Manipulation of strings. Able to split a string and concat 2 strings.
// Also able to get the root of the treap that contains a subtreap.
struct Node {
	Node *l, *r, *par;
	int name, siz, prior;
	Node(int name) : name(name) {
		prior = ((rand() & 0x7fff) << 15) | (rand() & 0x7fff);
		siz = 1;
		l = r = par = nullptr;
	}
};
const int MAXN = 10000005;
Node *nodes[MAXN];
int getsiz(Node *x) { return x ? x->siz : 0; }
void update(Node *x) {
	x->siz = getsiz(x->l) + getsiz(x->r) + 1;
	if (x->l) x->l->par = x;
	if (x->r) x->r->par = x;
}
// Splits the tree rooted at x into (<p, >=p)
// Note that this modifies x!
pair<Node*, Node*> split(Node *x, int p) {
	if (!x) return {nullptr, nullptr};
	pair<Node*, Node*> ret;
	Node *rest;
	int key = getsiz(x->l) + 1;
	if (key >= p) {
		tie(rest, x->l) = split(x->l, p);
		ret = {rest, x};
	} else {
		tie(x->r, rest) = split(x->r, p - key);
		ret = {x, rest};
	}
	if (rest) rest->par = nullptr;
	update(x);
	return ret;
}
// Assumes all of l are smaller than all of r.
// Note that this modifies l and r!
Node *merge(Node *l, Node *r) {
	if (!l) return r;
	if (!r) return l;
	Node *ret;
	if (l->prior > r->prior) {
		l->r = merge(l->r, r);
		ret = l;
	} else {
		r->l = merge(l, r->l);
		ret = r;
	}
	update(ret);
	return ret;
}
void print(Node *x) {
	if (!x) return;
	print(x->l);
	printf("%d ", x->name);
	print(x->r);
}
Node *getroot(Node *x) {
	while (x->par) x = x->par;
	return x;
}
int getindex(Node *x) {
	int ret = getsiz(x->l) + 1;
	for (; x->par; x = x->par) {
		if (x == x->par->r) ret += getsiz(x->par->l) + 1;
	}
	return ret;
}
int main() {

}


