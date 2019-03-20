#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <vector>
#include <deque>
#include <algorithm>
#include <string>

using namespace std;

#include "aho-corasick.cpp"

void naive_match(char *hay, int n, char **needles, vector<pair<int, int>> &matches) {
	for (int i = 0; i < n; ++i) {
		char *needle = needles[i];
		int nlen = strlen(needle);
		char *h = hay;
		while (true) {
			h = strstr(h, needle);
			if (!h) break;
			matches.push_back(make_pair(h - hay, h - hay + nlen));
			h++;
		}
	}
}

void matcheq(vector<pair<int,int>> &m1, vector<pair<int,int>> &m2) {
	sort(m1.begin(), m1.end());
	sort(m2.begin(), m2.end());
	assert(m1.size() == m2.size());
	assert(m1 == m2);
}

const int NTEST = 100;
const int HSIZE = 100;
const int NSUBNEEDLE = 20;
const int NNEEDLE = 100;
const int NSIZE = 100;
const int NUMC = 4;
char hay[HSIZE+1];
char nbuf[NNEEDLE * (NSIZE+1)];
char *needles[NNEEDLE];

int main() {
	vector<pair<int, int>> m1, m2;
	for (int test = 0; test < NTEST; ++test) {
		for (int i = 0; i < HSIZE; ++i) {
			hay[i] = (rand() % NUMC) + 'a';
		}
		int nptr = 0;
		for (int i = 0; i < NSUBNEEDLE; ++i) {
			int start = rand() % HSIZE;
			int end = min(HSIZE, start + (rand() % NSIZE) + 1);
			int len = end - start;
			char *needle = needles[i] = &nbuf[nptr];
			nptr += len + 1;
			memcpy(needle, &hay[start], end - start);
			needle[end-start] = 0;
		}
		for (int i = NSUBNEEDLE; i < NNEEDLE; ++i) {
			int len = 1 + rand() % NSIZE;
			char *needle = &nbuf[nptr];
			nptr += len;
			for (int j = 0; j < len; ++j) {
				needle[j] = (rand() % NUMC) + 'a';
			}
			needle[len] = 0;
			needles[i] = needle;
		}
		m1.clear(); build_and_match(hay, NNEEDLE, needles, m1);
		m2.clear(); naive_match(hay, NNEEDLE, needles, m2);
		matcheq(m1, m2);
	}
	return 0;
}