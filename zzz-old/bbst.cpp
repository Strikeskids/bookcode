/* [bbst.cpp] */
// BBST
#include <bits/stdc++.h>
using namespace std;
struct Node {
	Node *left, *right;
	int val, depth;
	Node(int a) : val(a), depth(1), left(NULL), right(NULL) {}
};
int height(Node *x) {
	return x == NULL ? 0 : x->depth;
}
void updateHeight(Node *x) {
	x->depth = max(height(x->left), height(x->right)) + 1;
}
Node *leftRotate(Node *x) {
	Node *a = x->right, *b = x->right->left;
	x->right = b;
	a->left = x;
	updateHeight(a);
	updateHeight(x);
	return a;
}
Node *rightRotate(Node *x) {
	Node *a = x->left, *b = x->left->right;
	x->left = b;
	a->right = x;
	updateHeight(a);
	updateHeight(x);
	return a;
}
void insert(Node *&x, int a) {
	if (x == NULL) {
		x = new Node(a);
		return;
	}
	if (a == x->val) return; // Already in tree
	if (a < x->val) insert(x->left, a);
	else insert(x->right, a);
	int lefth = height(x->left), righth = height(x->right);
	if (lefth - righth >= 2) {
		if (height(x->left->left) < height(x->left->right))
			x->left = leftRotate(x->left);
		x = rightRotate(x);
	} else if (righth - lefth >= 2) {
		if (height(x->right->left) > height(x->right->right))
			x->right = rightRotate(x->right);
		x = leftRotate(x);
	} else {
		updateHeight(x);
	}
}
void printTree(Node *x) {
	if (x == NULL) return;
	printTree(x->left);
	printf("%d ", x->val);
	printTree(x->right);
}
int A[1000005];
int main() {
	int N = 1000000;
	for (int i = 0; i < N; ++i) A[i] = i;
	srand(time(NULL));
	random_shuffle(A, A + N);
	Node *root = NULL;
	for (int i = 0; i < N; ++i) insert(root, A[i]);
	printf("%d\n", root->depth);
}


