#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
ld EPS = 1e-12;
bool eq(ld a, ld b) {
	return abs(a - b) < EPS;
}
struct Pt {
	ld x, y; 
	Pt() {}
	Pt(ld x, ld y) : x(x), y(y) {}
	Pt(const Pt &p) : x(p.x), y(p.y) {}
	Pt operator+(const Pt &p) const { return Pt(x + p.x, y + p.y); }
	Pt operator-(const Pt &p) const { return Pt(x - p.x, y - p.y); }
	Pt operator*(ld c) const { return Pt(x * c, y * c); }
	Pt operator/(ld c) const { return Pt(x / c, y / c); }
	bool operator<(const Pt &p) const {
		if (eq(x, p.x)) return y < p.y;
		return x < p.x;
	}
	Pt rot() { return {-y, x}; }
	ld abs() const { return hypotl(x, y); }
};
ld dot(Pt p, Pt q) {
	return p.x * q.x + p.y * q.y;
}
ld dist2(Pt p, Pt q) {
	return dot(p - q, p - q);
}
ld dist(Pt p, Pt q) {
	return sqrt(dist2(p, q));
}
ld cross(Pt p, Pt q) {
	return p.x * q.y - p.y * q.x;
}
bool linesParallel(Pt a, Pt b, Pt c, Pt d) { 
	return abs(cross(b - a, c - d)) < EPS;
}
bool linesCollinear(Pt a, Pt b, Pt c, Pt d) { 
	return linesParallel(a, b, c, d)
	&& abs(cross(a - b, a - c)) < EPS
	&& abs(cross(c - d, c - a)) < EPS; 
}
bool segmentsIntersect(Pt a, Pt b, Pt c, Pt d) {
	if (linesCollinear(a, b, c, d)) {
		if (dist2(a, c) < EPS || dist2(a, d) < EPS ||
			dist2(b, c) < EPS || dist2(b, d) < EPS) return true;
		if (dot(c - a, c-b) > 0 && dot(d - a, d - b) > 0 && dot(c - b, d - b) > 0)
			return false;
		return true;
	}
	if (cross(d - a, b - a) * cross(c - a, b - a) > 0) return false;
	if (cross(a - c, d - c) * cross(b - c, d - c) > 0) return false;
	return true;
}
Pt computeLineIntersection(Pt a, Pt b, Pt c, Pt d) {
	b = b - a;
	d = c - d;
	c = c - a;
	assert(dot(b, b) > EPS && dot(d, d) > EPS);
	return a + b * cross(c, d) / cross(b, d);
}
vector<Pt> polys[105];
int main() {
	int N;
	scanf("%d", &N);
	for (int i = 0; i < N; ++i) {
		int M;
		scanf("%d", &M);
		polys[i].resize(M);
		for (int j = 0; j < M; ++j) {
			scanf("%Lf%Lf", &polys[i][j].x, &polys[i][j].y);
		}
	}
	vector<ld> xs;
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < polys[i].size(); ++j) {
			int nj = (j + 1) % polys[i].size();
			xs.push_back(polys[i][j].x);
			for (int ii = i + 1; ii < N; ++ii)
				for (int jj = 0; jj < polys[ii].size(); ++jj) {
					int njj = (jj + 1) % polys[ii].size();
					if (segmentsIntersect(polys[i][j], polys[i][nj], polys[ii][jj], polys[ii][njj])) {
						auto p = computeLineIntersection(polys[i][j], polys[i][nj], polys[ii][jj], polys[ii][njj]);
						xs.push_back(p.x);
					}
				}
		}
	sort(xs.begin(), xs.end());
	xs.resize(unique(xs.begin(), xs.end(), eq) - xs.begin());
	ld ans = 0;
	for (int k = 0; k < (int)xs.size() - 1; ++k) {
		ld x1 = xs[k], x2 = xs[k + 1];
		vector<pair<Pt, int>> evts;
		for (int i = 0; i < N; ++i) {
			vector<Pt> lines;
			for (int j = 0; j < polys[i].size(); ++j) {
				int nj = (j + 1) % polys[i].size();
				Pt p = polys[i][j], q = polys[i][nj];
				if (p.x > q.x) swap(p, q);
				if (p.x < x1 + EPS && q.x > x2 - EPS) {
					ld grad = (q.y - p.y) / (q.x - p.x);
					ld y1 = p.y + (x1 - p.x) * grad;
					ld y2 = p.y + (x2 - p.x) * grad;
					lines.push_back({y1, y2});
				}
			}
			sort(lines.begin(), lines.end());
			int flag = 1;
			for (auto &l : lines) {
				evts.push_back({l, flag});
				flag = -flag;
			}
		}
		sort(evts.begin(), evts.end());
		int cnt = 0;
		for (int i = 0; i < evts.size(); ++i) {
			if (cnt) {
				ans += (evts[i].first.x + evts[i].first.y - evts[i - 1].first.x - evts[i - 1].first.y)
					* (x2 - x1) / 2;
			}
			cnt += evts[i].second;
		}
	}
	printf("%.9Lf\n", ans);
}

