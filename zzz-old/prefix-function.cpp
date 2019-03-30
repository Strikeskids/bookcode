/* [prefix-function.cpp] */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double Double;
int B[200005];
// Finds string S in string T
bool find(string S, string T) {
	string W = S + "#" + T;
	for (int i = 1; i <= W.length(); ++i) {
		int b = B[i - 1];
		while (b && W[b] != W[i]) b = B[b - 1];
		if (W[b] == W[i]) ++b;
		B[i] = b;
		if (W[i] == S.length()) {
			return true;
		}
	}
	return false;
}
int main() {

}


