#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <set>

#define dbp(...) fprintf(stderr, __VA_ARGS__)
#define dbgs() dbp("%s:%d: ", __func__, __LINE__)
#define dbg(fmt, ...) dbp("%s:%d: " fmt "\n", __func__, __LINE__, __VA_ARGS__)

using namespace std;

#include "lct.cpp"

const int MAXN = 100;
const int NOPS = 50000;

struct data {
	int id;
	data *parent = NULL;
	int value;
	int depth;
	data *component;
	set<data *> children;
	node n;
};

data d[MAXN];

void setcomp(data *a, data *par) {
	a->component = par ? par->component : a;
	a->depth = par ? par->depth + 1 : 0;
	for (data *c : a->children) setcomp(c, a);
}

bool olink(data *a, data *b) {
	if (a->parent) return false;
	if (a->component == b->component) return false;
	dbg("LNK %d %d", a->id, b->id);
	b->children.insert(a);
	a->parent = b;
	setcomp(a, b);
	return true;
}

bool ocut(data *a) {
	if (!a->parent) return false;
	dbg("CUT %d %d", a->id, a->parent->id);
	a->parent->children.erase(a);
	a->parent = NULL;
	setcomp(a, NULL);
	return true;
}

int osum(data *a, data *b) {
	if (a->component != b->component) return 0;
	dbg("SUM %d %d", a->id, b->id);
	int res = 0;
	while (a != b) {
		if (a->depth >= b->depth) {
			res += a->value;
			a = a->parent;
		} else {
			res += b->value;
			b = b->parent;
		}
	}
	res += a->value;
	dbg("RESULT %d", res);
	return res;
}

data *rdata() {
	return &d[rand() % MAXN];
}

void incpath(node *a, node *b, int by) {
	node *n1, *n2;
	expose(a);
	n1 = expose(b); if (n1->c[1]) n1->c[1]->inc += by; // include (lca, b]
	n2 = expose(a); if (n2->c[1]) n2->c[1]->inc += by; // include (lca, a]
	assert(n1 == n2);
	n2->value += by; // include [lca, lca]
}

bool oinc(data *a, data *b, int by) {
	if (a->component != b->component) return false;
	dbg("INC %d %d (+%d)", a->id, b->id, by);
	while (a != b) {
		if (a->depth >= b->depth) {
			a->value += by;
			a = a->parent;
		} else {
			b->value += by;
			b = b->parent;
		}
	}
	a->value += by;
	return true;
}

int main() {
	srand(1337);
	for (int i = 0; i < MAXN; ++i) {
		setcomp(&d[i], NULL);
		d[i].id = i+1;
		d[i].value = d[i].n.value = i+1;
		pull(&d[i].n);
	}
	for (int nop = 0; nop < NOPS; ++nop) {
		data *a = rdata();
otherop:
		data *b = rdata();
		switch (rand() % 7) {
			case 1: {
				if (!ocut(a)) goto otherop;
				cut(&a->n);
				break;
			}
			case 6:
			case 2: {
				int s = osum(a, b);
				if (!s) goto otherop;
				assert(sumpath(&a->n, &b->n) == s);
				break;
			}
			case 3: {
				int amnt = 1 + rand() % 100;
				if (!oinc(a, b, amnt)) goto otherop;
				incpath(&a->n, &b->n, amnt);
				break;
			}
			case 4: {
				assert(root(&a->n) == &a->component->n);
				break;
			}
			case 5:
			case 0: {
				if (!olink(a, b)) goto otherop;
				link(&a->n, &b->n);
				break;
			}
		}
	}
	return 0;
}