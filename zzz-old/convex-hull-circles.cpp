/* [convex-hull-circles.cpp] */
/* Find the convex hull of circles. */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
const ld EPS = 1e-8;
const ld PI = acosl(-1);
inline bool eq(ld a, ld b) {
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
	Pt rot90() { return Pt{-y, x}; }
	ld abs() const { return hypotl(x, y); }
	bool operator==(const Pt &p) const { return eq(x, p.x) && eq(y, p.y); }
	bool operator!=(const Pt &p) const { return !(*this == p); }
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
vector<Pt> tangentCirclePoint(Pt c, ld r, Pt p) {
	// r > 0
	ld x = dot(p - c, p - c);
	ld d = x - r * r;
	if (d < -EPS) return {}; // p in circle interior
	Pt q1 = (p - c) * (r * r / x);
	if (d < EPS) return {c + q1};
	Pt q2 = ((p - c) * (-r * sqrt(d) / x)).rot90();
	return {c + q1 - q2, c + q1 + q2};
}
vector<pair<Pt, Pt>> tangentCircleCircle(Pt c1, ld r1, Pt c2, ld r2) {
	// assert((c1, r1) != (c2, r2))
	// assert r1, r2 > 0
	vector<pair<Pt, Pt>> res;
	ld d = dist(c1, c2);
	if (d + min(r1, r2) < max(r1, r2) - EPS) return res; // one circle contained in the other
	if (abs(r1 - r2) < EPS) {
		Pt dir = c2 - c1;
		dir = (dir * (r1 / d)).rot90();
		res.push_back({c1 + dir, c2 + dir});
		res.push_back({c1 - dir, c2 - dir});
	} else {
		Pt p = ((c1 * -r2) + (c2 * r1)) / (r1 - r2);
		vector<Pt> ps = tangentCirclePoint(c1, r1, p), qs = tangentCirclePoint(c2, r2, p);
		for (int i = 0; i < ps.size() && i < qs.size(); ++i) {
			res.push_back({ps[i], qs[i]});
		}
	}
	return res;
}
int ccw(Pt p, Pt q, Pt r) {
	ld v = cross(p - q, p - r);
	if (v < -EPS) return -1;
	if (v > EPS) return 1;
	return 0;
}
Pt P[205], Q[205];
ld R[205], QR[205];
bool used[205][205];
int main() {
	int m;
	scanf("%d", &m);
	for (int i = 0; i < m; ++i) {
		scanf("%Lf%Lf%Lf", &Q[i].x, &Q[i].y, &QR[i]);
	}
	// Remove circles contained in other circles
	int n = 0;
	for (int i = 0; i < m; ++i) {
		bool contained = false;
		for (int j = 0; j < m; ++j) {
			if (Q[i] == Q[j] && eq(QR[i], QR[j])) {
				if (i < j) contained = true;
			} else if (j != i && dist(Q[i], Q[j]) + QR[i] <= QR[j] + EPS) contained = true;
		}
		if (!contained) {
			P[n] = Q[i];
			R[n++] = QR[i];
		}
	}
	if (n == 1) {
		printf("%.9Lf\n", 2 * PI * (R[0] + 10));
		return 0;
	}
	// Perform a wrap starting from the leftmost circle
	int p = 0;
	for (int i = 0; i < n; ++i) {
		if (P[i].x - R[i] < P[p].x - R[p] ||
			(eq(P[i].x - R[i], P[p].x - R[p]) && P[i].y < P[p].y)) p = i;
	}
	ld curang = -PI;
	Pt cur = P[p] - Pt(R[p], 0);
	ld ans = 0;
	bool first = true;
	for (;;) {
		// Find next point
		ld bestdiff = 1e100;
		int pick;
		Pt pp;
		ld pang, segdist;
		for (int i = 0; i < n; ++i) {
			if (i == p) continue;
			auto pts = tangentCircleCircle(P[p], R[p], P[i], R[i]);
			for (auto v : pts) {
				Pt q = v.first;
				ld ang = atan2(q.y - P[p].y, q.x - P[p].x);
				ld diff = ang - curang;
				while (diff < -EPS) diff += 2 * PI;
				diff = max(diff, (ld)0);
				if (diff < bestdiff - EPS && ccw(P[p], v.first, v.second) > 0) {
					bestdiff = diff;
					pick = i;
					pang = ang;
					pp = v.second;
					segdist = dist(v.first, v.second);
				}
			}
		}
		if (!first) ans += bestdiff * (R[p] + 10);
		first = false;
		if (used[p][pick]) break;
		used[p][pick] = true;
		ans += segdist;
		curang = pang;
		p = pick;
		cur = pp;
	}
	printf("%.9Lf\n", ans);
}


