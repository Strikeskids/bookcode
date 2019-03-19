typedef long long ll;

/* 1.5x faster than modmul2 */
ll modmul(ll a, ll b, ll m) {
	// assert(0 <= a && a < m && 0 <= b && b < m);
	return (a*b-(ll)(a/(long double)m*b+1e-3)*m+m)%m;
}

ll modmul2(ll a, ll b, ll m) {
	// assert(0 <= a && a < m && 0 <= b && b < m);
	ll result, quot;
	__asm__ (
		"imulq %[op2]; idivq %[modulus]"
		: "=d" (result), "=a" (quot)
		: [op1] "a" (a), [op2] "d" (b), [modulus] "r" (m)
		: 
	);
	return result;
}

ll modpow(ll b, ll e, ll m) {
	assert(0 <= b && b < m && e >= 0);
	ll result = 1;
	while (e) {
		if (e & 1) result = modmul(result, b, m);
		b = modmul(b, b, m);
		e >>= 1;
	}
	return result;
}

ll gcd(ll a, ll b) {
	while (b) { ll c = a % b; a = b; b = c; }
	return a;
}

// #include <random>
mt19937 gen;
bool miller_rabin(ll n, int rounds = 50) {
	assert(n>1);
	if (!(n & 1)) return n==2;
	if (n < 15) return n!=9;
	ll d=n-1,r=0;
	while (!(d & 1)) { d>>=1; r++; }
	uniform_int_distribution<ll> params(2, n-2);

witness: while (rounds-- > 0) {
		ll x = modpow(params(gen), d, n);
		if (x == 1 || x == n-1) goto witness;
		for (int i=1;i<r;++i) {
			x = modmul(x, x, n);
			if (x == n-1) goto witness;
		}
		return false;
	}
	return true;
}

bool isprime(ll n) {
	return n>1&&miller_rabin(n);
}

/* Expect <= .5ms each for uniform n */
ll pollard_rho(ll n) {
	// assert(n > 1 && !isprime(n));
	if (!(n&1)) return 2;
	uniform_int_distribution<ll> params(1, n);
	ll c = params(gen), x = params(gen), y = x, d;
	do {
		x = (modmul(x, x, n) + c) % n;
		y = (modmul(y, y, n) + c) % n;
		y = (modmul(y, y, n) + c) % n;
		d = gcd(abs(x-y), n);
	} while (d == 1);
	return d == n ? pollard_rho(n) : d;
}

// #include <map>
void factor(ll n, map<ll, int> &factors) {
	assert(n>0);
	while (!(n&1)) { factors[2]++; n>>=1; }
	if (n == 1) return;
	if (isprime(n)) { factors[n]++; return; }
	ll fac = pollard_rho(n);
	factor(fac, factors); factor(n / fac, factors);
}

// yarin's compressed sieve
#define MAXSIEVE 1000000 // All prime numbers up to this
#define MAXSIEVEHALF (MAXSIEVE/2)
#define MAXSQRT 500 // sqrt(MAXSIEVE)/2
char sieve[MAXSIEVE/16+2];
#define issieveprime(n) (!(sieve[(n)>>4]&(1<<(((n)>>1)&7)))) // Works when n is odd

void init_sieve() {
	sieve[0]=1;
	for(int i=1;i<MAXSQRT;i++)
	if (!(sieve[i>>3]&(1<<(i&7))))
	for(int j=3*i+1;j<MAXSIEVEHALF;j+=i+i+1)
	sieve[j>>3]|=(1<<(j&7));
}