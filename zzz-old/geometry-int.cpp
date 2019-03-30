/* [geometry-int.cpp] */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
struct Pt {
	ll x, y;
	Pt() {}
	Pt(ll _x, ll _y) : x(_x), y(_y) {}
	Pt operator+(const Pt &p) const { return Pt(x + p.x, y + p.y); }
	Pt operator-(const Pt &p) const { return Pt(x - p.x, y - p.y); }
	Pt operator*(ll c) const { return Pt(x * c, y * c); }
	Pt operator/(ll c) const { return Pt(x / c, y / c); }
	ll abs2() { return x * x + y * y; }
};
// Sort by atan2(y, x).
// Any x < 0, y = 0 will be first.
bool angleCmp(Pt a, Pt b) {
	assert((a.x || a.y) && (b.x || b.y));
	if (a.y <= 0 && b.y > 0) return true;
	if (a.y > 0 && b.y <= 0) return false;
	if (!a.y && !b.y) return a.x < b.x;
	return a.x * b.y > b.x * a.y;
}
// Positive if b is to the left of a
ll cross(Pt a, Pt b) { return a.x * b.y - a.y * b.x; }
ll dot(Pt a, Pt b) { return a.x * b.x + a.y * b.y; }
int sign(ll v) { return v > 0 ? 1 : v < 0 ? -1 : 0; }
bool isAcute(Pt a, Pt b) {
	// b's angle counterclockwise from a is < 90
	return dot(a, b) > 0 && cross(a, b) >= 0;
}
bool isAcute2(Pt a, Pt b) {
	// b's angle counterclockwise from a is > 270
	return dot(a, b) > 0 && cross(a, b) < 0;
}
int ccw(Pt p, Pt q, Pt r) {
	ll v = cross(p - q, p - r);
	if (v < 0) return -1;
	if (v > 0) return 1;
	return 0;
}
// Returns ccw(a,b,c) if in circumcircle, -ccw(a,b,c) if outside, 0 if on
// assert(ccw(a,b,c))
int incircle(Pt p, Pt a, Pt b, Pt c) {
	ll az = a.x * a.x + a.y * a.y;
	ll bz = b.x * b.x + b.y * b.y;
	ll cz = c.x * c.x + c.y * c.y;
	ll pz = p.x * p.x + p.y * p.y;
	ll d1 = a.y * (bz - cz) - b.y * (az - cz) + c.y * (az - bz);
	ll d2 = a.x * (bz - cz) - b.x * (az - cz) + c.x * (az - bz);
	ll d3 = cross(b, c) - cross(a, c) + cross(a, b);
	ll d4 = az * cross(b, c) - bz * cross(a, c) + cz * cross(a, b);
	ll d = -d1 * p.x + d2 * p.y - d3 * pz + d4;
	if (d < 0) return -1;
	if (d > 0) return 1;
	return 0;
}

// Get counter-clockwise convex hull, starting at leftmost bottommost point
// Removes all collinear points. If points all collinear, size is 2.
vector<Pt> convexHull(vector<Pt> &P) {
	sort(P.begin(), P.end(), [](const Pt &a, const Pt &b) {
		if (a.x != b.x) return a.x < b.x;
		return a.y < b.y;
	});
	vector<Pt> up, dn, hull;
	for (int i = 0; i < 4; ++i) {
		while (up.size() > 1 &&
			ccw(up[up.size() - 2], up.back(), P[i]) >= 0) up.pop_back();
		while (dn.size() > 1 &&
			ccw(dn[dn.size() - 2], dn.back(), P[i]) <= 0) dn.pop_back();
		up.push_back(P[i]);
		dn.push_back(P[i]);
	}
	hull = up;
	for (int i = (int)dn.size() - 2; i > 0; --i)
		hull.push_back(dn[i]);
	reverse(hull.begin(), hull.end());
}
int main() {
}


