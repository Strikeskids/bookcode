/* [simpson.cpp] */
// Assuming f is 4 times differentiable on [a,b],
// the error is ((b-a)/2)^5/90 * f^(4)(eta)
// Derive using int f = int (quartic Taylor polynomial at (b-a)/2)
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
ld f(ld x) {
	return 1;
}
ld integrate(ld a, ld b) {
	return (b - a) * (f(a) + f(b) + 4 * f((a + b) / 2)) / 6;
}
int main() {

}


