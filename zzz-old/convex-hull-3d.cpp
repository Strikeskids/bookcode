/* [convex-hull-3d.cpp] */
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;
typedef ll vtype;
const int MAXN = 1005;
/* Basic 3D vector implementation */
struct Pt3 {
	vtype X[3];
	/* 3D cross product */
	Pt3 operator*(const Pt3 &v) const {
		return Pt3{X[1] * v.X[2] - X[2] * v.X[1],
			X[2] * v.X[0] - X[0] * v.X[2],
			X[0] * v.X[1] - X[1] * v.X[0]};
	}
	Pt3 operator-(const Pt3 &v) const {
		return {X[0] - v.X[0], X[1] - v.X[1], X[2] - v.X[2]};
	}
	Pt3 operator-() const {
		return {-X[0], -X[1], -X[2]};
	}
	vtype dot(const Pt3 &v) const {
		return X[0] * v.X[0] + X[1] * v.X[1] + X[2] * v.X[2];
	}
};
Pt3 A[MAXN];
/* E[i][j] indicates which (up to two) other points combine with the edge i and
 * j to make a face in the hull.	Only defined when i < j.
 */
struct Twoset {
	void insert(int x) { (a == -1 ? a : b) = x; }
	bool contains(int x) { return a == x || b == x; }
	void erase(int x) { (a == x ? a : b) = -1; }
	int size() { return (a != -1) + (b != -1); }
	int a, b;
} E[MAXN][MAXN];

struct Face {
	Pt3 norm;
	vtype disc;
	int I[3];
};
/* Compute the half plane {x : c^T norm < disc}
 * defined by the three points A[i], A[j], A[k] where
 * A[inside] is considered to be on the 'interior' side of the face. */
Face makeFace(int i, int j, int k, int inside) {
	E[i][j].insert(k);
	E[i][k].insert(j);
	E[j][k].insert(i);
	Face f;
	f.I[0] = i;
	f.I[1] = j;
	f.I[2] = k;
	f.norm = (A[j] - A[i]) * (A[k] - A[i]);
	f.disc = f.norm.dot(A[i]);
	if (f.norm.dot(A[inside]) > f.disc) {
		f.norm = -f.norm;
		f.disc = -f.disc;
	}
	return f;
}
int main() {
	int N;
	scanf("%d", &N);
	for (int i = 0; i < N; ++i) {
		scanf("%lld%lld%lld", &A[i].X[0], &A[i].X[1], &A[i].X[2]);
	}
	/* Initially construct the hull as containing only the first four points. */
	/* Assert that these 4 points are non-coplanar. */
	Face f;
	vector<Face> faces;
	memset(E, -1, sizeof(E));
	for (int i = 0; i < 4; ++i)
		for (int j = i + 1; j < 4; ++j)
			for (int k = j + 1; k < 4; ++k) {
				faces.push_back(makeFace(i, j, k, 6 - i - j - k));
			}
	/* Now add a point into the hull one at a time. */
	for (int i = 4; i < N; ++i) {
		/* Find and delete all faces with their outside 'illuminated' by this
		 * point. */
		for (int j = 0; j < faces.size(); ++j) {
			f = faces[j];
			if (f.norm.dot(A[i]) > f.disc) {
				E[f.I[0]][f.I[1]].erase(f.I[2]);
				E[f.I[0]][f.I[2]].erase(f.I[1]);
				E[f.I[1]][f.I[2]].erase(f.I[0]);
				faces[j--] = faces.back();
				faces.resize(faces.size() - 1);
			}
		}
		/* Now for any edge still in the hull that is only part of one face
		 * add another face containing the new point and that edge to the hull. */
		int nfaces = faces.size();
		for (int j = 0; j < nfaces; ++j) {
			f = faces[j];
			for (int a = 0; a < 3; ++a)
				for (int b = a + 1; b < 3; ++b) {
					int c = 3 - a - b;
					if (E[f.I[a]][f.I[b]].size() == 2) continue;
					faces.push_back(makeFace(f.I[a], f.I[b], i, f.I[c]));
				}
		}
	}
	/* Answer each of the queries.	Compute the minimum distance of each query
	 * point to each face of the hull. */
	int Q;
	scanf("%d", &Q);
	for (int i = 0; i < Q; ++i) {
		Pt3 v;
		scanf("%lld%lld%lld", &v.X[0], &v.X[1], &v.X[2]);
		ld dist = 1e300;
		for (int i = 0; i < faces.size(); ++i) {
			vtype d = faces[i].disc - faces[i].norm.dot(v);
			dist = min(dist, 1.L * d / sqrt(faces[i].norm.dot(faces[i].norm)));
		}
		printf("%.4Lf\n", dist);
	}
}


