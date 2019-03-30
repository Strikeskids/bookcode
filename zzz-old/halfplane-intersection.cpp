/* [halfplane-intersection.cpp] */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
const ld EPS = 1e-12;

bool eq(ld a, ld b) {
	return abs(a - b) < EPS;
}
struct Pt {
	ld x, y;
	Pt() {}
	Pt(ld x, ld y) : x(x), y(y) {}
	Pt(const Pt &p) : x(p.x), y(p.y) {}
	Pt operator+(const Pt &p) const { return {x + p.x, y + p.y}; }
	Pt operator-(const Pt &p) const { return {x - p.x, y - p.y}; }
	Pt operator*(const ld c) const { return {x * c, y * c}; }
	Pt operator/(const ld c) const { return {x / c, y / c}; }
	bool operator==(const Pt &p) const { return eq(x, p.x) && eq(y, p.y); }
	bool operator!=(const Pt &p) const { return !(*this == p); }
	bool operator<(const Pt &p) const { return eq(x, p.x) ? y < p.y - EPS : x < p.x; }
	Pt rot() { return {-y, x}; }
	ld abs() const { return hypotl(x, y); }
} P[1005];

ld dot(Pt p, Pt q) {
	return p.x * q.x + p.y * q.y;
}
ld cross(Pt p, Pt q) {
	return p.x * q.y - p.y * q.x;
}
// ax + by + c = 0
// (a,b) != (0,0)
// norm(a,b) = 1 for precision issues
struct Line {
	ld a, b, c;
	int id;
	Line() {}
	Line(ld _a, ld _b, ld _c) : a(_a), b(_b), c(_c) {
		ld d = Pt{a, b}.abs();
		a /= d;
		b /= d;
		c /= d;
	}
	// Directed line from p1 to p2; (a,b) pointing ccw
	Line(Pt p1, Pt p2) {
		Pt n = (p2 - p1).rot();
		n = n / n.abs();
		a = n.x;
		b = n.y;
		c = -dot(n, p1);
	}
	// Returns if (a,b) > (0,0)
	bool right() const {
		return a > EPS || (eq(a, 0) && b > EPS);
	}
	ld signedDist(Pt p) {
		return dot(p, {a, b}) + c;
	}
};
// > 0 if intersection between l1,l2 is on right side of l3
ld det3x3(Line &l1, Line &l2, Line &l3) {
	return l1.a * (l2.b * l3.c - l2.c * l3.b) +
		l1.b * (l2.c * l3.a - l2.a * l3.c) +
		l1.c * (l2.a * l3.b - l2.b * l3.a);
}
Pt linesIntersection(Line l1, Line l2) {
	ld D = l1.a * l2.b - l1.b * l2.a;
	if (eq(D, 0)) {
		if (eq(l1.c, l2.c)) {
			// equal lines
		} else {
			// no intersection
		}
		return {0, 0};
	}
	ld dx = -l1.c * l2.b + l1.b * l2.c;
	ld dy = -l1.a * l2.c + l1.c * l2.a;
	Pt res{dx / D, dy / D};
	return res;
}
// (a,b) of lines point towards desired halfplane; i.e. ax+by+c >= 0
// If solution is bounded (or not exist), then we don't need a bounding box
// Else: Add a bounding box to make sure consecutive angles are < 180 and bounded
// Result: vertices are distinct, ccw order
vector<Pt> halfplanesIntersection(vector<Line> lines) {
	// Sort by angle of normal, starting from upper y-axis (exclusive) anticlockwise.
	// For parallel lines, sort by smallest halfplane first
	sort(lines.begin(), lines.end(), [](const Line &a, const Line &b) {
		bool ar = a.right(), br = b.right();
		if (ar ^ br) return ar;
		ld prod = cross({a.a, a.b}, {b.a, b.b});
		if (!eq(prod, 0)) return prod > 0;
		return a.c < b.c;
	});
	vector<Line> lines2;
	Pt pr;
	for (int i = 0; i < lines.size(); ++i) {
		Pt cur{lines[i].a, lines[i].b};
		if (!i || cur != pr)
			lines2.push_back(lines[i]);
		pr = cur;
	}
	// Ensure all (a,b) are distinct
	lines = lines2;
	int n = lines.size();
	for (int i = 0; i < n; ++i) lines[i].id = i;
	vector<Line> hull;
	// Invariant: last 2 in hull is always < 180 deg, unless no soln
	for (int i = 0; i < 2 * n; ++i) {
		Line l = lines[i % n];
		while (hull.size() >= 2) {
			// If intersection of top 2 halfplanes is wrong side of l, pop
			ld D = det3x3(hull[hull.size() - 2], hull.back(), l);
			if (D > -EPS) break;
			hull.pop_back();
		}
		hull.push_back(l);
	}
	vector<int> firstTime(n, -1);
	vector<Line> v;
	for (int i = 0; i < hull.size(); ++i) {
		int cid = hull[i].id;
		if (firstTime[cid] == -1) {
			firstTime[cid] = i;
			continue;
		}
		for (int j = firstTime[cid]; j < i; ++j)
			v.push_back(hull[j]);
		break;
	}
	// There must be a repeat, unless there is no solution
	n = v.size();
	if (!n) return {};
	v.push_back(v[0]);
	vector<Pt> res;
	Pt center{0, 0};
	for (int i = 0; i < n; ++i) {
		res.push_back(linesIntersection(v[i], v[i + 1]));
		center = center + res.back();
	}
	// This check is only necessary if there can be no solution
	center = center / n;
	for (auto l : lines)
		if (l.signedDist(center) < -EPS) return {};
	// This is to make the result have unique points
	res.resize(unique(res.begin(), res.end()) - res.begin());
	if (res.back() == res.front()) res.pop_back();
	return res;
}
int main() {
	const ld X = 100, Y = 100;
	vector<Line> lines;
	lines.push_back(Line({0, 0}, {X, 0}));
	lines.push_back(Line({0, Y}, {0, 0}));
	lines.push_back(Line({X, 0}, {X, Y}));
	lines.push_back(Line({X, Y}, {0, Y}));
	auto ans = halfplanesIntersection(lines);
}

