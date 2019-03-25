import java.util.*;

public class TestHungarian {
	static final int CASES = 30;
	static final int N = 1000;

	public static void main(String... args) {
		Random r = new Random();
		for (int CASE = 1; CASE <= CASES; ++CASE) {
			System.err.println("Running case: " + CASE);
			long[][] mat = new long[N][N];
			for (int i=0;i<N;++i) {
				for (int j=0;j<N;++j) {
					mat[i][j] = r.nextLong() >>> 2;
				}
			}
			Hungarian.Node[][] ns = Hungarian.hungarianMin(mat);
			checkProof(ns[0], ns[1], mat);
		}
	}

	static void assert_(boolean b) {
		if (!b) throw new AssertionError();
	}

	static void checkProof(Hungarian.Node[] A, Hungarian.Node[] B, long[][] costs) {
		for (Hungarian.Node a : A) {
			assert_(A[a.idx] == a && a.pair != null);
			assert_(a.pair.pair == a && B[a.pair.idx] == a.pair);
			assert_(costs[a.idx][a.pair.idx] == a.price + a.pair.price);
			for (Hungarian.Node b : B) {
				assert_(costs[a.idx][b.idx] >= a.price + b.price);
			}
		}
	}
}