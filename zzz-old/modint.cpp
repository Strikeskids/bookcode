/* [modint.cpp] */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
const ll MOD = 1000000007;
struct modint {
	ll a = 0;
	modint() {}
	modint(ll x) { *this = x; }
	void operator=(ll x){
		a = x % MOD;
		if (a < 0) a += MOD;
	}
	bool operator==(modint y) { return a == y.a; }
	bool operator!=(modint y) { return a != y.a; }
	modint operator+(modint y) { return a + y.a; }
	modint operator-(modint y) { return a - y.a; }
	modint operator*(modint y) { return a * y.a; }
	modint operator/(modint y);
};
modint pow(modint x, int y) {
	if (!y) return 1;
	if (y & 1) return x * pow(x * x, y >> 1);
	return pow(x * x, y >> 1);
}
modint modint::operator/(modint y) { return *this * pow(y, MOD - 2); }
ostream &operator<<(ostream &os, const modint &x) {
	return os << x.a;
}
int main() {
	modint a(2000000000);
	cout << a << endl;
}


