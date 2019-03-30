/* [gcc-hacks.cpp] */
#pragma GCC optimize("Ofast")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,sse4.1,sse4.2,popcnt,abm,mmx,avx,tune=native")
#include <bits/stdc++.h>
#include <sys/resource.h>
using namespace std;
void increaseStack() {
	rlimit R;
	getrlimit(RLIMIT_STACK, &R);
	R.rlim_cur = R.rlim_max;
	setrlimit(RLIMIT_STACK, &R);
}
int main() {
	increaseStack();
}

