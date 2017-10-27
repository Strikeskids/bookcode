import java.io.BufferedReader;
import java.io.Closeable;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.util.BitSet;
import java.util.StringTokenizer;

public class C2 {

	static final int NHEX = 7;
	static final int HEX = 6;

	static final int HEAPS = 1;
	static final int SEQUENTIAL = 2;
	static final int HEAPS_FWD = 3;
	static final int ALGORITHM = SEQUENTIAL;

	static int[][] hexes;
	static int[][][] adj;

	public static void main(String... args) throws Exception {
		FastScan sc = new FastScan(new BufferedReader(new InputStreamReader(System.in)));
		PrintWriter pw = new PrintWriter(System.out);

		BitSet set = new BitSet();
		set.set(0, NHEX);
		hexes = new int[NHEX][HEX];
		adj = new int[NHEX][HEX][2];

		outer: for (int TC = 1, TCC = sc.in(); TC <= TCC; ++TC) {
			for (int i = 0; i < NHEX; ++i) {
				for (int j = 0; j < HEX; ++j) {
					hexes[i][j] = sc.in() - 1;
				}
			}

			for (int i = 0; i < NHEX; ++i) {
				for (int j = 0; j < HEX; ++j) {
					adj[i][hexes[i][j]][0] = hexes[i][(j + HEX - 1) % HEX];
					adj[i][hexes[i][j]][1] = hexes[i][(j + 1) % HEX];
				}
			}

			int[] perm = new int[NHEX - 1];

			for (int i = 0; i < NHEX; ++i) {
				int base = i;
				int offset = 0;
				while (hexes[base][offset] != 0) {
					offset++;
				}
				// set.clear(i);
				// int[] idxso = soln(new int[NHEX - 1], 0, base, offset, set);
				// set.set(i);

				int[] idxs;

				for (int j = 0; j < NHEX - 1; ++j) {
					perm[j] = j < i ? j : j + 1;
				}

				if (ALGORITHM == HEAPS) {
					idxs = heaps(perm, base, offset, NHEX - 1);
				} else if (ALGORITHM == SEQUENTIAL) {
					idxs = lex(perm, base, offset);
				} else if (ALGORITHM == HEAPS_FWD) {
					idxs = heaps_fwd(perm, base, offset, 0);
				}

				if (idxs != null) {
					pw.printf("Case %d: %d %d %d %d %d %d %d\n", TC, base, idxs[0], idxs[1], idxs[2], idxs[3], idxs[4],
							idxs[5]);
					continue outer;
				}
			}
			pw.printf("Case %d: No solution\n", TC);
		}

		sc.close();
		pw.close();
		System.exit(0);
	}

	static int[] heaps(int[] arr, int base, int offset, int N) {
		if (N != 1) {
			// INDUCTIVE CASE
			int[] result = heaps(arr, base, offset, N - 1);
			if (result != null)
				return result;

			for (int i = 0; i < N - 1; ++i) {
				if (N % 2 == 0)
					swap(arr, i, N - 1);
				else
					swap(arr, 0, N - 1);

				result = heaps(arr, base, offset, N - 1);
				if (result != null)
					return result;
			}
		} else {
			// BASE CASE: have a permutation in arr
			if (check(arr, 0, base, offset) >= HEX) {
				return arr;
			}
		}
		return null;
	}

	static void heaps_skip(int[] arr, int N) {
		// Simulate a step of heap's algorithm without going through
		// all the permutations
		if (N == 0) {
		} else if (N % 2 == 1) {
			swap(arr, 0, N - 1);
		} else if (N == 2) {
			swap(arr, 0, 1);
		} else {
			int first = arr[N - 3];
			for (int targ = N - 3; targ > 1; --targ) {
				arr[targ] = arr[targ - 1];
			}
			arr[1] = arr[N - 2];
			arr[N - 2] = arr[N - 1];
			arr[N - 1] = arr[0];
			arr[0] = first;
		}
	}

	static void heaps_fwd_skip(int[] arr, int idx) {
		// Simulate a step of heap's algorithm in reverse without going through
		// all the permutations
		int len = arr.length - idx;
		if (len == 0) {
		} else if (len % 2 == 1) {
			// Simulate an odd pass
			swap(arr, idx, arr.length - 1);
		} else if (len == 2) {
			// Simulate an even pass when len = 2
			swap(arr, idx, arr.length - 1);
		} else {
			// Simulate an even pass
			int last = arr[idx + 2];
			for (int targ = idx + 2; targ < arr.length - 2; ++targ) {
				arr[targ] = arr[targ + 1];
			}
			arr[arr.length - 2] = arr[idx + 1];
			arr[idx + 1] = arr[idx];
			arr[idx] = arr[arr.length - 1];
			arr[arr.length - 1] = last;
		}
	}

