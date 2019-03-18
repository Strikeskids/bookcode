struct node {
	node *c[2];
	node *p;

	bool r() { return !p || (p->c[0] != this && p->c[1] != this); }
	bool d() { return p && p->c[1] == this; }

	size_t size; int sum; int inc; int value;
};

// update aggregates of n with child data; idempotent
void pull(node *n) {
	n->size = 1;
	n->sum = n->value;
	for (node *c : n->c) {
		if (!c) continue;
		n->sum += c->sum; n->size += c->size;
	}
	n->sum += n->inc * n->size;
}

// update operations of children and clear operation of n; idempotent
void push(node *n) {
	if (n->inc) {
		for (node *c : n->c) if (c) { c->inc += n->inc; pull(c); }
		n->value += n->inc;
		n->inc = 0;
	}
}

void rotate(node *n) {
	assert(!n->r());
	node *p = n->p, *g = p->p, *c;

	if (g) push(g); push(p); push(n);

	bool dn = n->d(), dp = p->d();
	c = n->c[!dn];
	assert(!c || c->p == n);

	n->p = g; if (!p->r()) g->c[dp] = n;
	if (c) c->p = p; p->c[dn] = c;
	p->p = n; n->c[!dn] = p;

	pull(p); pull(n);
} 

node *splay(node *n) {
	while (!n->r()) {
		node *p = n->p;
		if (!p->r())
			rotate(!p->p->r() && p->d() == n->d() ? p : n);
		rotate(n);
	}
	push(n);
	return n;
}

node *expose(node *n) {
	splay(n); n->c[1] = NULL; pull(n);
	node *k = n;
	for (; k->p; pull(k = k->p)) {
		splay(k->p)->c[1] = k;
	}
	return k;
}

node *spine(node *n, bool dir) {
	while (n->c[dir]) { n = n->c[dir]; push(n); }
	return splay(n);
}

node *root(node *n) {
	return spine(expose(n), 0);
}

void link(node *ch, node *par) {
	assert(ch == root(ch) && ch != root(par));
	root(ch)->p = par;
}

void cut(node *n) {
	expose(n); splay(n);
	assert(n->c[0]);
	n->c[0] = (n->c[0]->p = NULL); pull(n);
}

node *lca(node *a, node *b) {
	expose(b);
	node *n1 = expose(a), *n2 = expose(b);
	assert(n1 == n2); // same tree
	return n1;
}

int sumpath(node *a, node *b) {
	int value = 0; node *n1, *n2;
	expose(a);
	n1 = expose(b); if (n1->c[1]) value += n1->c[1]->sum; // include (lca, b]
	n2 = expose(a); if (n2->c[1]) value += n2->c[1]->sum; // include (lca, a]
	assert(n1 == n2);
	value += n2->value; // include [lca, lca]
	return value;
}
