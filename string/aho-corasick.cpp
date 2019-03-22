const int MAXC = 26;

struct node {
	node *c[MAXC] = {}, *f = NULL, *m = NULL;
	int depth = 0, countat = 0;

	node() {}
	node(int d) : depth(d) {}
};

// add a string to the trie
void add(node *n, char *s) {
	int len = 0;
	for (; s[len]; len++) {
		int id = s[len]-'a';
		if (!n->c[id]) { n->c[id] = new node(len+1); }
		n = n->c[id];
	}
	n->countat++;
}

// turn trie into automata
void build(node *root) {
	deque<node *> q;
	node *n, *x, *c, *f;
	for (int i=0;i<MAXC;++i) {
		if ((c=root->c[i])) {
			c->f = root; q.push_back(c);
		} else {
			root->c[i] = root;
		}
	}
	while (!q.empty()) {
		n = q.front(); q.pop_front();
		for (int i=0;i<MAXC;++i) {
			if (!(c = n->c[i])) continue;
			for (x = n->f; !x->c[i]; x = x->f) {}
			f = c->f = x->c[i];
			c->m = f->countat ? f : f->m;
			q.push_back(c);
		}
	}
}

// check string for matches using automata
void match(node *n, char *s, vector<pair<int, int>> &matches) {
	for (int i=0;s[i];++i) {
		int c = s[i]-'a';
		while (!n->c[c]) n = n->f;
		n = n->c[c];
		// if this node has a match, add it
		for (int k=n->countat; k-->0;)
			matches.push_back(make_pair(i-n->depth+1, i+1));
		// find all shorter matches that also end here
		for (node *m = n->m; m; m = m->m)
			for (int k=m->countat; k-->0;)
				matches.push_back(make_pair(i-m->depth+1, i+1));
	}
}

void build_and_match(char *hay, int n, char **needle, vector<pair<int, int>> &ms) {
	node *root = new node();
	for (int i = 0; i < n; ++i) add(root, needle[i]);
	build(root);
	match(root, hay, ms);
}
