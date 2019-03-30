/* [A.java] */
import java.math.*;
import java.util.*;
import java.io.*;

public class A {
	FastScanner in;
	PrintWriter out;
	int N;
	BigInteger[] fac = new BigInteger[105];
	void run() throws Exception {
		in = new FastScanner(new File("meeting.in"));
		out = new PrintWriter(new File("meeting.out"));
		in = new FastScanner(System.in);
		out = new PrintWriter(System.out);
		N = in.nextInt();
		fac[0] = BigInteger.ONE;
		BigDecimal lo = new BigDecimal(2);
		for (int i = 1; i <= 2 * (N + 1); ++i) fac[i] = fac[i - 1].multiply(BigInteger.valueOf(i));
		BigInteger ans = fac[2 * (N + 1)].divide(fac[N + 1].multiply(fac[N + 2]));
		out.println(ans);
		out.close();
	}
	class FastScanner {
		BufferedReader reader;
		StringTokenizer tokenizer;
		FastScanner(File f) throws Exception {
			reader = new BufferedReader(new FileReader(f));
		}
		FastScanner(InputStream stream) {
			reader = new BufferedReader(new InputStreamReader(stream));
		}
		String next() {
			while (tokenizer == null || !tokenizer.hasMoreTokens()) {
				try {
					tokenizer = new StringTokenizer(reader.readLine());
				} catch (IOException e) {
					throw new RuntimeException(e);
				}
			}
			return tokenizer.nextToken();
		}
		int nextInt() {
			return Integer.parseInt(next());
		}
		BigInteger nextBigInteger() {
			return new BigInteger(next());
		}
	}
	public static void main(String[] args) throws Exception {
		new A().run();
	}
}

