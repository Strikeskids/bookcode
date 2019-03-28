import java.util.*;

////////////////////////////
///////// GEOMETRY /////////
/// STOLEN BACK FROM MIT ///
////////////////////////////

public class Geometry {
	public static final double pi = Math.PI;
	public static final double eps = 1e-9;
	public static boolean eq(double a, double b) { return Math.abs(a-b) < eps; }
	public static double sq(double d) { return d*d; }
	public static double posmod(double a, double b) { return ((a % b) + b) % b; }
	public static double normang(double t) { return posmod(t, 2*pi); }
	public static double angdiff(double t1, double t2) { return normang(t1 - t2 + pi) - pi; }
	public static double hypot(double x, double y){ return Math.sqrt(x*x + y*y); }

	static class Point implements Comparable<Point> {
		double x;
		double y;

		public Point(double mx, double my) {
			x = mx;
			y = my;
		}

		public static Point polar(double r, double theta) {
			return new Point(r*Math.cos(theta), r*Math.sin(theta));
		}

		public Point add(Point p) { return new Point(x+p.x, y+p.y); }
		public Point sub(Point p) { return new Point(x-p.x, y-p.y); }
		public Point mult(double d) { return new Point(x*d, y*d); }
		public double dot(Point p) { return x*p.x + y*p.y; }
		public double cross(Point p) { return x*p.y - y*p.x; }
		public double len() { return hypot(x, y); }
		public Point scale(double d) { return mult(d / len()); }
		public double dist(Point p) { return sub(p).len(); }
		public double ang() { return Math.atan2(y, x); }
		public Point rot(double theta) { return new Point(x*Math.cos(theta) - y*Math.sin(theta), x*Math.sin(theta) + y*Math.cos(theta)); }
		public Point perp() { return new Point(-y, x); }
		public boolean equals(Point o) { return Math.abs(dist(o)) < eps; }
		public double norm() { return dot(this); }
		public String toString(){ return String.format("(%.6f, %.6f)", x, y); }

		public int compareTo(Point o) { // sort by x, then by y
			if (eq(x, o.x)) {
				if (eq(y, o.y)) {
					return 0;
				}

				return y > o.y ? 1 : -1;
			}

			return x > o.x ? 1 : -1;
		}
	}

	static class Line {
		Point a;
		Point b;
		Point along; // used only in upperHull

		public Line(Point ma, Point mb) {
			a = ma;
			b = mb;
		}

		public String toString(){ return String.format("{%s -> %s}", a.toString(), b.toString()); }
	}

	public static double ccw(Point p1, Point p2, Point p3) { // left is 1, right is -1
		return p2.sub(p1).cross(p3.sub(p1));
	}

	public static Point lineline(Line a, Line b) {
		double d = a.b.sub(a.a).cross(b.b.sub(b.a));

		if (eq(d, 0)) {
			return null;
		}

		return a.a.add((a.b.sub(a.a)).mult(b.b.sub(b.a).cross(a.a.sub(b.a))/d));
	}

	public static boolean online(Line l, Point p) {
		return eq(ccw(l.a, l.b, p), 0);
	}

	public static boolean onseg(Line l, Point p) {
		Point delta = l.b.sub(l.a);
		return online(l, p) && delta.dot(l.a) - eps <= delta.dot(p) && delta.dot(p) <= delta.dot(l.b) + eps;
	}

	public static Point segline(Line a, Line b) {
		Point inter = lineline(a, b);

		if (inter == null || !onseg(a, inter)) {
			return null;
		}

		return inter;
	}

	public static Point segseg(Line a, Line b) {
		Point inter = segline(a, b);

		if (inter == null || !onseg(b, inter)) {
			return null;
		}

		return inter;
	}

	public static Point pointline(Point p, Line l) {
		Point v = l.b.sub(l.a).scale(1);
		double dot = p.sub(l.a).dot(v);
		return l.a.add(v.mult(dot));
	}

	public static Point pointseg(Point p, Line l) {
		Point v = l.b.sub(l.a).scale(1);
		double dot = p.sub(l.a).dot(v);
		dot = Math.max(dot, 0);
		dot = Math.min(dot, l.b.dist(l.a));
		return l.a.add(v.mult(dot));
	}

