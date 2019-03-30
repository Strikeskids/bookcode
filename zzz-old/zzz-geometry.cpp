/* [zzz-geometry.cpp] */
const ld INF = 1e100;
const ld EPS = 1e-12;
const ld eps = 1e-12;
const ld pi = acosl(-1);
bool eq(ld a, ld b) {
	return abs(a - b) < EPS;
}
int dcmp(ld x) {
	if (fabs(x)<eps) return 0; else return x<0 ? -1 : 1;
}
struct Pt {
	ld x, y;
	Pt() {}
	Pt(ld x, ld y) : x(x), y(y) {}
	Pt(const Pt &p) : x(p.x), y(p.y) {}
	Pt operator-(){ return {-x,-y}; }
	Pt operator+(const Pt &p) const { return {x + p.x, y + p.y}; }
	Pt operator-(const Pt &p) const { return {x - p.x, y - p.y}; }
	Pt operator*(const ld c) const { return {x * c, y * c}; }
	Pt operator/(const ld c) const { return {x / c, y / c}; }
	bool operator==(const Pt &p) const { return eq(x, p.x) && eq(y, p.y); }
	bool operator!=(const Pt &p) const { return !(*this == p); }
	bool operator<(const Pt &p) const { return eq(x, p.x) ? y < p.y - EPS : x < p.x; }
	bool operator>(const Pt &p) const { return !(*this == p || *this < p); }
	Pt rot() { return {-y, x}; }
	ld abs() const { return hypotl(x, y);}
	bool isUpper(){
		return y > 0 || y == 0 && x > 0;}
};
typedef Pt vec;

ld dot(Pt p, Pt q) {return p.x * q.x + p.y * q.y;}
ld cross(Pt p, Pt q) {return p.x * q.y - p.y * q.x;}
ld dist2(Pt p, Pt q) {return dot(p - q, p - q);}
ld dist(Pt p, Pt q) {return sqrt(dist2(p, q));}
ld length(vec a) {return sqrt(dot(a,a));}
ld angle(vec a, vec b) {return acos(dot(a,b)/length(a)/length(b));}
ld angle_aob(Pt a, Pt o, Pt b) {
	vec oa = a-o, ob = b-o; return angle(oa, ob);}
vec rotate(vec a, ld rad) {
	return vec(a.x*cos(rad)-a.y*sin(rad), a.x*sin(rad)+a.y*cos(rad)); }
vec normal(vec a) {ld l = length(a); return vec(-a.y/l, a.x/l);} // perpendicular unit
int ccw(Pt p, Pt q, Pt r) {
	ld v = cross(p - q, p - r);
	if (v < -EPS) return -1;
	if (v > EPS) return 1;
	return 0;
}
ld perimeter(Pt* P, int n) {// last == first
	ld ret = 0;
	for (int i = 0; i < n-1; i++) ret += dist(P[i],P[i+1]);
	return ret; }
//tested
vector<Pt> tangentCirclePoint(Pt c, ld r, Pt p) {
	// r > 0
	ld x = dot(p - c, p - c);
	ld d = x - r * r;
	if (d < -EPS) return {}; // p in circle interior
	Pt q1 = (p - c) * (r * r / x);
	if (d < EPS) return {c + q1};
	Pt q2 = ((p - c) * (-r * sqrt(d) / x)).rot();
	return {c + q1 - q2, c + q1 + q2};
}

// rotate a point CCW or CW around the origin
Pt rotateCCW90(Pt p) { return Pt(-p.y, p.x); }
Pt rotateCW90(Pt p) { return Pt(p.y, -p.x); }
Pt rotateCCW(Pt p, ld t) {
	return Pt(p.x * cos(t) - p.y * sin(t), p.x * sin(t) + p.y * cos(t));
}

// project point c onto line through a and b
// assuming a != b
//tested
Pt projectPointLine(Pt a, Pt b, Pt c) {
	return a + (b - a) * dot(c - a, b - a) / dist2(b, a);
}

// project point c onto line segment through a and b
//tested
Pt projectPointSegment(Pt a, Pt b, Pt c) {
	ld r = dist2(b, a);
	if (abs(r) < EPS) return a;
	r = dot(c - a, b - a) / r;
	if (r < 0) return a;
	if (r > 1) return b;
	return a + (b - a) * r;
}

