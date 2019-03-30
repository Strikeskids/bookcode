/* [bigint.cpp] */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
struct BigInt {
	vector<int> num;
	BigInt(int m = 0) : num(1, m) {}
	void remLeadingZeros() {
		while (num.size() > 1 && !num.back()) num.pop_back();
	}
	void add(BigInt &x) {
		int maxs = max(num.size(), x.num.size());
		num.resize(maxs);
		x.num.resize(maxs);
		int carry = 0;
		for (int i = 0; i < maxs; ++i) {
			num[i] += x.num[i] + carry;
			carry = num[i] / 10;
			num[i] %= 10;
		}
		if (carry) num.push_back(carry);
	}
	// 1 <= m <= 9
	void multiply(int m) {
		int carry = 0;
		for (int i = 0; i < num.size(); ++i) {
			num[i] = num[i] * m + carry;
			carry = num[i] / 10;
			num[i] %= 10;
		}
		if (carry) num.push_back(carry);
	}
	int divideAndRemainder(int d) {
		int cur = 0;
		vector<int> ret;
		for (int i = (int)num.size() - 1; i >= 0; --i) {
			cur = cur * 10 + num[i];
			ret.push_back(cur / d);
			cur %= d;
		}
		reverse(ret.begin(), ret.end());
		num = ret;
		remLeadingZeros();
		return cur;
	}
	void print() {
		for (int i = (int)num.size() - 1; i >= 0; --i)
			printf("%d", num[i]);
		printf("\n");
	}
};
int main() {
}


