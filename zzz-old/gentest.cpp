/* [gentest.cpp] */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
mt19937_64 rng;
string randstr(int len) {
	string s;
	for (int j = 0; j < len; ++j)
		s += 'a' + rng() % 26;
	return s;
}
int main() {
	random_device rd;
	rng.seed(rd());
	for (;;) {
		FILE *fin = fopen("b.in", "w");
		int N = 6;
		fprintf(fin, "%d\n", N);
		for (int i = 0; i < N; ++i) {
			int l = 3 + (rand() % 5);
			fprintf(fin, "%s\n", randstr(l).c_str());
		}
		fprintf(fin, "\n");
		fclose(fin);
		system("type b.in");
		system("b < b.in > b.out");
	}
}