	public static double pointsegdist(Point p, Line l) {
		return pointseg(p, l).dist(p);
	}

	public static double pointlinedist(Point p, Line l) {
		return pointline(p, l).dist(p);
	}

	public static double polyarea(Point[] poly) {
		double area = 0;
		for (int i = 0; i < poly.length; i++) {
			area += poly[i].cross(poly[(i+1)%poly.length]);
		}
		return Math.abs(area/2);
	}

	public static int pointinpoly(Point p, Point[] poly) { // -1 outside, 0 on, 1 inside
		double ang = 0;

		for (int i = 0; i < poly.length; i++) {
			Point a = poly[i];
			Point b = poly[(i+1)%poly.length];

			if (onseg(new Line(a, b), p)) {
				return 0;
			}

			ang += angdiff(a.sub(p).ang(), b.sub(p).ang());
		}

		return eq(ang, 0) ? -1 : 1;
	}

	static class Circle {
		Point p;
		double r;

		public Circle(Point mp, double mr) {
			p = mp;
			r = mr;
		}

		public Circle(Point a, Point b, Point c) {
			Point p1 = a.add(b).mult(.5);
			Line l1 = new Line(p1, p1.add(a.sub(b).perp()));
			Point p2 = b.add(c).mult(.5);
			Line l2 = new Line(p2, p2.add(b.sub(c).perp()));

			p = lineline(l1, l2);
			r = p.dist(a);
		}
	}

	// Convex hull - may have issues with common x-value.  Rotate by rand if unsure.

	public Point[] convexHull(Point[] pts) {
		Point v0 = null;
		Arrays.sort(pts);
		ArrayList<Point> ch = new ArrayList<Point>();

		for (Point p : pts) {
			if (ch.size() > 0 && eq(ch.get(ch.size() - 1).x, p.x)) {
				if (p.y > ch.get(ch.size() - 1).y) {
					ch.remove(ch.size() - 1);
				} else {
					continue;
				}
			}

			while (ch.size() >= 2 && ccw(ch.get(ch.size() - 2), ch.get(ch.size() - 1), p) > -eps) {
				ch.remove(ch.size() - 1);
			}

			ch.add(p);
		}

		// ch contains the upper hull here

		ArrayList<Point> lh = new ArrayList<Point>();

		for (Point p : pts) {
			if (lh.size() > 0 && eq(lh.get(lh.size() - 1).x, p.x)) {
				if (p.y < lh.get(lh.size() - 1).y) {
					lh.remove(lh.size() - 1);
				} else {
					continue;
				}
			}

			while (lh.size() >= 2 && ccw(lh.get(lh.size() - 2), lh.get(lh.size() - 1), p) < eps) {
				lh.remove(lh.size() - 1);
			}

			lh.add(p);
		}

		// lh contains the lower hull here
		Collections.reverse(lh);

		for (Point p : lh) {
			if (ch.get(0).equals(p) || ch.get(ch.size()-1).equals(p)) {
				continue;
			}

			ch.add(p);
		}

		return ch.toArray(new Point[0]);
	}

	// Upper hull of a set of lines.  Might have degeneracy issues.

	public static Point origin = new Point(0, 0);

	static class UpperHullComparator implements Comparator<Line> {
		public int compare(Line a, Line b) {
			double hit = ccw(origin, a.along, b.along);

			if (!eq(hit, 0)) {
				if (hit > 0) {
					return -1;
				} else {
					return 1;
				}
			}

			if (eq(ccw(a.a, a.b, b.a), 0)) {
				return 0;
			} else if (ccw (a.a, a.b, b.a) > 0) {
				return -1;
			} else {
				return 1;
			}
		}
	}

	Line[] upperHull(Line[] lines) {
		for (Line l : lines) {
			l.along = l.b.sub(l.a);
		}

		Arrays.sort(lines, new UpperHullComparator());
		ArrayList<Line> ch = new ArrayList<Line>();

		for (Line l : lines) {
			while (ch.size() > 0) {
				Point inter = lineline(ch.get(ch.size() - 1), l);

				if (inter == null) {
					ch.remove(ch.size() - 1);
				} else if (ch.size() > 1) {
					Line a = ch.get(ch.size() - 2);

					if (ccw(a.a, a.b, inter) < eps) {
						ch.remove(ch.size() - 1);
					} else {
						break;
					}
				} else {
					break;
				}
			}

			ch.add(l);
		}

		return ch.toArray(new Line[0]);
	}

