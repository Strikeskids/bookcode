#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
// Get smallest power greater than resdeg
int n = 1 << (32 - __builtin_clz(resdeg));

// NTT with CRT: iterative (as fast as possible)
namespace fft {
	const int mods[2] = {998244353, 897581057}; // 2^23 * 119 + 1, 2^23 * 107 + 1
	const int gens[2] = {3, 3};
	int mod, gen;
	int rootpw[MAXN], AV[MAXN], BV[MAXN];
	ll CC[2][MAXN];
	int modex(int a, int p) {
		if (!p) return 1;
		if (p & 1) return (ll)a * modex(a, p - 1) % mod;
		ll v = modex(a, p >> 1);
		return v * v % mod;
	}
	void fft(int *a, int n) {
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
			int root = modex(gen, (mod - 1) / (2 * len));
			rootpw[0] = 1;
			for (int i = 1; i < len; ++i) rootpw[i] = (ll)rootpw[i - 1] * root % mod;
			for (int i = 0; i < n; i += 2 * len) {
				for (int j = 0; j < len; ++j) {
					int u = a[i + j], v = (ll)a[i + j + len] * rootpw[j] % mod;
					a[i + j] = u + v >= mod ? u + v - mod : u + v;
					a[i + j + len] = u - v < 0 ? u - v + mod : u - v;
				}
			}
		}
	}
	void multiply(int *A, int *B, ll *C, int n) {
		for (int t = 0; t < 2; ++t) {
			mod = mods[t];
			gen = gens[t];
			for (int i = 0; i < n; ++i) {
				AV[i] = A[i];
				BV[i] = B[i];
			}
			fft(AV, n);
			fft(BV, n);
			int inv = modex(n, mod - 2);
			for (int i = 0; i < n; ++i) AV[i] = (ll)AV[i] * BV[i] % mod;
			fft(AV, n);
			reverse(AV + 1, AV + n);
			for (int i = 0; i < n; ++i) CC[t][i] = (ll)AV[i] * inv % mod;
		}
		// Perform CRT to make result big enough
		// a = a1 + x * p1 (mod p1p2)
		ll bigmod = (ll)mods[0] * mods[1];
		ll p12 = modex(mods[0], mods[1] - 2);
		for (int i = 0; i < n; ++i) {
			ll x = (CC[1][i] - CC[0][i]) * p12 % mods[1];
			C[i] = (CC[0][i] + x * mods[0]) % bigmod;
			if (C[i] < 0) C[i] += bigmod;
		}
	}
};

// NTT-iterative with cache locality: fast
namespace fft {
	const int mod = 998244353; // 2^23 * 119 + 1
	const int gen = 3; // ord(3) = 2^23 * 119
	int rootpw[MAXN], AV[MAXN], BV[MAXN];
	int modex(int a, int p) {
		if (!p) return 1;
		if (p & 1) return (ll)a * modex(a, p - 1) % mod;
		ll v = modex(a, p >> 1);
		return v * v % mod;
	}
	void fft(int *a, int n) {
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
			int root = modex(gen, (mod - 1) / (2 * len));
			rootpw[0] = 1;
			for (int i = 1; i < len; ++i) rootpw[i] = (ll)rootpw[i - 1] * root % mod;
			for (int i = 0; i < n; i += 2 * len) {
				for (int j = 0; j < len; ++j) {
					int u = a[i + j], v = (ll)a[i + j + len] * rootpw[j] % mod;
					a[i + j] = u + v >= mod ? u + v - mod : u + v;
					a[i + j + len] = u - v < 0 ? u - v + mod : u - v;
				}
			}
		}
	}
	void multiply(int *A, int *B, int *C, int n) {
		for (int i = 0; i < n; ++i) {
			AV[i] = A[i];
			BV[i] = B[i];
		}
		fft(AV, n);
		fft(BV, n);
		int inv = modex(n, mod - 2);
		for (int i = 0; i < n; ++i) C[i] = (ll)AV[i] * BV[i] % mod;
		fft(C, n);
		reverse(C + 1, C + n);
		for (int i = 0; i < n; ++i) C[i] = (ll)C[i] * inv % mod;
	}
};

// NTT-recursive with only 1 prime: short
namespace fft {
	const int mod = 998244353; // 2^23 * 119 + 1
	const int gen = 3; // ord(3) = 2^23 * 119
	int rootpw[MAXN], AV[MAXN], BV[MAXN];
	int modex(int a, int p) {
		if (!p) return 1;
		if (p & 1) return (ll)a * modex(a, p - 1) % mod;
		ll v = modex(a, p >> 1);
		return v * v % mod;
	}
	void fft(int *in, int *out, int step, int n) {
		if (n == 1) {
			out[0] = in[0];
			return;
		}
		int m = n >> 1;
		fft(in, out, step << 1, m);
		fft(in + step, out + m, step << 1, m);
		for (int i = 0; i < m; ++i) {
			int u = out[i];
			int v = (ll)out[i + m] * rootpw[i * step] % mod;
			out[i] = u + v >= mod ? u + v - mod : u + v;
			out[i + m] = u - v < 0 ? u - v + mod : u - v;
		}
	}
	void multiply(int *A, int *B, int *C, int n) {
		int root = modex(gen, (mod - 1) / n);
		rootpw[0] = 1;
		for (int i = 1; i < n; ++i) rootpw[i] = (ll)rootpw[i - 1] * root % mod;
		fft(A, AV, 1, n);
		fft(B, BV, 1, n);
		int inv = modex(n, mod - 2);
		for (int i = 0; i < n; ++i) AV[i] = (ll)AV[i] * BV[i] % mod;
		fft(AV, C, 1, n);
		reverse(C + 1, C + n);
		for (int i = 0; i < n; ++i) C[i] = (ll)C[i] * inv % mod;
	}
};