ld distanceToLine(Pt p, Pt a, Pt b) { // p to line a-b
	vec v1 = b-a, v2 = p-a;
	return fabs(cross(v1,v2)) / length(v1);}
// compute distance from c to segment between a and b
//tested
ld distancePointSegment(Pt a, Pt b, Pt c) {
	return sqrt(dist2(c, projectPointSegment(a, b, c)));
}

// compute distance between point (x,y,z) and plane ax+by+cz=d
ld distancePointPlane(ld x, ld y, ld z,
	ld a, ld b, ld c, ld d) {
	return abs(a * x + b * y + c * z - d) / sqrt(a * a + b * b + c * c);
}
//tested
// determine if lines from a to b and c to d are parallel or collinear
bool linesParallel(Pt a, Pt b, Pt c, Pt d) {
	return abs(cross(b - a, c - d)) < EPS;
}
//tested
bool linesCollinear(Pt a, Pt b, Pt c, Pt d) {
	return linesParallel(a, b, c, d)
	&& abs(cross(a - b, a - c)) < EPS
	&& abs(cross(c - d, c - a)) < EPS;
}
//tested
bool linesOrthogonal(Pt a, Pt b, Pt c, Pt d) {
	return linesParallel(a, b, rotate(c,pi/2), rotate(d,pi/2));
}

// tested
//p+tv intersect q+tw (make sure they do intersect cross(v,w)!=0)
Pt lineIntersection(Pt p, vec v, Pt q, vec w) {
	vec u = p-q; ld t = cross(w,u)/cross(v,w); return p+v*t;}
// tested
bool onSeg(Pt& p, Pt& a, Pt& b) {
	return dcmp(cross(a-p, b-p)) == 0 && dcmp(dot(a-p, b-p)) < 0;}
// tested
// determine if line segment from a to b intersects with
// line segment from c to d
bool segmentsIntersect(Pt a, Pt b, Pt c, Pt d) {
	if (linesCollinear(a, b, c, d)) {
		if (dist2(a, c) < EPS || dist2(a, d) < EPS ||
			dist2(b, c) < EPS || dist2(b, d) < EPS) return true;
		if (dot(c - a, c - b) > 0 && dot(d - a, d - b) > 0 && dot(c - b, d - b) > 0)
			return false;
		return true;
	}
	if (cross(d - a, b - a) * cross(c - a, b - a) > 0) return false;
	if (cross(a - c, d - c) * cross(b - c, d - c) > 0) return false;
	return true;
}
bool segProperIntersection(Pt a1, Pt a2, Pt b1, Pt b2) {
	double c1 = cross(a2-a1,b1-a1), c2 = cross(a2-a1,b2-a1),
	c3 = cross(b2-b1, a1-b1), c4 = cross(b2-b1, a2-b1);
	return dcmp(c1)*dcmp(c2)<0 && dcmp(c3)*dcmp(c4)<0;}
// if c1=0 and c2=0 means collinear (might or might not overlap)
// if c1!= or c2!=0 check whether intersection is at end;

// compute intersection of line passing through a and b
// with line passing through c and d, assuming that unique
// intersection exists; for segment intersection, check if
// segments intersect first
Pt computeLineIntersection(Pt a, Pt b, Pt c, Pt d) {
	b = b - a;
	d = c - d;
	c = c - a;
	assert(dot(b, b) > EPS && dot(d, d) > EPS);
	return a + b * cross(c, d) / cross(b, d);
}

// compute center of circle given three points
Pt computeCircleCenter(Pt a, Pt b, Pt c) {
	b = (a + b) / 2;
	c = (a + c) / 2;
	return computeLineIntersection(b, b + rotateCW90(a - b), c, c + rotateCW90(a - c));
}
// tested
// returns true if we always make the same turn while examining
// all the edges of the polygon one by one
bool isConvex(Pt* P, int n) { // last == same
	if (n <= 3) return false;     // a Pt/sz=2 or a line/sz=3 is not convex
	bool isLeft = ccw(P[0], P[1], P[2])>=0; // remember one result
	for (int i = 1; i < n-1; i++)
		if ((ccw(P[i], P[i+1], P[(i+2) == n ? 1 : i+2])>=0) != isLeft)
			return false;
	return true; }
