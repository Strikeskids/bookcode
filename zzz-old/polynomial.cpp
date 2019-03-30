/* [polynomial.cpp] */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
typedef unsigned long long ull;
typedef vector<ull> poly;
// Empty vector is zero polynomial
// Complexity: deg(a) * deg(b). Degree: deg(a) + deg(b)
poly multiply(poly a, poly b) {
	if (a.empty() || b.empty()) return {};
	poly c(a.size() + b.size() - 1);
	for (int i = 0; i < a.size(); ++i)
		for (int j = 0; j < b.size(); ++j)
			c[i + j] += a[i] * b[j];
	return c;
}
// Compute a(b(x)). Complexity: deg(a)^2 * deg(b)^2. Degree: deg(a)deg(b)
poly compose(poly a, poly b) {
	poly c;
	for (int i = (int)a.size() - 1; i >= 0; --i) {
		c = multiply(c, b);
		if (c.empty()) c.push_back(a[i]);
		else c[0] += a[i];
	}
	return c;
}
int main() {
}


