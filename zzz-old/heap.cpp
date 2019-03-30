/* [heap.cpp] */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
int A[200005], heap[200005];
int n;
// max-heap
void bubbleUp() {
	int child = n - 1;
	int parent = (child - 1) >> 1;
	while (heap[child] > heap[parent]) {
		swap(heap[child], heap[parent]);
		child = parent;
		if (!child) break;
		parent = (child - 1) >> 1;
	}
}
void push(int x) {
	heap[n++] = x;
	bubbleUp();
}
void bubbleDown() {
	int parent = 0;
	for (;;) {
		int left = (parent << 1) + 1;
		int right = (parent << 1) + 2;
		int mx = parent;
		if (left < n && heap[left] > heap[mx])
			mx = left;
		if (right < n && heap[right] > heap[mx])
			mx = right;
		if (mx == parent) break;
		swap(heap[mx], heap[parent]);
		parent = mx;
	}
}
int pop() {
	int ret = heap[0];
	swap(heap[--n], heap[0]);
	bubbleDown();
	return ret;
}
int main() {
}


