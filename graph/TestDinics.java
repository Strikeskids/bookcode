import java.util.*;
import java.io.*;

public class TestDinics {
	public static void main(String... args) throws Exception {
		FastScan sc = new FastScan(new BufferedReader(new InputStreamReader(System.in)));
		PrintWriter pw = new PrintWriter(System.out);

		int N = sc.in();
		int M = sc.in();

		Node S = new Node(-1);
		Node T = new Node(-2);
		Node[] sts = new Node[N];
		Node[] pairs = new Node[M];

		for (int i = 0; i < N; ++i) {
			sts[i] = new Node(i + 1);
			S.connect(sts[i], 1, 0);
		}
		for (int i = 0; i < M; ++i) {
			Node s1 = sts[sc.in() - 1];
			Node s2 = sts[sc.in() - 1];
			pairs[i] = new Node(1000 + i);
			s1.connect(pairs[i], 1, 0);
			s2.connect(pairs[i], 1, 0);
			pairs[i].connect(T, 1, 0);
		}

		int lo = 0, hi = N + 1;
		long flow = 0;
		while (lo < hi) {
			int mid = lo + (hi - lo) / 2;

			flow -= limit(S, sts, mid);
			flow += dinics(S, T);
			if (flow == M) {
				hi = mid;
			} else {
				lo = mid + 1;
			}
		}
		limit(S, sts, lo);
		dinics(S, T);
		pw.println(lo);

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

	static long limit(Node S, Node[] sts, int cap) {
		long decrease = 0;
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
					decrease++;
				}
			}
			root.fwd = cap - root.bwd;
		}
		return decrease;
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

	static final long INFTY = Long.MAX_VALUE/2;
	static int round;

	/* Returns new flow pushed within the flow graph */
	static long dinics(Node S, Node T) {
		long flow = 0, currentFlow;
		do {
			levelGraph(S, T);
			flow += currentFlow = blockingFlow(S, T);
		} while (currentFlow > 0);
		return flow;
	}

	/**
	 * Find a blocking flow in a level graph from S to T. This is implemented by
	 * unrolling a recursive function into a loop. We keep the call stack implicitly
	 * in prev edge pointers on each node.
	 */
	static long blockingFlow(Node S, Node T) {
		if (T.round != round) return 0;

		S.pushed = 0;
		S.cap = INFTY;
		S.prev = null;

		for (Node n = S; !n.levelEdges.isEmpty() || n.prev != null;) {
			// If at the sink, we can consume all of the flow
			if (n == T) n.pushed = n.cap;

			if (n.levelEdges.isEmpty() || n.cap == n.pushed) {
				// If we've saturated a node or run out of edges, finish the node
				Edge e = n.prev;
				Node p = e.opp(n);

				// Pull the flow to decrease the outgoing flow to 0 at the current node
				e.pull(n, n.pushed);
				p.pushed += n.pushed;

				// If we haven't saturated the edge, add it back to the parents's list
				if (!n.levelEdges.isEmpty() && e.cap(p) > 0)
					p.levelEdges.addLast(e);

				n = p;
			} else {
				// We've still got more flow to push and edges to examine
				Edge e = n.levelEdges.pollLast();
				Node c = e.opp(n);
				if (c != T && c.level >= T.level) continue;
				if (c.level != n.level+1) throw new RuntimeException();

				// Recurse into the child, seeking to push flow from it
				c.cap = Math.min(n.cap - n.pushed, e.cap(n));
				c.pushed = 0;
				c.prev = e;

				n = c;
			}
		}

		// Return the total flow pushed out of the source
		return S.pushed;
	}

	/**
	 * Compute the level graph of the flow network with source S and sink T. This
	 * will populate the levelEdges of all reasonable nodes.
	 * 
	 * We use round-based initialization so that we don't have to pass the full list
	 * of nodes.
	 */
	static void levelGraph(Node S, Node T) {
		Deque<Node> open = new ArrayDeque<>();

		S.round = ++round;
		S.level = 0;

		open.addLast(S);
		while (!open.isEmpty()) {
			Node n = open.pollFirst();
			if (n == T) break; // optimization

			int level = n.level;

			n.levelEdges.clear();
			for (Edge e : n.edges) {
				if (e.cap(n) == 0)
					continue;
				
				Node a = e.opp(n);

				// If we haven't seen a node yet, add it to the queue
				if (a.round != round) {
					a.round = round;
					a.level = level + 1;
					open.addLast(a);
				}

				// If the node is on the next level, enable the edge to be used
				if (a.level == level + 1) {
					n.levelEdges.addLast(e);
				}
			}
		}
	}

	static class Node {
		int id, round, level;
		Deque<Edge> levelEdges = new ArrayDeque<>();

		long cap, pushed;
		Edge prev;

		List<Edge> edges = new ArrayList<>();

		Node(int i) { id = i; }

		void connect(Node dst, long fwd, long bwd) {
			Edge e = new Edge(this, dst, fwd, bwd);
			edges.add(e); dst.edges.add(e);
		}
	}

	static class Edge {
		long fwd;
		long bwd;
		Node src;
		Node dst;

		public Edge(Node u, Node v, long f, long b) {
			src = u; dst = v; fwd = f; bwd = b;
			if (src == dst || fwd < 0 || bwd < 0) throw new RuntimeException();
		}

		Node opp(Node n) {
			if (n == src) return dst;
			else if (n == dst) return src;
			else throw new RuntimeException();
		}

		long cap(Node n) {
			if (n == src) return fwd;
			else if (n == dst) return bwd;
			else throw new RuntimeException();
		}

		void pull(Node n, long value) {
			if (n == dst) {
				if (fwd < value) throw new RuntimeException();
				fwd -= value; bwd += value;
			} else if (n == src) {
				if (bwd < value) throw new RuntimeException();
				bwd -= value; fwd += value;
			} else {
				throw new RuntimeException();
			}
		}
	}
}