	public static Point[] circline(Circle c, Line l) {
		Point x = pointline(c.p, l);
		double d = x.dist(c.p);

		if (d > c.r + eps) {
			return new Point[0];
		}

		double h = Math.sqrt(Math.max(0, sq(c.r) - sq(d)));
		Point perp = l.a.sub(l.b);

		Point[] ans = new Point[2];

		ans[0] = x.add(perp.scale(h));
		ans[1] = x.add(perp.scale(-h));

		return ans;
	}

	public static Point[] circcirc(Circle a, Circle b) {
		double d = a.p.dist(b.p);

		if (d > a.r + b.r + eps || d < Math.abs(a.r - b.r) - eps) {
			return new Point[0];
		}

		double x = (sq(d) - sq(b.r) + sq(a.r)) / (2*d);
		double y = Math.sqrt(Math.max(0, sq(a.r) - sq(x)));
		Point v = b.p.sub(a.p);

		Point[] ans = new Point[2];

		ans[0] = a.p.add(v.scale(x)).add(v.perp().scale(y));
		ans[1] = a.p.add(v.scale(x)).add(v.perp().scale(-y));

		return ans;
	}

	public static Line[] circcirctan(Circle a, Circle b) {
		if(a.r < b.r) {
			return circcirctan(b, a);
		}

		Line[] res = new Line[4];

		double d = a.p.dist(b.p);
		double d1 = a.r * d / (a.r + b.r);
		double t = Math.acos(a.r/d1);
		Point v = b.p.sub(a.p);

		// crossing
		res[0] = new Line(a.p.add(v.scale(a.r).rot(t)), b.p.add(v.scale(-b.r).rot(t)));
		res[1] = new Line(a.p.add(v.scale(a.r).rot(-t)), b.p.add(v.scale(-b.r).rot(-t)));

		t = Math.asin((a.r-b.r)/d) + pi/2;
		v = a.p.sub(b.p);

		// same side
		res[2] = new Line(a.p.add(v.scale(a.r).rot(t)), b.p.add(v.scale(b.r).rot(t)));
		res[3] = new Line(a.p.add(v.scale(a.r).rot(-t)), b.p.add(v.scale(b.r).rot(-t)));

		return res;
	}

	static class Point3 {
		double x;
		double y;
		double z;

		public Point3(double mx, double my, double mz) {
			x = mx;
			y = my;
			z = mz;
		}

		public static Point3 spherical(double r, double theta, double phi) {
			return new Point3(r * Math.cos(theta) * Math.sin(phi), r * Math.sin(theta) * Math.sin(phi), r * Math.cos(phi));
		}

		public double[] ang() {
			double[] ans = new double[2];

			ans[0] = Math.atan(y/x);
			ans[1] = Math.acos(z/len());

			return ans;
		}

		public Point proj() { return new Point(x, y); }
		public Point3 add(Point3 p) { return new Point3(x+p.x, y+p.y, z+p.z); }
		public Point3 sub(Point3 p) { return new Point3(x-p.x, y-p.y, z-p.z); }
		public double dot(Point3 p) { return x*p.x + y*p.y + z*p.z; }
		public Point3 cross(Point3 p) { return new Point3(y*p.z - p.y*z, z*p.x - p.z*x, x*p.y - p.x*y); }
		public Point3 mult(double d) { return new Point3(x*d, y*d, z*d); }
		public double len() { return Math.sqrt(x*x + y*y + z*z); }
		public Point3 scale(double d) { return mult(d / len()); }
		public double dist(Point3 p) { return sub(p).len(); }
		public boolean equals(Point3 p) { return eq(dist(p), 0); }
		public String toString(){ return String.format("(%.6f, %.6f, %.6f)", x, y, z); }

