/* [walsh-hadamard.cpp] */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
// XOR-transform
// Let n = 2^k. Given (a[0],...,a[n-1]), (b[0],...,b[n-1]), compute
// c[i] = sum_{j xor j'=i} a[j]b[j']
// H_k = ((H[k-1], H[k-1]), (H[k-1], -H[k-1]))
// H_k(H_k a * H_k b) = 2^k c
// H_k^2 = 2^k I
// transform(a) computes H_k(a)
void transform(ll *a, int n) {
	for (int len = 1; len < n; len <<= 1)
		for (int i = 0; i < n; i += 2 * len)
			for (int j = 0; j < len; ++j) {
				ll u = a[i + j], v = a[i + j + len];
				a[i + j] = u + v;
				a[i + j + len] = u - v;
			}
}
// Note C[i] can become as big as n^2 * max A * max B
void convolveXor(ll *A, ll *B, ll *C, int n) {
	transform(A, n);
	transform(B, n);
	for (int i = 0; i < n; ++i) C[i] = A[i] * B[i];
	transform(C, n);
	for (int i = 0; i < n; ++i) C[i] /= n;
}
// OR-transform
// Let n = 2^k. Given (a[0],...,a[n-1]), (b[0],...,b[n-1]), compute
// c[i] = sum_{j or j'=i} a[j]b[j']
// H_k = ((H[k-1], 0), (H[k-1], H[k-1]))
// J_k = ((J[k-1], 0), (-J[k-1], J[k-1]))
// J_k(H_k a * H_k b) = c
// J_k H_k = I
// transform(a) computes H_k(a), transform(a, flag) computes J_k(a)
void transform(ll *a, int n, bool inv = false) {
	for (int len = 1; len < n; len <<= 1)
		for (int i = 0; i < n; i += 2 * len)
			for (int j = 0; j < len; ++j) {
				ll u = a[i + j], v = a[i + j + len];
				a[i + j] = u;
				a[i + j + len] = inv ? -u + v : u + v;
			}
}
// Note C[i] can become as big as n^2 * max A * max B
void convolveOr(ll *A, ll *B, ll *C, int n) {
	transform(A, n);
	transform(B, n);
	for (int i = 0; i < n; ++i) C[i] = A[i] * B[i];
	transform(C, n, true);
}
// AND-transform
// Let n = 2^k. Given (a[0],...,a[n-1]), (b[0],...,b[n-1]), compute
// c[i] = sum_{j and j'=i} a[j]b[j']
// H_k = ((0, H[k-1]), (H[k-1], H[k-1]))
// J_k = ((-J[k-1], J[k-1]), (J[k-1], 0))
// J_k(H_k a * H_k b) = c
// J_k H_k = I
// transform(a) computes H_k(a), transform(a, flag) computes J_k(a)
void transform(ll *a, int n, bool inv = false) {
	for (int len = 1; len < n; len <<= 1)
		for (int i = 0; i < n; i += 2 * len)
			for (int j = 0; j < len; ++j) {
				ll u = a[i + j], v = a[i + j + len];
				if (inv) {
					a[i + j] = -u + v;
					a[i + j + len] = u;
				} else {
					a[i + j] = v;
					a[i + j + len] = u + v;
				}
			}
}
// Note C[i] can become as big as n^2 * max A * max B
void convolveAnd(ll *A, ll *B, ll *C, int n) {
	transform(A, n);
	transform(B, n);
	for (int i = 0; i < n; ++i) C[i] = A[i] * B[i];
	transform(C, n, true);
}
int main() {
}


