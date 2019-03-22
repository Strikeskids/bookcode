struct node {
	int lo, hi, len; bool end;
	node *p; node *c[MAXC];

	node(int a, int b, int k, bool t) : lo(a), hi(b), len(k), end(t) {}
};

node* split(int *r, node *n, int off) {
	assert(0 < off && off < n->hi - n->lo);
	node *p = new node(n->lo, n->hi-off, false);
	p->c[r[n->lo += off]] = n;
	return n->p = (p->p = n->p)->c[r[p->lo]] = p;
}

int nst;
node *sttopo[maxn+1];

node *stk[maxn+1]; // temporary

// input: r is length n, sa is suffix array of r, lcp[i] = lcp(sa[i], sa[i+1])
// output: root of suffix tree, sttopo is topo-sort of suffix tree with length nst
node *compute_st(int n, int *r, int *sa, int *lcp) {
	int top=0;
	node *root = stk[top] = new node(-1, -1, 0, false);
	root->c[r[sa[0]]] = stk[++top] = new node(sa[0], n, n - sa[0], true);
	stk[top]->p = root;
	for (int i = 1; i < n; ++i) {
		int k = lcp[i-1], m = n - sa[i];
		assert(top >= 0 && 0 <= k && k <= m);
		while (top && k < stk[top-1]->len) {
			sttopo[nst++] = stk[top--];
		}
		if (k < stk[top]->len) {
			assert(top);
			node *n = sttopo[nst++] = stk[top];
			stk[top] = split(r, n, n->len - k);
		}
		if (m == stk[top]->len) {
			stk[top]->end = true;
		} else {
			int left = m - stk[top]->len;
			stk[++top] = new node(n - left, n, left, true);
			stk[top]->p = stk[top-1];
		}
	}
	while (top >= 0) sttopo[nst++] = stk[top--];
	return root;
}
