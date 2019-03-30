/* [aho-corasick.cpp] */
// Aho Corasick
// Finds keywords in big string
#include <bits/stdc++.h>
using namespace std;
char S[100005], word[100005];
struct Node {
	int matchlen;
	Node *fail; // Longest border
	Node *nex[26];
} *root;
int main() {
	int N;
	scanf("%s%d", S, &N);
	root = new Node();
	for (int i = 0; i < N; ++i) {
		scanf("%s", word);
		int len = strlen(word);
		Node *p = root;
		for (int j = 0; j < len; ++j) {
			int c = word[j] - 'a';
			if (!p->nex[c]) {
				p->nex[c] = new Node();
			}
			p = p->nex[c];
		}
		p->matchlen = len;
	}
	queue<Node*> Q;
	for (int i = 0; i < 26; ++i)
		if (!root->nex[i]) {
			root->nex[i] = root;
		} else {
			root->nex[i]->fail = root;
			Q.push(root->nex[i]);
		}
	while (!Q.empty()) {
		Node *p = Q.front();
		Q.pop();
		for (int i = 0; i < 26; ++i) {
			Node *v = p->fail;
			while (!v->nex[i]) v = v->fail;
			v = v->nex[i];
			if (p->nex[i]) {
				p->nex[i]->fail = v;
				Q.push(p->nex[i]);
				// p->nex[i]->matches |= v->matches;
			} else {
				// Goes directly to failed vertex.
				p->nex[i] = v;
			}
		}
	}
	int M = strlen(S);
	Node *p = root;
	for (int i = 0; i < M; ++i) {
		int c = S[i] - 'a';
		p = p->nex[c];
		if (p->matchlen) {
			// Found a match!
			printf("%s\n", string(S).substr(i - p->matchlen + 1, p->matchlen).c_str());
		}
	}
}


