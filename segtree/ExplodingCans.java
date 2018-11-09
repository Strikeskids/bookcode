import java.io.*;
import java.util.*;

public class ExplodingCans {
	public static void main(String[] args) throws Exception {
		BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
		PrintWriter pw = new PrintWriter(System.out);
		StringTokenizer st;

		int n = nextInt(br);

		Can[] cans = new Can[n];
		Can[] order = new Can[n];

		for (int i = 0; i < n; i++) {
			st = getst(br);
			cans[i] = new Can(i, nextInt(st), nextInt(st));
			order[i] = cans[i];
		}

		Arrays.sort(cans);

		for (int i = 0; i < n; i++) {
			cans[i].id  = i;
			cans[i].minReach = bslow(cans, cans[i].x - cans[i].r);
			cans[i].maxReach = bslow(cans, cans[i].x + cans[i].r);

			if (cans[cans[i].maxReach].x > cans[i].r + cans[i].x) {
				cans[i].maxReach--;
			}
		}

		Pair[] cur = new Pair[n];

		for (int i = 0; i < n; i++) {
			cur[i] = new Pair(cans[i].minReach, cans[i].maxReach + 1);
		}

		// System.err.println(Arrays.toString(cur));

		while (true) {
			Pair[] next = new Pair[n];
			Segtree<Pair> seg = new Segtree<Pair>(cur, new Pair[2*n]);
			boolean changed = false;

			for (int i = 0; i < n; i++) {
				next[i] = seg.query(cur[i].a, cur[i].b, new Pair(1000000, -1));

				// System.err.println(String.format("%s --> %s", cur[i], next[i]));

				if (next[i].a != cur[i].a || next[i].b != cur[i].b) {
					changed = true;
				}
			}

			// System.err.println(Arrays.toString(next));

			cur = next;

			if (!changed) {
				break;
			}
		}

		for (int i = 0; i < n; i++) {
			if (i > 0) {
				pw.print(" ");
			}
			pw.print(cur[order[i].id].b - cur[order[i].id].a);
		}

		pw.println();

		br.close();
		pw.close();
	}

	public static int bslow(Can[] arr, int x) {
		int lo = 0;
		int hi = arr.length - 1;

		while (lo < hi) {
			int mid = (lo + hi) / 2;

			if (arr[mid].x >= x) {
				hi = mid;
			} else {
				lo = mid + 1;
			}
		}

		return lo;
	}

	interface Queryable<T> {
		public abstract T merge(T obj);
	}

	static class Segtree<T extends Queryable<T>> {
		T[] arr;
		int n;

		public Segtree(T[] values, T[] arr) {
			n = values.length;
			this.arr = arr;

			for (int i = 0; i < n; i++) {
				arr[i + n] = values[i];
			}

			for (int i = n - 1; i > 0; i--) {
				arr[i] = arr[2*i].merge(arr[2*i+1]);
			}
		}

		public T query(int l, int r, T min) {
			T ret = min;

			l += n;
			r += n;

			while (l < r) {
				if ((l & 1) > 0) {
					ret = ret.merge(arr[l]);
					l++;
				}

				if ((r & 1) > 0) {
					r--;
					ret = ret.merge(arr[r]);
				}

				l >>= 1;
				r >>= 1;
			}

			return ret;
		}
	}

	static class Pair implements Queryable<Pair> {
		int a;
		int b;

		public Pair(int ma, int mb) {
			a = ma;
			b = mb;
		}

		public Pair merge(Pair o) {
			return new Pair(Math.min(a, o.a), Math.max(b, o.b));
		}

		public String toString() {
			return String.format("(%d, %d)", a, b);
		}
	}

	static class Can implements Comparable<Can> {
		int x;
		int r;
		int minReach;
		int maxReach;
		int id;

		public Can(int i, int mx, int mr) {
			x = mx;
			r = mr;
			id = i;
		}

		public int compareTo(Can o) {
			return x - o.x;
		}

		public String toString() {
			return String.format("(%d, %d)", x, r);
		}
	}

	////////////////////////////////////////////////

	public static StringTokenizer getst(BufferedReader br) throws Exception {
		return new StringTokenizer(br.readLine(), " ");
	}

	public static int nextInt(BufferedReader br) throws Exception {
		return Integer.parseInt(br.readLine());
	}

	public static int nextInt(StringTokenizer st) throws Exception {
		return Integer.parseInt(st.nextToken());
	}
}