// tested
// determine if point is in a possibly non-convex polygon (by William
// Randolph Franklin); returns 1 for strictly interior points, 0 for
// strictly exterior points, and 0 or 1 for the remaining points.
// Note that it is possible to convert this into an *exact* test using
// integer arithmetic by taking care of the division appropriately
// (making sure to deal with signs properly) and then by writing exact
// tests for checking point on polygon boundary
bool pointInPolygon(const vector<Pt> &p, Pt q) {
	bool c = 0;
	for (int i = 0; i < p.size(); ++i) {
		int j = (i + 1) % p.size();
		if ((p[i].y <= q.y && q.y < p[j].y ||
			p[j].y <= q.y && q.y < p[i].y) &&
			q.x < p[i].x + (p[j].x - p[i].x) * (q.y - p[i].y) / (p[j].y - p[i].y))
			c = !c;
	}
	return c;
}
// tested
// determine if point is on the boundary of a polygon
bool pointOnPolygon(const vector<Pt> &p, Pt q) {
	for (int i = 0; i < p.size(); ++i)
		if (distancePointSegment(p[i], p[(i + 1) % p.size()], q) < EPS)
			return true;
	return false;
}
// tested
// return "left" of a->b, may return a point or segment
typedef vector<Pt> polygon;
polygon cutPolygon(polygon poly, Pt a, Pt b) {
	polygon ret;
	int n = poly.size();
	for (int i = 0; i < n; i++) {
		Pt c = poly[i], d = poly[(i+1)%n];
		if (dcmp(cross(b-a,c-a)) >= 0) ret.push_back(c);
		if (dcmp(cross(b-a,c-d)) != 0) {
			Pt ip = lineIntersection(a, b-a, c, d-c);
			if (onSeg(ip, c, d)) ret.push_back(ip);
		}
	}
	return ret;
}
// tested, be careful with √eps, it might give a big value
// compute intersection of line through points a and b with
// circle centered at c with radius r > 0
// preserve points order (a -> b)
vector<Pt> circleLineIntersection(Pt a, Pt b, Pt c, ld r) {
	bool atob = a<b;
	vector<Pt> ret;
	b = b - a;
	a = a - c;
	ld A = dot(b, b);
	ld B = dot(a, b);
	ld C = dot(a, a) - r * r;
	ld D = B * B - A * C;
	if (D < -EPS) return ret;
	ret.push_back(c + a + b * (-B + sqrt(D + EPS)) / A);
	if (D > EPS)
		ret.push_back(c + a + b * (-B - sqrt(D)) / A);
	if (ret.size() == 2) {
		if ((atob && ret[1]<ret[0])||(!atob && ret[0]<ret[1])) swap(ret[0],ret[1]);
	}
	return ret;
}
// tested
// compute intersection of circle centered at a with radius r
// with circle centered at b with radius R
vector<Pt> circleCircleIntersection(Pt a, Pt b, ld r, ld R) {
	vector<Pt> ret;
	ld d = dist(a, b);
	if (d > r + R + EPS || d + min(r, R) < max(r, R) - EPS) return ret;
	if (d < EPS) return ret; // Same circle, infinite intersection!
	ld x = (d * d - R * R + r * r) / (2 * d);
	ld y = sqrt(max(0.L, r * r - x * x));
	Pt v = (b - a) / d;
	ret.push_back(a + v * x + v.rot() * y);
	if (y > EPS)
		ret.push_back(a + v * x - v.rot() * y);
	return ret;
}

