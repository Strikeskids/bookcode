/* [fft.cpp] */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
struct cpx {
	double r, i;
	cpx(double r = 0, double i = 0) : r(r), i(i) {}
	cpx operator+(cpx b) { return {r + b.r, i + b.i}; }
	cpx operator-(cpx b) { return {r - b.r, i - b.i}; }
	cpx operator*(cpx b) { return {r * b.r - i * b.i, r * b.i + i * b.r}; }
	cpx operator/(double b) { return {r / b, i / b}; }
};
const double PI = acos(-1);
// FFT-iterative: fast
// This version is faster than NTT-iterative
namespace fft {
	cpx rootpw[MAXN];
	void fft(cpx *a, int n) {
		for (int i = 0; i < n; ++i) {
			int j = 0;
			int x = i, y = n - 1;
			while (y) {
				j = (j << 1) + (x & 1);
				x >>= 1;
				y >>= 1;
			}
			if (i < j) swap(a[i], a[j]);
		}
		for (int len = 1; len < n; len <<= 1) {
			double angle = 2 * PI / (2 * len);
			cpx root(cos(angle) , sin(angle));
			rootpw[0] = 1;
			for (int i = 1; i < len; ++i) rootpw[i] = rootpw[i - 1] * root;
			for (int i = 0; i < n; i += 2 * len) {
				for (int j = 0; j < len; ++j) {
					cpx u = a[i + j];
					cpx v = a[i + j + len] * rootpw[j];
					a[i + j] = u + v;
					a[i + j + len] = u - v;
				}
			}
		}
	}
	void multiply(cpx *A, cpx *B, int *C, int n) {
		fft(A, n);
		fft(B, n);
		for (int i = 0; i < n; ++i) A[i] = A[i] * B[i] / n;
		fft(A, n);
		reverse(A + 1, A + n);
		for (int i = 0; i < n; ++i) C[i] = lround(A[i].r);
	}
	cpx AA[MAXN], BB[MAXN];
	void multiply(int *A, int *B, int *C, int n) {
		for (int i = 0; i < n; ++i) {
			AA[i] = A[i];
			BB[i] = B[i];
		}
		multiply(AA, BB, C, n);
	}
};

// FFT-recursive: short!
// This is slower than NTT-recursive
namespace fft {
	cpx AV[MAXN], BV[MAXN], CC[MAXN];
	void fft(cpx *in, cpx *out, int step, int n) {
		if (n == 1) {
			out[0] = in[0];
			return;
		}
		int m = n >> 1;
		fft(in, out, step << 1, m);
		fft(in + step, out + m, step << 1, m);
		double angle = 2 * PI / n;
		cpx root(cos(angle), sin(angle));
		cpx w = 1;
		for (int i = 0; i < m; ++i) {
			cpx u = out[i];
			cpx v = out[i + m] * w;
			out[i] = u + v;
			out[i + m] = u - v;
			w = w * root;
		}
	}
	void multiply(cpx *A, cpx *B, int *C, int n) {
		fft(A, AV, 1, n);
		fft(B, BV, 1, n);
		for (int i = 0; i < n; ++i) AV[i] = AV[i] * BV[i] / n;
		fft(AV, CC, 1, n);
		reverse(CC + 1, CC + n);
		for (int i = 0; i < n; ++i) C[i] = lround(CC[i].r);
	}
	cpx AA[MAXN], BB[MAXN];
	void multiply(int *A, int *B, int *C, int n) {
		for (int i = 0; i < n; ++i) {
			AA[i] = A[i];
			BB[i] = B[i];
		}
		multiply(AA, BB, C, n);
	}
};


