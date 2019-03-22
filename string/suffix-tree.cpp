struct node {
	int lo, hi, dpt; // dpt is depth at end of this node
	bool term; // terminal
	node *p = {}; node *c[MAXC] = {};

	node() {}
	node(int a, int b, int k, bool t) : lo(a), hi(b), dpt(k), term(t) {}
};

node* split(int *r, node *n, int end) {
	int off = end - (n->dpt - (n->hi - n->lo));
	assert(0 < off && off < n->hi - n->lo);
	node *p = new node(n->lo, n->lo + off, end, false);
	p->c[r[n->lo += off]] = n;
	return n->p = (p->p = n->p)->c[r[p->lo]] = p;
}

int nst;
node *sttopo[MAXN+1];

node *stk[MAXN+1]; // temporary

// input: r is dptgth n, sa is suffix array of r, lcp[i] = lcp(sa[i], sa[i+1])
// output: root of suffix tree, sttopo is topo-sort of suffix tree with dptgth nst
node *compute_st(int n, int *r, int *sa, int *lcp) {
	int top = 0; nst = 0; 
	node *root = stk[top] = new node(-1, -1, 0, false);
	root->c[r[sa[0]]] = stk[++top] = new node(sa[0], n, n - sa[0], true);
	stk[top]->p = root;
	for (int i = 1; i < n; ++i) {
		int k = lcp[i-1], m = n - sa[i];
		assert(top >= 0 && 0 <= k && k <= m);
		while (top && k <= stk[top-1]->dpt) {
			sttopo[nst++] = stk[top--];
		}
		if (k < stk[top]->dpt) {
			assert(top);
			node *n = sttopo[nst++] = stk[top];
			stk[top] = split(r, n, k);
		}
		assert(stk[top]->dpt <= m);
		if (m == stk[top]->dpt) {
			stk[top]->term = true;
		} else {
			int st = n - (m - stk[top]->dpt);
			top++;
			stk[top-1]->c[r[st]] = stk[top] = new node(st, n, m, true);
			stk[top]->p = stk[top-1];
		}
	}
	while (top >= 0) {
		sttopo[nst++] = stk[top--];
	}
	return root;
}
