import java.util.*;

public class Dinics {
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

		for (Node n = S; !n.le.isEmpty() || n.prev != null;) {
			// If at the sink, we can consume all of the flow
			if (n == T) n.pushed = n.cap;

			if (n.le.isEmpty() || n.cap == n.pushed) {
				// If we've saturated a node or run out of edges, finish the node
				Edge e = n.prev;
				Node p = e.opp(n);

				// Pull the flow to decrease the outgoing flow to 0 at the current node
				e.pull(n, n.pushed);
				p.pushed += n.pushed;

				// If we haven't saturated the edge, add it back to the parents's list
				if (!n.le.isEmpty() && e.cap(p) > 0)
					p.le.addLast(e);

				n = p;
			} else {
				// We've still got more flow to push and edges to examine
				Edge e = n.le.pollLast();
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

			n.le.clear();
			for (Edge e : n.es) {
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
					n.le.addLast(e);
				}
			}
		}
	}

	static class Node {
		int id, round, level;
		Deque<Edge> le = new ArrayDeque<>(); // levelEdges

		long cap, pushed;
		Edge prev;

		List<Edge> es = new ArrayList<>();

		Node(int i) { id = i; }

		void connect(Node dst, long fwd, long bwd) {
			Edge e = new Edge(this, dst, fwd, bwd);
			es.add(e); dst.es.add(e);
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
