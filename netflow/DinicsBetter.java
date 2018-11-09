import java.io.BufferedReader;
import java.io.Closeable;
import java.io.IOException;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Deque;
import java.util.List;
import java.util.StringTokenizer;

public class DinicsBetter {

	static int round;

	/** Perform dinics by continually recomputing blocking flows while they exist */
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
		S.pushedForward = 0;
		S.capacity = Long.MAX_VALUE;
		S.prev = null;

		for (Node n = S; !n.levelEdges.isEmpty() || n.prev != null;) {
			if (n == T) {
				// If at the sink, we can consume all of the flow
				n.pushedForward = n.capacity;
			}

			if (n.levelEdges.isEmpty() || n.capacity == n.pushedForward) {
				// If we've saturated a node or run out of edges, finish the node
				Edge e = n.prev;
				Node parent = e.opp(n);

				// Pull the flow to decrease the outgoing flow to 0 at the current node
				e.pull(n, n.pushedForward);
				parent.pushedForward += n.pushedForward;

				// If we haven't saturated the edge, add it back to the parent's list
				if (e.cap(parent) > 0)
					parent.levelEdges.addLast(e);

				n = parent;
			} else {
				// We've still got more flow to push and edges to examine
				Edge e = n.levelEdges.pollFirst();
				Node child = e.opp(n);

				// Recurse into the child, seeking to push flow from it
				child.capacity = Math.min(n.capacity - n.pushedForward, e.cap(n));
				child.pushedForward = 0;
				child.prev = e;

				n = child;
			}
		}

		// Return the total flow pushed out of the source
		return S.pushedForward;
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

			int level = n.level;

			for (Edge e : n.edges) {
				Node a = e.opp(n);
				if (e.cap(n) == 0)
					continue;

				// If we haven't seen a node yet, add it to the queue
				if (a.round != round) {
					a.round = round;
					a.level = level + 1;
					open.addLast(a);
				}

				// If the node is on the next level, enable the edge to be used
				if (a.round == round && a.level == level + 1) {
					n.levelEdges.addLast(e);
				}
			}
		}
	}

	static class Node {
		int id;

		int round;
		int level;
		Deque<Edge> levelEdges = new ArrayDeque<>();

		long pushedForward;
		long capacity;
		Edge prev;

		List<Edge> edges = new ArrayList<>();

		Node(int id) {
			this.id = id;
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
