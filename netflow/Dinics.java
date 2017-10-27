/*
Dinic's
O(n^2 m)

flow = 0
while true: // executed at most n times
	construct a level graph using a BFS

	// find and push a blocking flow
	f = 0
	while true: // executed at most m times per iteration
		if there is an augmenting flow:
			f += its size
			push it
		else:
			break
	if f > 0:
		flow += f
	else:
		break

return flow
*/

import java.io.*;
import java.util.*;

public class Dinics {
	public static final int INFINITY = 2000000000;

	public static void main(String[] args) throws Exception {
		// ...
	}

	public static int dinics(Node s, Node t) {
		int flow = 0;

		while (true) {
			makeLevelGraph(s, t);
			int levelFlow = 0;

			while (true) {
				int f = search(s, t);

				if (f == 0) {
					break;
				} else {
					levelFlow += f;
					pushPath(s, t, f);
				}
			}

			flow += levelFlow;

			if (levelFlow == 0) {
				break;
			}
		}

		return flow;
	}

	public static void makeLevelGraph(Node s, Node t) {
		Queue<Node> queue = new LinkedList<Node>();
		queue.add(s);
		int gen = s.gen + 1;
		s.gen = gen;
		s.level = 0;

		while (!queue.isEmpty()) {
			Node next = queue.poll();
			next.levelEdges.clear();
			next.n = 0;
			if (next == t) {
				break;
			}
			for (Node n : next.edges.keySet()) {
				if ((n.gen < gen || n.gen == gen && n.level > next.level)) {
					if (next.edges.get(n) == 0) {
					} else {
						next.levelEdges.add(n);
						if (n.gen < gen) {
							n.gen = gen;
							n.level = next.level + 1;
							queue.add(n);
						}
					}
				}
			}
		}
	}

	public static int search(Node s, Node t) {
		int ret = -1;

		Stack<Node> stack = new Stack<Node>();
		stack.push(s);

		while (!stack.isEmpty()) {
			Node v = stack.peek();

			if (v == t) {
				ret = INFINITY;
				stack.pop();
				continue;
			}

			if (ret > 0) {
				ret = Math.min(v.edges.get(v.levelEdges.get(v.n)), ret);
				stack.pop();
				continue;
			}

			if (ret == 0) {
				v.n++;
				ret = -1;
			}

			if (v.n == v.levelEdges.size()) {
				ret = 0;
				stack.pop();
				continue;
			}

			if (v.edges.get(v.levelEdges.get(v.n)) == 0) {
				v.n++;
			} else {
				stack.push(v.levelEdges.get(v.n));
			}
		}

		return ret;
	}

	public static void pushPath(Node s, Node t, int f) {
		while (s != t) {
			push(s, s.levelEdges.get(s.n), f);
			s = s.levelEdges.get(s.n);
		}
	}

	public static void push(Node a, Node b, int f) {
		a.edges.put(b, a.edges.get(b) - f);
		b.edges.put(a, b.edges.get(a) + f);
	}

	public static void edge(Node a, Node b, int c) {
		if (!a.edges.containsKey(b)) {
			a.edges.put(b, 0);
			b.edges.put(a, 0);
		}
		a.edges.put(b, a.edges.get(b) + c);
	}

	static class Node {
		HashMap<Node, Integer> edges;
		int gen;
		String name;
		int level;
		ArrayList<Node> levelEdges;
		int n;

		public Node(String name) {
			edges = new HashMap<Node, Integer>();
			gen = -1;
			level = -1;
			levelEdges = new ArrayList<Node>();
			n = 0;
			this.name = name;
		}
	}
}