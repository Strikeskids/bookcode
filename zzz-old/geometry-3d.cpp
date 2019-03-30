/* [geometry-3d.cpp] */
/* Computes faces of 3D polyhedron from extreme points and finds centroid
 * See http://wwwf.imperial.ac.uk/~rn/centroid.pdf */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
const ld EPS = 1e-12;
bool eq(ld a, ld b) {
	return abs(a - b) < EPS;
}
struct Pt3 {
	ld x, y, z;
	Pt3 operator+(const Pt3 &v) const {
		return {x + v.x, y + v.y, z + v.z};
	}
	Pt3 operator-(const Pt3 &v) const {
		return {x - v.x, y - v.y, z - v.z};
	}
	Pt3 operator-() const {
		return {-x, -y, -z};
	}
	Pt3 operator/(const ld a) const {
		return {x / a, y / a, z / a};
	}
	ld dot(const Pt3 &v) const {
		return x * v.x + y * v.y + z * v.z;
	}
	Pt3 cross(const Pt3 &v) const {
		return {y * v.z - z * v.y,
			z * v.x - x * v.z,
			x * v.y - y * v.x};
	}
	bool operator==(const Pt3 &p) const { return eq(x, p.x) && eq(y, p.y) && eq(z, p.z); }
	bool operator!=(const Pt3 &p) const { return !(*this == p); }
	ld abs() { return sqrt(dot(*this)); }
} A[66];
ld sq(ld a) {
	return a * a;
}
struct Face {
	/* Half plane {x : c^T norm <= disc} */
	Pt3 norm;
	ld disc;
	vector<Pt3> P;
	ld angleBetween(Pt3 u, Pt3 v) {
		Pt3 w = norm.cross(u);
		ld sin = w.dot(v) / w.abs() / v.abs();
		ld cos = u.dot(v) / u.abs() / v.abs();
		return atan2(sin, cos);
	}
};
ld solve() {
	int N;
	scanf("%d", &N);
	for (int i = 0; i < N; ++i)
		scanf("%Lf%Lf%Lf", &A[i].x, &A[i].y, &A[i].z);
	vector<Face> faces;
	// Find all faces from extreme points
	for (int i = 0; i < N; ++i)
		for (int j = i + 1; j < N; ++j)
			for (int k = j + 1; k < N; ++k) {
				Face f;
				f.norm = (A[j] - A[i]).cross(A[k] - A[i]);
				f.disc = f.norm.dot(A[i]);
				bool pos = false, neg = false;
				for (int l = 0; l < N; ++l) {
					ld v = f.norm.dot(A[l]);
					if (eq(v, f.disc)) f.P.push_back(A[l]);
					else if (v > f.disc) pos = true;
					else neg = true;
				}
				if (pos && neg) continue;
				if (f.P[2] != A[k]) continue;
				if (pos) {
					f.norm = -f.norm;
					f.disc = -f.disc;
				}
				faces.push_back(f);
			}
	ld vol = 0;
	Pt3 centroid{0, 0, 0};
	for (Face &f : faces) {
		// Get all triangles in face, by finding point in middle and sorting around it
		Pt3 avg{0, 0, 0};
		for (Pt3 p : f.P) avg = avg + p;
		int len = f.P.size();
		avg = avg / len;
		Pt3 base = f.P[0] - avg;
		sort(f.P.begin(), f.P.end(), [&](const Pt3 &a, const Pt3 &b) {
			return f.angleBetween(base, a - avg) < f.angleBetween(base, b - avg);
		});
		for (int i = 0; i < len; ++i) {
			Pt3 a = avg, b = f.P[i], c = f.P[(i + 1) % len];
			Pt3 norm = (b - a).cross(c - a);
			vol += a.dot(norm) / 6;
			centroid.x += norm.x * (sq((a + b).x) + sq((b + c).x) + sq((c + a).x)) / 48;
			centroid.y += norm.y * (sq((a + b).y) + sq((b + c).y) + sq((c + a).y)) / 48;
			centroid.z += norm.z * (sq((a + b).z) + sq((b + c).z) + sq((c + a).z)) / 48;
		}
	}
	centroid = centroid / vol;
	// Find minimum distance from centroid to a face
	ld dist = 1e100;
	for (Face &f : faces) {
		ld d = f.disc - f.norm.dot(centroid);
		dist = min(dist, d / f.norm.abs());
	}
	return dist;
}
int main() {
	freopen("asteroids.in", "r", stdin);
	freopen("asteroids.out", "w", stdout);
	ld d1 = solve();
	ld d2 = solve();
	printf("%.9Lf\n", d1 + d2);
}


