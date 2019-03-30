/* [ukkonen.cpp] */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
const int MAXN = 100005;
const int INF = 1000000000;
char S[MAXN];
struct Node {
	map<int, Node*> to;
	int len, fchar;
	Node *link;
	Node(int fchar, int len): fchar(fchar), len(len) {
		st = new set<int>();
	}
};
Node *node, *root;
int pos, curn;
void init() {
	node = root = new Node(0, 0);
	pos = curn = 0;
}
void goDown() {
	for (;;) {
		Node *next = node->to[S[curn - pos]];
		if (!next || pos <= next->len) return;
		node = next;
		pos -= node->len;
	}
}
void addLetter() {
	int c = S[curn++];
	++pos;
	Node *last = nullptr;
	// While there are still suffixes to add
	while (pos) {
		goDown();
		int edge = S[curn - pos];
		Node *&v = node->to[edge];
		if (!v) {
			v = new Node(curn - pos, INF);
			if (last) last->link = node;
			last = nullptr; // Leaf has no suffix link
		} else {
			int t = S[v->fchar + pos - 1];
			if (t == c) {
				// We followed a letter on the edge
				if (last) last->link = node;
				return;
			} else {
				Node *u = new Node(v->fchar, pos - 1);
				u->to[c] = new Node(curn - 1, INF);
				u->to[t] = v;
				v->fchar += pos - 1;
				v->len -= pos - 1;
				v = u;
				if (last) last->link = u;
				last = u;
			}
		}
		if (node == root) --pos;
		else node = node->link;
	}
}
void finalize(Node *x, int len) {
	// x->len gives length of edge of parent, x->fchar gives position of first char
	// x->len > len means leaf
	// For counting unique substrings, add '$' sign!
	// Makes the leaves have finite length
	x->len = min(x->len, len - x->fchar);
	for (auto m : x->to) {
		finalize(m.second, len);
	}
}
void build(int len) {
	init();
	for (int i = 0; i < len; ++i) addLetter();
	finalize(root, len);
}
// For building suffix tree of 2 strings S and T
// Make string S$T#, then removeExtra to get rid of suffixes with both $ and #
void removeExtra(Node *x) {
	if (!x) return;
	// Check if edge contains "$"
	if (x->fchar < slen && x->fchar + x->len >= slen) {
		x->len = slen - x->fchar;
		x->to.clear();
		return;
	}
	x->len = min(x->len, slen + tlen - x->fchar);
	for (auto m : x->to) {
		removeExtra(m.second);
	}
}
// For obtaining a set of the suffixes in each node
void go(Node *u) {
	if (u->fchar + u->len == curn) {
		u->st->insert(len);
	}
	for (auto x : u->to) {
		Node *v = x.second;
		if (!v) continue;
		go(v, len);
		if (v->st->size() > u->st->size()) swap(v->st, u->st);
		u->st->insert(v->st->begin(), v->st->end());
	}
}
int main() {
	scanf("%s", S);
	build(strlen(S));
	cout << ans << "\n";
}