	static int[] heaps_fwd(int[] arr, int base, int offset, int idx) {
		int correct = check(arr, idx - 2, base, offset);
		if (correct < idx - 1) {
			heaps_fwd_skip(arr, idx);
			return null;
		}
		if (idx < arr.length - 1) {
			// INDUCTIVE CASE
			int[] result = heaps_fwd(arr, base, offset, idx + 1);
			if (result != null)
				return result;
			int len = arr.length - idx;

			for (int i = arr.length - 1; i > idx; --i) {
				if (len % 2 == 0)
					swap(arr, idx, i);
				else
					swap(arr, idx, arr.length - 1);

				result = heaps_fwd(arr, base, offset, idx + 1);
				if (result != null)
					return result;
			}
		} else {
			// BASE CASE: have a permutation in arr
			if (correct >= HEX) {
				return arr;
			}
		}
		return null;

	}

	static void swap(int[] arr, int i, int j) {
		int tmp = arr[i];
		arr[i] = arr[j];
		arr[j] = tmp;
	}

	static int check(int[] arr, int start, int base, int offset) {
		for (int idx = Math.max(start, 0); idx < HEX; ++idx) {
			int c = hexes[base][(offset + idx) % HEX];
			int n = hexes[base][(offset + idx + 1) % HEX];
			int[][] ch = adj[arr[idx]];
			int[][] nh = adj[arr[(idx + 1) % HEX]];

			if (ch[c][0] != nh[n][1]) {
				return idx;
			}
		}

		return HEX;
	}

	static int[] lex(int[] arr, int base, int offset) {
		// THIS CODE HAS A BUG BELOW DO NOT USE

		// Desc points to the first element that we know is the head of a
		// descending suffix

		int desc = arr.length - 1;
		// Safe points to the last element that we know is a valid prefix
		int safe = 0;
		while (true) {
			if (check(arr, safe, base, offset) >= HEX)
				return arr;

			// Find the first element that is not descending
			while (desc > 0 && arr[desc - 1] > arr[desc]) {
				desc--;
			}
			// Reached the last permutation
			if (desc == 0)
				break;

			int next;

			// Find the first smaller element in the descending portion
			for (next = desc; next < arr.length; ++next) {
				if (arr[next] < arr[desc - 1]) {
					break;
				}
			}

			do {
				--next;
				// Swap the next largest element in the descending portion
				swap(arr, next, desc - 1);
				// If the prefix with that new swap is invalid, continue
				safe = check(arr, Math.min(safe, desc - 2), base, offset);
			} while (next >= desc && safe < desc - 1);

			// If none of the prefixes were valid, our entire array now
			// descending, so continue to the next digit
			if (next < desc)
				continue;

			// Reverse the remaining indices, and consider that permutation
			for (int i = desc, j = arr.length - 1; i < j; ++i, --j) {
				swap(arr, i, j);
			}
			desc = arr.length - 1;
		}

		return null;
	}

	static int[] soln(int[] prev, int idx, int base, int offset, BitSet left) {
		if (idx == NHEX - 1) {
			int c = hexes[base][(offset + HEX - 1) % HEX];
			int n = hexes[base][offset % HEX];

			int[][] ch = adj[prev[HEX - 1]];
			int[][] nh = adj[prev[0]];

			if (ch[c][0] != nh[n][1]) {
				return null;
			}

			return prev;
		} else {
			for (int i = left.nextSetBit(0); i != -1; i = left.nextSetBit(i + 1)) {
				prev[idx] = i;

				left.clear(i);
				int[] res = null;
				if (idx > 0) {
					int c = hexes[base][(offset + idx - 1) % HEX];
					int n = hexes[base][(offset + idx) % HEX];
					int[][] ch = adj[prev[idx - 1]];
					int[][] nh = adj[prev[idx]];

					if (ch[c][0] == nh[n][1]) {
						res = soln(prev, idx + 1, base, offset, left);
					}
				} else {
					res = soln(prev, idx + 1, base, offset, left);
				}
				left.set(i);

				if (res != null)
					return res;
			}

			return null;
		}
	}

	static class FastScan implements Closeable {
		private BufferedReader br;
		private StringTokenizer tk;

		public FastScan(BufferedReader br) {
			this.br = br;
		}

		public int in() throws NumberFormatException, IOException {
			return Integer.parseInt(next());
		}

		public long ln() throws NumberFormatException, IOException {
			return Long.parseLong(next());
		}

		public double db() throws NumberFormatException, IOException {
			return Double.parseDouble(next());
		}

		@Override
		public void close() throws IOException {
			tk = null;
			br.close();
		}

		public String next() throws IOException {
			while (tk == null || !tk.hasMoreTokens()) {
				String line = br.readLine();
				if (line == null)
					return null;
				tk = new StringTokenizer(line);
			}
			return tk.nextToken();
		}
	}

	/*
	 * 2 3 5 6 1 2 4 5 1 2 3 6 4 2 3 5 4 1 6 3 1 5 6 2 4 5 4 1 3 6 2 4 2 3 1 5 6
	 * 3 6 1 2 4 5 6 3 4 1 2 5 6 4 3 2 5 1 6 5 3 2 4 1 5 4 6 3 2 1 2 5 6 1 4 3 4
	 * 6 3 5 2 1 1 3 5 2 6 4
	 */
}
