// Burnside's Lemma: Let G be a group acting on a set X. Then
// 1) Number of orbits = (sum_G |Fix(g)|) / |G|
// 2) |orbit of x| = |number of cosets of stabilizer G_x := {g : g x = x}| = |G|/|G_x|
// Fun fact: sum{d|n} phi(d) = n.
// Mobius inversion formula:
// f(n) = sum_{d|n} g(d); then g(n) = sum_{d|n} f(d)mob(n/d)
// Euler's theorem (noncoprime): a^(phi(m) + (b mod phi(m))) = a^b (mod m) if b >= phi(m).
// Number of quadratic residues mod odd prime p is exactly (p+1)/2
// Check if a is quadratic residue: a^((p-1)/2) = 1 mod p
// {n, r} = # ways to partition 1..n into r non-empty subsets (second-kind Stirling)
// x^k = sum_{r=1}^k {k, r} (x)(x-1)..(x-k+1)
// Newton's identities: p[k] = sum a[i]^k, e[k] = sum a[i1]a[i2]..a[ik] (i1 < i2 <..)
// k * e[k] = sum_{1 to k} p[j] * (-1)^(j+1) * e[k-j]
// Proof: Differentiate \prod (1-a[i]x) = sum (-1)^k e[k]x^k
// # of n-permutations with r runs is <n,r> = sum_{i=0..r} (-1)^i (n+1 choose i) (r-i)^n
#include <bits/stdc++.h>
using namespace std;
const int MAXN = 1000000;
int phi[MAXN];

const ll relprimes[5] = {1000073001431003663LL, 1000219015039312741LL,
	1000371045812882881LL, 1000481077023105539LL, 1000563105637604653LL};