struct circle {
	Pt c; ld r, x, y;
	circle(Pt c, ld r):c(c), r(r) {x=c.x; y=c.y;}
	Pt point(ld a) { return Pt(c.x+cos(a)*r, c.y+sin(a)*r);}
};
// return tangent line not points
int getTangents(Pt p, circle c, vec* v) {
	vec u = c.c - p;
	ld dist = length(u);
	if (dist < c.r) return 0;
	else if (dcmp(dist-c.r) == 0) { // p on circle, one tangent line
		v[0] = rotate(u, pi/2);
		return 1;
	} else {
		ld ang = asin(c.r / dist);
		v[0] = rotate(u, -ang);
		v[1] = rotate(u, ang);
		return 2;
	}
}
// tested
int getTangents(circle A, circle B, Pt* a, Pt* b) {
	int cnt = 0;
	if (A.r < B.r) {swap(A,B); swap(a,b);}
	int d2 = (A.x-B.x)*(A.x-B.x)+(A.y-B.y)*(A.y-B.y);
	int rdiff = A.r-B.r;
	int rsum = A.r+B.r;
	if (dcmp(d2-rdiff*rdiff) < 0) return 0; // one inside the other
	ld base = atan2(B.y-A.y, B.x-A.x);
	if (dcmp(d2) == 0 && dcmp(A.r-B.r)==0) return -1; // infinity
	if (dcmp(d2-rdiff*rdiff)==0) {   // inside tangent
		a[cnt] = A.point(base); b[cnt] = B.point(base); cnt++;
		return cnt;
	}
	// outside tangent
	ld ang = acos((A.r-B.r) / sqrt(d2));
	a[cnt] = A.point(base+ang); b[cnt] = B.point(base+ang); cnt++;
	a[cnt] = A.point(base-ang); b[cnt] = B.point(base-ang); cnt++;
	if (dcmp(d2-rsum*rsum)==0) { // +1 out tangent
		a[cnt] = A.point(base); b[cnt] = B.point(pi+base); cnt++;
	} else if (dcmp(d2-rsum*rsum)>0) { // +2 out tangent
		ld ang = acos((A.r+B.r) / sqrt(d2));
		a[cnt] = A.point(base+ang); b[cnt] = B.point(pi+base+ang); cnt++;
		a[cnt] = A.point(base-ang); b[cnt] = B.point(pi+base-ang); cnt++;
	}
	return cnt;
}
// tested
ld computeSignedArea(const vector<Pt> &p) {
	ld area = 0;
	for (int i = 0; i < p.size(); ++i) {
		int j = (i + 1) % p.size();
		area += p[i].x * p[j].y - p[j].x * p[i].y;
	}
	return area / 2;
}
// tested
ld computeArea(const vector<Pt> &p) {
	return abs(computeSignedArea(p));
}

// This code computes the area or centroid of a (possibly nonconvex)
// polygon, assuming that the coordinates are listed in a clockwise or
// counterclockwise fashion.	Note that the centroid is often known as
// the "center of gravity" or "center of mass".
Pt computeCentroid(const vector<Pt> &p) {
	Pt c(0, 0);
	ld scale = 6 * computeSignedArea(p);
	for (int i = 0; i < p.size(); ++i) {
		int j = (i + 1) % p.size();
		c = c + (p[i] + p[j]) * (p[i].x * p[j].y - p[j].x * p[i].y);
	}
	return c / scale;
}

// tests whether or not a given polygon (in CW or CCW order) is simple
bool isSimple(const vector<Pt> &p) {
	for (int i = 0; i < p.size(); ++i) {
		for (int k = i + 1; k < p.size(); ++k) {
			int j = (i + 1) % p.size();
			int l = (k + 1) % p.size();
			if (i == l || j == k) continue;
			if (segmentsIntersect(p[i], p[j], p[k], p[l]))
				return false;
		}
	}
	return true;
}

