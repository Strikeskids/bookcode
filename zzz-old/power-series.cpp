/* [power-series.cpp] */
#include <bits/stdc++.h>
using namespace std;
#define db(x) cerr << #x << "=" << x << endl
#define db2(x, y) cerr << #x << "=" << x << "," << #y << "=" << y << endl
#define db3(x, y, z) cerr << #x << "=" << x << "," << #y << "=" << y << "," << #z << "=" << z << endl
typedef long long ll;
typedef long double ld;
int main() {
	// Newton's method: want to find P such that F(P) = 0.
	// Say we have P - P[n] = 0 mod x^n.
	// Set P[2n] = P[n] - F(P[n]) / F'(P[n])
	// Then P - P[2n] = 0 mod x^{2n}.
	// E.g. To find P=1/Q, set P[2n] = P[n](2 - Q * P[n])
	// For Q=P^k, set P[2n] = P[n]((k-1)/k + Q/(k*P[n]^k))
	// For Q=exp(P), set P[2n] = P[n](1 + Q - log(P[n]))
	// Note for intermediate inverses and log, we need accuracy up to 2n
}


