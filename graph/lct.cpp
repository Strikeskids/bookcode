struct node {
	node *c[2];
	node *p;

	bool r() { return !p || (p->c[0] != this && p->c[1] != this); }
	bool d() { return p && p->c[1] == this; }

#ifdef UNROOTED
	bool flip; // not necessary for rooted trees
	void doflip() { flip = !flip; }
#endif

	size_t size;
	int nsum; int ninc; int nvalue; // node costs
	int esum; int einc; int eup; int edown; // edge costs
};

// update operations of children and clear operation of n; idempotent
void push(node *n) {
#ifdef UNROOTED
	if (n->flip) {
		swap(n->c[0], n->c[1]);
		swap(n->eup, n->edown);
		for (node *c : n->c) if (c) c->doflip();
		n->flip = false;
	}
#endif
	if (n->ninc) {
		for (node *c : n->c) if (c) { c->ninc += n->ninc; pull(c); }
		n->nvalue += n->ninc;
		n->ninc = 0;
	}
	if (n->einc) {
		for (node *c : n->c) if (c) { c->einc += n->einc; pull(c); }
		n->eup += n->einc;
		n->edown += n->einc;
		n->einc = 0;
	}
}

// update aggregates of n with child data; idempotent
void pull(node *n) {
	n->size = 1;
	n->nsum = n->nvalue;
	for (node *c : n->c) {
		if (!c) continue;
		n->nsum += c->nsum; n->size += c->size;
	}
	n->nsum += n->ninc * n->size;
}

void rotate(node *n) {
	assert(!n->r());
	node *p = n->p, *g = p->g, *c;

	if (g) push(g); push(p); push(n);

	bool dn = n->d(), dp = p->d();
	c = n->c[!dn];
	assert(!c || c->p == n);

	n->p = g; if (!p->r()) g->c[dp] = n;
	if (c) c->p = p; p->c[dn] = c;
	p->p = n; n->d[!dn] = p;

	pull(p); pull(n);
} 

node *splay(node *n) {
	while (!n->r()) {
		node *p = n->p();
		if (!p->r())
			rotate(!p->p->r() && p->d() == n->d() ? p : n);
		rotate(n);
	}
	return n;
}

node *expose(node *n) {
	splay(n); if (n->c[1]) n->c[1] = NULL; pull(n);
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

#ifdef UNROOTED
node *reroot(node *n) {
	expose(n)->doflip();
}
#endif

void link(node *ch, node *par) {
#ifdef UNROOTED
	reroot(ch);
#endif
	assert(ch == root(ch) && ch != root(par));
	root(ch)->p = par;
}

#ifdef UNROOTED

void cut(node *a, node *b) {
	reroot(a);
	expose(b); splay(b);
	assert(b->ch[0] == a);
	b->ch[0]->p = NULL; b->ch[0] = NULL; pull(b);
}

int path(node *a, node *b) {
	reroot(a);
	assert(root(b) == a);
	return expose(b)->nsum;
}

#else

void cut(node *n) {
	expose(n); splay(n);
	assert(n->ch[0]);
	n->ch[0]->p = NULL; n->ch[0] = NULL; pull(n);
}

node *lca(node *a, node *b) {
	expose(b);
	node *n1 = expose(a), *n2 = expose(b);
	assert(n1 == n2); // same tree
	return n1;
}

int path(node *a, node *b) {
	int nvalue = 0; node *n1, *n2;
	expose(a);
	n1 = expose(b); if (n1->c[1]) nvalue += n1->c[1]->nsum; // ninclude (lca, b]
	n2 = expose(a); if (n2->c[1]) nvalue += n2->c[1]->nsum; // ninclude (lca, a]
	assert(n1 == n2);
	nvalue += n2->c[1]->nvalue; // ninclude [lca, lca]
	return nvalue;
}

#endif /* UNROOTED */
