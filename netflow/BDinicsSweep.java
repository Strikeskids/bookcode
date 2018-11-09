import java.io.BufferedReader;
import java.io.Closeable;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Deque;
import java.util.StringTokenizer;

public class BDinicsSweep {

	static int round;

	public static void main(String... args) throws Exception {
		FastScan sc = new FastScan(new BufferedReader(new InputStreamReader(System.in)));
		PrintWriter pw = new PrintWriter(System.out);

		int N = sc.in();
		int M = sc.in();

		Node S = new Node();
		S.id = -1;
		Node T = new Node();
		T.id = -2;
		Node[] sts = new Node[N];
		Node[] pairs = new Node[M];

		for (int i = 0; i < N; ++i) {
			sts[i] = new Node();
			sts[i].id = i + 1;
			S.connect(sts[i], 0);
		}
		for (int i = 0; i < M; ++i) {
			Node s1 = sts[sc.in() - 1];
			Node s2 = sts[sc.in() - 1];
			pairs[i] = new Node();
			pairs[i].id = 1000 + i;
			s1.connect(pairs[i], 1);
			s2.connect(pairs[i], 1);
			pairs[i].connect(T, 1);
		}

		long flow = dinics(S, T);
		int cap;
		for (cap = 0; cap <= N && flow < M;) {
			++cap;
			limit(S, sts, cap);
			long curFlow;
			do {
				curFlow = dinicsOnce(S, T);
				flow += curFlow;
			} while (curFlow > 0);
		}
		pw.println(cap);

		for (Node p : pairs) {
			Edge a = p.edges.get(0);
			Edge b = p.edges.get(1);
			if (b.cap(p) == 1) {
				Edge tmp = a;
				a = b;
				b = tmp;
			}
			pw.println(a.opp(p).id + " " + b.opp(p).id);
		}

		sc.close();
		pw.close();
		System.exit(0);
	}

	static long flow(Node S, Node T) {
		// return edmondsKarp(S, T);
		return dinics(S, T);
	}

	static void limit(Node S, Node[] sts, int cap) {
		for (Node n : sts) {
			Edge root = n.edges.get(0);
			long total = root.cap(n);
			for (Edge e : n.edges) {
				if (total <= cap)
					break;
				Node pair = e.opp(n);
				if (pair == S)
					continue;
				if (e.cap(n) == 0) {
					// pull from sink
					root.pull(S, 1);
					e.pull(n, 1);
					pair.edges.get(2).pull(pair, 1);
					total--;
				}
			}
			root.fwd = cap - root.bwd;
		}
	}

	static long dinicsOnce(Node S, Node T) {
		Deque<Node> open = new ArrayDeque<>();
		Node sentinel = new Node();

		++round;

		open.clear();
		S.round = round;
		S.prev = null;
		int level = S.level = 0;
		open.addLast(S);
		open.addLast(sentinel);
		while (open.size() > 1) {
			Node cur = open.pollFirst();
			if (cur == sentinel) {
				level++;
				open.addLast(cur);
				continue;
			}
			if (cur == T) {
				break;
			}
			cur.stk.clear();
			for (Edge e : cur.edges) {
				Node adj = e.opp(cur);
				long cap = e.cap(cur);
				if (cap > 0) {
					if (adj.round != round) {
						adj.round = round;
						adj.level = level + 1;
						cur.stk.addLast(e);
						open.addLast(adj);
					} else if (adj == T || adj.level == level + 1) {
						cur.stk.addLast(e);
					}
				}
			}
		}

		S.minCap = Long.MAX_VALUE;
		S.flowed = 0;

		for (Node cur = S;;) {
			if (cur == T)
				cur.flowed = cur.minCap;
			if (cur.stk.isEmpty() || cur.minCap == cur.flowed) {
				// finish
				if (cur.prev == null)
					break;
				Node next = cur.prev.opp(cur);
				cur.prev.pull(cur, cur.flowed);
				next.flowed += cur.flowed;

				cur = next;
			} else {
				// start
				Edge e = cur.stk.pollLast();
				Node adj = e.opp(cur);

				if (adj != T && adj.level >= T.level) {
					continue;
				}

				adj.minCap = Math.min(cur.minCap - cur.flowed, e.cap(cur));
				adj.flowed = 0;
				adj.prev = e;
				cur = adj;
			}
		}

		return S.flowed;
	}

	static long dinics(Node S, Node T) {
		long flow = 0, curFlow;
		for (Edge e : S.edges) {
			flow += e.bwd;
		}

		do {
			curFlow = dinicsOnce(S, T);
			flow += curFlow;
		} while (curFlow > 0);

		return flow;
	}

	static long edmondsKarp(Node S, Node T) {
		Deque<Node> open = new ArrayDeque<>();
		while (true) {
			++round;

			open.clear();
			S.round = round;
			S.minCap = Long.MAX_VALUE;
			open.addLast(S);
			outer: while (!open.isEmpty()) {
				Node cur = open.pollLast();
				for (Edge e : cur.edges) {
					Node adj = e.opp(cur);
					long cap = e.cap(cur);
					if (cap > 0 && adj.round != round) {
						adj.round = round;
						adj.minCap = Math.min(cur.minCap, e.cap(cur));
						adj.prev = e;
						if (adj == T)
							break outer;
						open.addLast(adj);
					}
				}
			}

			if (T.round != round)
				break;

			long value = T.minCap;
			for (Node cur = T; cur != S; cur = cur.prev.opp(cur)) {
				cur.prev.pull(cur, value);
			}
		}
		long totalFlow = 0;
		for (Edge e : T.edges) {
			totalFlow += e.cap(T);
		}
		return totalFlow;
	}

	static class Node {
		int id;

		int level;

		long flowed;

		int round;
		Edge prev;
		long minCap;

		Deque<Edge> stk = new ArrayDeque<>();
		ArrayList<Edge> edges = new ArrayList<>();

		void connect(Node dst, long fwd) {
			Edge e = new Edge(this, dst, fwd, 0);
			edges.add(e);
			dst.edges.add(e);
		}

		@Override
		public String toString() {
			return "Node [id=" + id + ", level=" + level + ", flowed=" + flowed + ", prev=" + prev + ", minCap="
					+ minCap + "]";
		}
	}

	static class Edge {
		long fwd;
		long bwd;
		Node src;
		Node dst;

		public Edge(Node src, Node dst, long fwd, long bwd) {
			super();
			this.src = src;
			this.dst = dst;
			this.fwd = fwd;
			this.bwd = bwd;
		}

		Node opp(Node n) {
			if (n == src)
				return dst;
			else if (n == dst)
				return src;
			else
				throw new RuntimeException();
		}

		long cap(Node n) {
			if (n == src)
				return fwd;
			else if (n == dst)
				return bwd;
			else
				throw new RuntimeException();
		}

		void pull(Node n, long value) {
			if (n == dst) {
				if (fwd < value)
					throw new RuntimeException();
				fwd -= value;
				bwd += value;
			} else if (n == src) {
				if (bwd < value)
					throw new RuntimeException();
				bwd -= value;
				fwd += value;
			} else {
				throw new RuntimeException();
			}
		}

		@Override
		public String toString() {
			return "Edge [" + fwd + "/" + bwd + " " + src.id + " to " + dst.id + "]";
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
}
