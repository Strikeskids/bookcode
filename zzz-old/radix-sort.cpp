/* [radix-sort.cpp] */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
const int MAXN = 1 << 25;
int A[MAXN], B[MAXN], n;
const int MAXC = 1 << 16;
int cnt[MAXC], pos[MAXC];
void radixSort() {
	memset(cnt, 0, sizeof(cnt));
	unsigned int mask = MAXC - 1;
	for (int i = 0; i < n; ++i) ++cnt[A[i] & mask];
	pos[0] = 0;
	for (int i = 1; i < MAXC; ++i) pos[i] = pos[i - 1] + cnt[i - 1];
	for (int i = 0; i < n; ++i) B[pos[A[i] & mask]++] = A[i];
	mask <<= 16;
	memset(cnt, 0, sizeof(cnt));
	for (int i = 0; i < n; ++i) ++cnt[(B[i] & mask) >> 16];
	pos[0] = 0;
	for (int i = 1; i < MAXC; ++i) pos[i] = pos[i - 1] + cnt[i - 1];
	for (int i = 0; i < n; ++i) A[pos[(B[i] & mask) >> 16]++] = B[i];
}
int main() {
	n = MAXN;
	for (int i = 0; i < n; ++i) B[i] = A[i] = (rand() << 15) | rand();
	{
		auto start = clock();
		sort(A, A + n);
		auto end = clock();
		printf("sort takes %d time\n", end - start);
	}
	for (int i = 0; i < n; ++i) A[i] = B[i];
	{
		auto start = clock();
		radixSort();
		auto end = clock();
		printf("radix-sort takes %d time\n", end - start);
	}
}