		public Point3[] getBasis(Point3 z) {
			Point3[] basis = new Point3[3];

			z = z.scale(1);
			Point3 y = z.cross(new Point3(1, 0, 0));

			if (eq(y.len(), 0)) {
				y = z.cross(new Point3(0, 1, 0));
			}

			Point3 x = y.cross(z);

			y = y.scale(1);
			x = x.scale(1);

			basis[0] = x;
			basis[1] = y;
			basis[2] = z;

			return basis;
		}

		public Point3 trans(Point3[] basis, Point3 p) {
			return new Point3(basis[0].dot(p), basis[1].dot(p), basis[2].dot(p));
		}

		public Point3 invtrans(Point3[] basis, Point3 p) {
			return basis[0].mult(p.x).add(basis[1].mult(p.y)).add(basis[2].mult(p.z));
		}
	}

	static class Point3Comparator implements Comparator<Point3> {
		static Point3Comparator[] inst;
		int axis;

		public static Point3Comparator axis(int a) {
			if (inst == null) {
				inst = new Point3Comparator[3];
				inst[0] = new Point3Comparator(0);
				inst[1] = new Point3Comparator(1);
				inst[2] = new Point3Comparator(2);
			}

			return inst[a];
		}

		public Point3Comparator(int a) {
			axis = a;
		}

		public int compare(Point3 a, Point3 b) {
			if (a.equals(b)) {
				return 0;
			}

			if (axis == 0) {
				return Double.compare(a.x, b.x);
			}

			if (axis == 1) {
				return Double.compare(a.y, b.y);
			}

			if (axis == 2) {
				return Double.compare(a.z, b.z);
			}

			return Point3Comparator.axis((axis+1)%3).compare(a, b);
		}
	}

	static class KD3 {
		Point3 root;
		int axis;
		KD3 left;
		KD3 right;

		public KD3(Point3[] points) {
			this(points, 0, points.length, 0);
		}

		private KD3(Point3[] points, int l, int r, int a) {
			axis = a;

			if (points.length == 1) {
				root = points[0];
				return;
			}

			Arrays.sort(points, l, r, Point3Comparator.axis(axis));
			int ind = (r+l)/2;

			root = points[ind];

			if (l < ind) {
				left = new KD3(points, l, ind, (a+1)%3);
			}

			if (r > ind+1) {
				right = new KD3(points, ind+1, r, (a+1)%3);
			}
		}

		private KD3(Point3 p, int a) {
			root = p;
			axis = a;
		}

		public KD3 insert(Point3 p) {
			int c = Point3Comparator.axis(axis).compare(p, root);

			if (c < 0) {
				if (left == null) {
					left = new KD3(p, (axis+1)%3);
				} else {
					left.insert(p);
				}
			} else if (c > 0) {
				if (right == null) {
					right = new KD3(p, (axis+1)%3);
				} else {
					right.insert(p);
				}
			}

			return this;
		}

		public Point3 nearest(Point3 p) {
			return nearest(p, null);
		}

		public Point3 nearest(Point3 p, Point3 best) {
			int c = Point3Comparator.axis(axis).compare(p, root);

			if (best == null || p.dist(best) > p.dist(root)) {
				best = root;
			}

			if (c < 0) {
				if (left != null) {
					Point3 bl = left.nearest(p);

					if (p.dist(bl) < p.dist(best)) {
						best = bl;
					}
				}
			} else if (c > 0) {
				if (right != null) {
					Point3 br = right.nearest(p);

					if (p.dist(br) < p.dist(best)) {
						best = br;
					}
				}
			}

			if (Math.abs(getAxis(root, axis) - getAxis(p, axis)) < p.dist(best)) {
				if (c < 0) {
					if (right != null) {
						Point3 br = right.nearest(p);

						if (p.dist(br) < p.dist(best)) {
							best = br;
						}
					}
				} else if (c > 0) {
					if (left != null) {
						Point3 bl = left.nearest(p);

						if (p.dist(bl) < p.dist(best)) {
							best = bl;
						}
					}
				}
			}

			return best;
		}

		public double getAxis(Point3 p, int axis) {
			switch(axis) {
				case 0:
					return p.x;
				case 1:
					return p.y;
				case 2:
					return p.z;
				default:
					return 0;
			}
		}

		public String toString() {
			return String.format("{%s, %s, %s}", root.toString(), (left == null ? "--" : left.toString()), (right == null) ? "--" : right.toString());
		}
	}
}