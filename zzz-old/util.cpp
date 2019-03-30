/* [util.cpp] */
#pragma GCC optimize("Ofast")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,sse4.1,sse4.2,popcnt,abm,mmx,avx,tune=native")
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
vector<string> split(char *P) {
	// Note this modifies P!
	char *str;
	str = strtok(P, "*");
	vector<string> L;
	while (str != NULL) {
		L.push_back(string(str));
		str = strtok(NULL, "*");
	}
	return L;
}
int main() {
	mt19937_64 gen;
	gen.seed(42);
	printf("%llu\n", gen());
	shuffle(P + 1, P + N + 1, gen);

	// Iterate through all subsets except emptyset
	for (int y = x; y; y = (y - 1) & x);
}


