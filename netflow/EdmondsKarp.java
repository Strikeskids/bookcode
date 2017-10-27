/*
Edmonds-Karp #1
O(m lg F)

flow = 0
while true:
	find an augmenting path with the maximum flow
		(this is done via a modified Dijkstra)
	if no augmenting path of non-zero flow exists:
		break
	else:
		flow += flow of that augmenting path
		push flow along augmenting path
return flow
*/

import java.io.*;
import java.util.*;

public class EdmondsKarp {
	public static final int INFINITY = 2000000000;

	public static void main(String[] args) throws Exception {
		// ...
	}

	public static int edmondskarp(Node s, Node t) {
		int flow = 0;

		while (true) {
			int insertGen = s.gen + 1;
			int removeGen = insertGen + 1;

			PriorityQueue<Node> pq = new PriorityQueue<Node>();
			pq.add(s);

			while (!pq.isEmpty()) {
				Node next = pq.poll();

				if (next.gen == removeGen) {
					continue;
				}

				next.gen = removeGen;

				if (next == t) {
					break;
				}

				for (Node n : next.edges.keySet()) {
					if (next.edges.get(n) == 0) {
						continue;
					}

					if (n.gen < insertGen
						|| n.gen == insertGen
							&& n.d < Math.min(next.d, next.edges.get(n))) {
						n.gen = insertGen;
						n.d = Math.min(next.d, next.edges.get(n));
						n.prev = next;
						pq.add(n);
					}
				}
			}

			if (t.gen != removeGen) {
				break;
			} else {
				flow += t.d;
				pushPath(s, t, t.d);
			}
		}

		return flow;
	}

	public static void pushPath(Node s, Node t, int f) {
		while (s != t) {
			push(t.prev, t, f);
			t = t.prev;
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

	static class Node implements Comparable<Node> {
		HashMap<Node, Integer> edges;
		int gen;
		String name;

		Node prev;
		int d;

		public Node(String name) {
			edges = new HashMap<Node, Integer>();
			gen = -1;
			d = INFINITY;
			this.name = name;
		}

		public int compareTo(Node o) {
			return o.d - d;
		}
	}
}