// <= 1LL << 40
unsigned ll randll() {
	unsigned ll r = 0;
	for (int i = 0; i < 5; ++i) {
	  r = (r << 15) ^ (rand() & 0x7fff);
	}
	return r;
}
ll binom(int a, int b) {
	if (b > a) return 0;
	return (((fac[a] * invfac[b]) % MOD) * invfac[a - b]) % MOD;
}
ll modex(ll a, ll p, ll m = MOD) {
	if (!p) return 1;
	if (p & 1) return a * modex(a, p - 1, m) % m;
	ll v = modex(a, p >> 1, m);
	return v * v % m;
}
// For a, b >= 0, a + b > 0
// Returns x, y such that ax + by = gcd(a, b)
// Furthermore if (a, 0) -> (1, 0); (0, b) -> (0, 1); (a, b) -> |x| <= b, |y| <= a
pair<ll, ll> euclid(ll a, ll b) {
	if (!b) return {1, 0};
	ll x, y;
	tie(x, y) = euclid(b, a % b);
	return {y, x - y * (a / b)};
}
// Returns 0 <= x < m such that a * x = 1 mod m
ll getModInverse(ll a, ll m) {
	assert(a > 0 && m > 0 && __gcd(a, m) == 1);
	auto v = euclid(a, m);
	if (v.first < 0) v.first += m;
	if (v.first >= m) v.first -= m;
	return v.first;
}
int phi(int x) {
	int p = 2, ret = 1;
	while (p * p <= x) {
		if (x % p == 0) {
			int mult = 1;
			do {
				mult *= p;
				x /= p;
			} while (x % p == 0);
			ret *= mult / p * (p - 1);
		}
		++p;
	}
	if (x > 1) ret *= x - 1;
	return ret;
}
void getFactors(int i, vector<pair<int, int>> &pfacs, int v = 1) {
	if (i == pfacs.size()) {
		printf("%d ", v);
		return;
	}
	int w = v;
	for (int j = 0; j <= pfacs[i].second; ++j) {
		getFactors(i + 1, pfacs, w);
		w *= pfacs[i].first;
	}
}
// For numbers up to 10^9
void calcFactors(int x) {
	vector<pair<int, int>> factors;
	int p = 2;
	while (p * p <= x) {
		if (x % p == 0) {
			int cnt = 0;
			do {
				++cnt;
				x /= p;
			} while (x % p == 0);
			factors.push_back({p, cnt});
		}
		++p;
	}
	if (x > 1) factors.push_back({x, 1});
	getFactors(0, factors);
}
int binom[MAXN][MAXN];
int main() {
	fac[0] = invfac[0] = 1;
	for (int i = 1; i <= 200000; ++i) {
		fac[i] = (fac[i - 1] * i) % MOD;
		invfac[i] = modex(fac[i], MOD - 2);
	}
	for (int i = 0; i <= MAXN; ++i)
		for (int j = 0; j <= i; ++j)
			if (!j || j == i) binom[i][j] = 1;
			else binom[i][j] = (binom[i - 1][j - 1] + binom[i - 1][j]) % MOD;
	int pfac[MAXN];
	vector<int> primes;
	for (int i = 2; i < MAXN; ++i) pfac[i] = i;
	for (int i = 2; i < MAXN; ++i)
		if (pfac[i] == i) {
			primes.push_back(i);
			for (int j = i + i; j < MAXN; j += i)
				pfac[j] = i;
		}
	// Prime factors (x <= 10^6)
	while (x > 1) {
		int p = pfac[x];
		int cnt = 0;
		while (x % p == 0) {
			++cnt;
			x /= p;
		}
		factors.push_back({p, cnt});
	}
	// Prime factors (without precomputing)
	int p = 2;
	while (p * p <= x) {
		if (x % p == 0) {
			int cnt = 0;
			do {
				++cnt;
				x /= p;
			} while (x % p == 0);
			factors.push_back({p, cnt});
		}
		++p;
	}
	if (x > 1) factors.push_back({x, 1});
	// Phi function
	phi[1] = 1;
	for (int i = 2; i < MAXN; ++i) {
		int ii = i, p = pfac[i], mult = p - 1;
		ii /= p;
		while (ii % p == 0) {
			ii /= p;
			mult *= p;
		}
		phi[i] = phi[ii] * mult;
	}
}
ll crt2(ll a1, ll m1, ll a2, ll m2) {
	// There exists a 1-1 bijection between (mod ab) and (mod a, mod b)
	// Say ans = y (mod a), ans = z (mod b)
	auto x = euclid(m1, m2);
	ll m = m1 * m2;
	ll ret = (m1 * x.first * a2 + m2 * x.second * a1) % m;
	if (ret < 0) ret += m;
	return ret;
}
// Assume a, m <= 10^9, returns <A, M> such that 0 <= A < M = lcm(m)
pair<ll, ll> crtCoprime(vector<ll> a, vector<ll> m) {
	/* If x = a[i] (mod m[i]), and m[i]'s are pairwise coprime then
	Let M = prod m[i], x = sum (M/m[i]) * a[i] * inv(M/m[i], m[i])) is a solution. */
	int n = a.size();
	ll M = 1, x = 0;
	for (int i = 0; i < n; ++i) M *= m[i];
	for (int i = 0; i < n; ++i) {
		ll v = a[i] * getModInverse(M / m[i], m[i]) % m[i];
		(x += M / m[i] * v) %= M;
	}
	return {x, M};
}
// Assume a, m <= 10^9, returns <A, M> such that 0 <= A < M = lcm(m)
pair<ll, ll> crt(vector<ll> a, vector<ll> m) {
	/* If m[i] are not pairwise coprime, break them into prime factors, group by factors */
	int n = a.size();
	map<ll, pair<ll, ll>> exps;
	for (int i = 0; i < n; ++i) {
		ll x = m[i], p = 2;
		while (p * p <= x) {
			if (x % p == 0) {
				ll pp = 1;
				do {
					pp *= p;
					x /= p;
				} while (x % p == 0);
				if (pp > exps[p].first) exps[p] = {pp, a[i] % pp};
			}
			++p;
		}
		if (x > 1) {
			if (x > exps[x].first) exps[x] = {x, a[i] % x};
		}
	}
	vector<ll> aa, mm;
	for (auto pp : exps) {
		mm.push_back(pp.second.first);
		aa.push_back(pp.second.second);
	}
	auto ans = crtCoprime(aa, mm);
	for (int i = 0; i < n; ++i) {
		if (ans.first % m[i] != a[i]) return {-1, -1};
	}
	return ans;
}
// Bitwise Gaussian elimination, check if there is solution
// Rref means:
// It is in row echelon form (triangular shape).
// Every leading coefficient is 1 and is the only nonzero entry in column.
bool hassol(vector<bitset<MAXN>> M, int N) {
	int R = M.size();
	memset(where, -1, sizeof(where));
	for (int r = 0, c = 0; c <= N && r < R; ++c) {
		for (int i = r; i < R; ++i)
			if (M[i][c]) {
				swap(M[i], M[r]);
				break;
			}
		if (c == N) {
			if (M[r][c]) return false;
			break;
		}
		if (!M[r][c]) continue;
		where[c] = r;
		for (int i = 0; i < R; ++i)
			if (i != r && M[i][c])
				M[i] ^= M[r];
		++r;
	}
	// To recover the solution:
	for (int i = 0; i < N; ++i)
		if (where[i] != -1)
			ans[i] = M[where[i]][N];
	return true;
}
vector<bitset<MAXC>> matrixInverse(vector<bitset<MAXC>> M, int R, int C) {
	assert(MAXC >= 2 * C);
	for (int r = 0; r < R; ++r)
		for (int i = 0; i < C; ++i)
			M[r][i + C] = M[r][i];
	for (int r = 0, c = 0; c < C && r < R; ++c) {
		for (int i = r; i < R; ++i)
			if (M[i][c]) {
				swap(M[i], M[r]);
				break;
			}
		if (!M[r][c]) continue;
		for (int i = 0; i < R; ++i)
			if (i != r && M[i][c])
				M[i] ^= M[r];
		++r;
	}
	vector<bitset<MAXC>> ret(R);
	for (int r = 0; r < R; ++r)
		for (int c = 0; c < C; ++c)
			ret[r] = M[r][c + C];
	return ret;
}
// Finding a primitive root modulo n
// Exists iff n is 1, 2, 4, or n=p^k or n=2p^k (p odd)
// There are exactly phi(phi(n)) primitive roots.
// Only need to check a^(phi(n)/p_i) != 1 where n = prod p_i^k_i.
int primitiveRoot(int n) {
	vector<int> fact;
	int phi = getPhi(n), x = phi;
	for (int i = 2; i * i <= x; ++i)
		if (x % i == 0) {
			fact.push_back(i);
			while (x % i == 0)
				x /= i;
		}
	if (x > 1) fact.push_back(x);
	for (int res = 1; res <= n; ++res) {
		bool ok = true;
		for (int i = 0; i < fact.size() && ok; ++i)
			ok &= modex(res, phi / fact[i], n) != 1;
		if (ok) return res;
	}
	return -1;
}
// Discrete log. Find x < m such that a^x = b (mod m), where (a,m) = 1
int discreteLog(int a, int b, int m) {
	int n = (int)sqrt(m) + 1;
	map<int, int> vals;
	for (int i = n; i >= 1; --i)
		vals[modex(a, i * n, m)] = i;
	for (int i = 0; i <= n; ++i) {
		int cur = (modex(a, i, m) * b) % m;
		if (vals.count(cur)) {
			int ans = vals[cur] * n - i;
			if (ans < m) return ans;
		}
	}
	return -1;
}
// Factorization
ll modmul(ll a, ll b, ll mod) {
	if (!b) return 0;
	if (b & 1) return (a + modmul(a, b - 1, mod)) % mod;
	ll v = modmul(a, b / 2, mod);
	return (v + v) % mod;
}
ll modex2(ll a, ll p, ll mod) {
	if (!p) return 1;
	if (p & 1) return modmul(a, modex2(a, p - 1, mod), mod);
	ll v = modex2(a, p / 2, mod);
	return modmul(v, v, mod);
}
mt19937_64 rng;
// O(log^2 n)
bool miller(ll d, ll n) {
	ll a = 1 + (rng() % (n - 1));
	ll x = modex2(a, d, n);
	if (x == 1 || x == n - 1) return true;
	while (d < n - 1) {
		x = modmul(x, x, n);
		d *= 2;
		if (x == n - 1) return true;
	}
	return false;
}
// O(20 * log^2 n)
bool isPrime(ll n) {
	if (n <= 1)  return false;
	if (n == 2) return true;
	if (n % 2 == 0) return false;
	ll d = n - 1;
	while (d % 2 == 0) d /= 2;
	// Fact: >= 75% of n will test false if n composite
	// Easy to prove: > 50% of n will test false if n composite
	for (int i = 0; i < 20; ++i)
		if (!miller(d, n)) return false;
	return true;
}
// Finds a factor in expected O(n^0.25)
ll pollardRho(ll a) {
	for (;;) {
		ll x = rng() % a;
		ll y = x;
		ll c = rng() % a;
		ll g = 1;
		while (g == 1) {
			x = (modmul(x, x, a) + c) % a;
			y = (modmul(y, y, a) + c) % a;
			y = (modmul(y, y, a) + c) % a;
			g = __gcd(abs(x - y), a);
		}
		if (g != a) return g;
	}
}
// O(log n * max(20 * log^2 n, n^0.25)))
void factorize(ll a, vector<ll> &factors) {
	if (a == 1) return;
	if (a % 2 == 0) {
		factors.push_back(2);
		factorize(a / 2, factors);
		return;
	}
	if (isPrime(a)) {
		factors.push_back(a);
		return;
	}
	ll f = pollardRho(a);
	factorize(f, factors);
	factorize(a / f, factors);
}
