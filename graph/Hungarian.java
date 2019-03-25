import java.util.*;

public class Hungarian {
	static final long INFTY = (long) 1e18;

	static class Node {
		int idx;
		long price, slack;
		Node pair, prev;

		Node(int i) {
			this.idx = i;
		}
	}
 
	static Node[][] hungarianMin(long[][] cost) {
		int N = cost.length;

		Node[] A = new Node[N];
		Node[] B = new Node[N];

		for (int i = 0; i < N; ++i) {
			long initPrice = INFTY;
			for (int j = 0; j < N; ++j)
				initPrice = Math.min(initPrice, cost[i][j]);
			A[i] = new Node(i);
			A[i].price = initPrice;
			B[i] = new Node(i);
		}

		for (Node start : A) {
			Node pathB = null;

			for (Node a : A) {
				a.prev = null;
			}
			for (Node b : B) {
				b.slack = cost[start.idx][b.idx] - start.price - b.price;
				b.prev = start;
			}

			// Grow alternating zero-cost component
			start.prev = start;
			grow: while (true) {
				long delta = INFTY;
				Node nxt = null;
				for (Node b : B) {
					if (b.slack != 0) {
						delta = Math.min(delta, b.slack);
						continue;
					}
					if (b.pair == null) {
						pathB = b;
						break grow;
					}
					if (b.pair.prev == null) nxt = b.pair;
				}
				if (nxt == null) {
					// decrease #(b.slack != 0) by 1
					for (Node b : B) {
						if (b.slack != 0) {
							b.slack -= delta;
						} else {
							b.price -= delta;
						}
					}
					for (Node a : A) {
						if (a.prev != null) {
							a.price += delta;
						}
					}
				} else {
					// decrease #(a.prev == null) by 1
					nxt.prev = nxt.pair.prev;
					for (Node b : B) {
						long rc = cost[nxt.idx][b.idx] - nxt.price - b.price;
						if (rc < b.slack) {
							b.prev = nxt;
							b.slack = rc;
						}
					}
				}
			}

			// Augment matching along zero-cost path
			while (pathB != null) {
				Node pathA = pathB.prev, prevB = pathA.pair;

				pathA.pair = pathB;
				pathB.pair = pathA;

				pathB = prevB;
			}
		}

		return new Node[][]{A, B};
	}
}