// Get counter-clockwise convex hull, starting at leftmost bottommost point
// Removes all collinear points. If points all collinear, size is 2.
vector<Pt> convexHull(vector<Pt> &P) {
	sort(P.begin(), P.end(), [](const Pt &a, const Pt &b) {
		if (!eq(a.x, b.x)) return a.x < b.x;
		return a.y < b.y;
	});
	vector<Pt> up, dn, hull;
	for (int i = 0; i < P.size(); ++i) {
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
	return hull;
}
// Rujia Liu
// input p ,output ch and m (last != first for input and ouput)
// no Points on edges (change <= to < to keep collinear Pts)
//tested
int convexHull(Pt* p, int n, Pt* ch) {
	sort(p, p+n);
	int m = 0;
	for (int i = 0; i < n; i++) {
		while (m > 1 && cross(ch[m-1]-ch[m-2], p[i]-ch[m-2]) <= 0) m--;
		ch[m++] = p[i];
	}
	int k = m;
	for (int i = n-2; i >= 0; i--) {
		while (m > k && cross(ch[m-1]-ch[m-2], p[i]-ch[m-2]) <= 0) m--;
		ch[m++] = p[i];
	}
	if (n > 1) m--;
	return m;
}
//tested
ld convexDiameter(Pt* Q, int m) {
	int j = 1; ld maxi = 0;
	for (int i = 0; i < m; i++) {
		while (dist(Q[i], Q[j]) < dist(Q[i], Q[(j+1)%m])) j = (j+1)%m;
		maxi = max(dist(Q[i], Q[j]), maxi);}
	return maxi;}
//tested
//sort(p,p+n); first
Pt p[maxn];
int tmpt[maxn];
ld sqr(ld x) {return x*x;}
bool cmpy(const int& a, const int& b) {
	return p[a].y < p[b].y;}
ld closestPair(Pt* p, int l, int r) {
	ld d = inf;
	if (l == r) return d;
	if (l+1 == r) return dist2(p[l], p[r]);
	int mid = (l+r)/2;
	ld d1 = closestPair(p, l, mid);
	ld d2 = closestPair(p, mid + 1, r);
	d = min(d1, d2);
	int i, j, k = 0;
	for (i = l; i <= r; i++) {
		if (sqr(p[mid].x - p[i].x) <= d)
			tmpt[k++] = i;
	}
	sort(tmpt, tmpt + k, cmpy);
	for (i = 0; i < k; i++) {
		for (j = i + 1; j < k && sqr(p[tmpt[j]].y - p[tmpt[i]].y) < d;
			 j++) {
			ld d3 = dist2(p[tmpt[i]], p[tmpt[j]]);
			if (d > d3) d = d3;
		}
	}
	return d;
}
// tested
// signed area of intersection of triange (0,0)(x0,y0)(x1,y1) and circle (0,0,r)
// make sure they don't intersect on (x0,y0)->(x1,y1)
ld calc(Pt a, Pt b, ld r) {
	if (dcmp(length(a)-r) <= 0 && dcmp(length(b)-r) <= 0) {
		return cross(a,b) / 2;
	}
	// fan area
	ld theta = atan2(cross(a, b), dot(a,b));
	return theta*r*r/2;
}
// tested
ld polygonCircleArea(vector<Pt>& p, Pt c, ld r) {
	ld ans = 0;
	int n = p.size();
	for (int i = 0; i < n; i++) {
		auto ret = circleLineIntersection(p[i], p[(i+1)%n], c, r);
		vector<Pt> result;
		for (auto pp : ret) if (onSeg(pp, p[i], p[(i+1)%n])) {
			result.push_back(pp);
		}
		Pt cur = p[i];
		for (auto p : result) {
			ans += calc(cur-c,p-c,r);
			cur = p;
		}
		ans += calc(cur-c, p[(i+1)%n]-c, r);
	}
	return ans;
}
// find intersection between line and polygon in O(logn)
// only for points strictly inside polygon
// input: line(point, angle) and polygon(0...n-1)
// return: intersection segment L,R (L~L+1 and R~R+1)
ii linePolygonIntersection(const Pt& pt, double ang, Pt* a, int n) {
	int rev = 0;
	if (cross(a[0]-pt,vec(cos(ang), sin(ang))) < 0) {
		rev = 1; ang -= pi;
	}
	Pt unit(cos(ang), sin(ang));
	int l = 0, r = n-1, s1,s2;
	while (l <= r) {
		int mid = (l+r)/2;
		if (cross(unit,a[mid]-pt) > 0) r = mid-1;
		else {
			if (cross(a[mid]-pt, a[0]-pt) <=0) {l = mid+1; s1=mid;}
			else r = mid-1;
		}
	}
	r = n-1;
	unit.x = -unit.x; unit.y = -unit.y;
	while (l <= r) {
		int mid = (l+r)/2;
		if (cross(unit,a[mid]-pt) < 0) {l = mid+1; s2 = mid;}
		else r = mid-1;
	}
	if (rev) swap(s1, s2);
	return {s1,s2};
}

// Smallest enclosing circle in expected O(n)
mt19937 rng;
pair<Pt, ld> circumcircle(const vector<Pt> &p, int k) {
	if (!k) return {{0, 0}, 0};
	if (k == 1) return {p[0], 0};
	if (k == 2) return {(p[0] + p[1]) / 2, dist2(p[0], p[1]) / 4};
	if (k == 3) {
		Pt c = computeCircleCenter(p[0], p[1], p[2]);
		return {c, dist2(c, p[0])};
	}
	assert(false);
}
// Returns (center, radius^2) = enclosingCircle(pts, 0, pts.size());
pair<Pt, ld> enclosingCircle(vector<Pt> &p, int known, int length) {
	auto c = circumcircle(p, known);
	for (int i = known; i < length; ++i) {
		int r = i + rng() % (length - i);
		swap(p[i], p[r]);
		if (dist2(p[i], c.first) > c.second + EPS) {
			swap(p[known], p[i]);
			c = enclosingCircle(p, known + 1, i + 1);
		}
	}
	return c;
}
struct CH{
	int n;
	vector<Pt> ps, lower, upper;
	Pt operator[](int i){return ps[i];}
	void init(vector<Pt> _ps){
		ps = _ps; n = ps.size();
		rotate(ps.begin(),min_element(ps.begin(), ps.end()),ps.end());
		int at = max_element(ps.begin(), ps.end()) - ps.begin();
		lower = vector<Pt>(ps.begin(),ps.begin() + at + 1);
		upper = vector<Pt>(ps.begin()+at,ps.end()); upper.pb(ps[0]);
	}
	void update_tangent(Pt p, int id, int&a,int&b){
		if(cross(ps[a]-p,ps[id]-p) > 0) a = id;
		if(cross(ps[b]-p,ps[id]-p) < 0) b = id;
	}
	void binary_search(int l,int r,Pt p,int&a,int&b){
		if(l==r) return;
		update_tangent(p,l%n,a,b);
		int sl = dcmp(cross(ps[l%n]-p,ps[(l+1)%n]-p));
		while(l+1<r){
			int m = l+r>>1;
			if(dcmp(cross(ps[m%n]-p,ps[(m+1)%n]-p)) == sl)
			l=m;
			else
			r=m;
		}
		update_tangent(p,r%n,a,b);
	}
	bool contain(Pt p){
		if(p.x < lower[0].x || p.x > lower.back().x) return 0;
		int id = lower_bound(lower.begin(), lower.end(),(Pt){p.x,-inf}) - lower.begin();
		if(lower[id].x == p.x){
			if(lower[id].y > p.y) return 0;
		} else {
			if(cross(lower[id]-lower[id-1],p-lower[id-1]) < 0) return 0;
		}
		id = lower_bound(upper.begin(), upper.end(),Pt(p.x,inf),greater<Pt>()) - upper.begin();
		if(upper[id].x == p.x){
			if(upper[id].y < p.y) return 0;
		} else {
			if(cross(upper[id]-upper[id-1],p-upper[id-1]) < 0) return 0;
		}
		return 1;}
	bool get_tangent(Pt p,int&a,int&b){ // b->a
		if(contain(p)) return 0;
		a=b=0;
		int id = lower_bound(lower.begin(), lower.end(),p) - lower.begin();
		binary_search(0,id,p,a,b);
		binary_search(id,lower.size(),p,a,b);
		id = lower_bound(upper.begin(), upper.end(),p,greater<Pt>()) - upper.begin();
		binary_search((int)lower.size() - 1, (int) lower.size() - 1 + id,p,a,b);
		binary_search((int) lower.size() - 1 + id,(int) lower.size() - 1 + upper.size(),p,a,b);
		return 1;
	}
}D;



Change Build command to g++ -std=c++14 -O2 -o "%e" "%f" in Geany > Build > Set Build Commands

#include <bits/stdc++.h>
using namespace std;
#define db(x) cerr << #x << "=" << x << endl
#define db2(x, y) cerr << #x << "=" << x << "," << #y << "=" << y << endl
#define db3(x, y, z) cerr << #x << "=" << x << "," << #y << "=" << y << "," << #z << "=" << z << endl
typedef long long ll;
typedef long double ld;

