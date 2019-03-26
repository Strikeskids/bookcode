import java.util.*;

public class TestKMP {
	public static void main(String[] args) {
		Random rnd = new Random(0x15295);

		for (int b = 0; b < 3; b++) {
			for (int k = 1; k <= 1000; k++) {
				int sl = (k+1) * (k+1);
				int tl = k;
				int a = 4 + 11*b;

				StringBuilder sb = new StringBuilder();
				for (int i = 0; i < sl; i++) {
					sb.append((char)('a' + rnd.nextInt(a)));
				}

				String s = sb.toString();
				String t;

				if (rnd.nextBoolean()) {
					int i = rnd.nextInt(sl - tl);
					t = s.substring(i, i + tl);
				} else {
					sb = new StringBuilder();
					for (int i = 0; i < tl; i++) {
						sb.append((char)('a' + rnd.nextInt(a)));
					}
					t = sb.toString();
				}

				int ans = s.indexOf(t);
				int given = KMP.search(s, t);

				if (given == ans) {
					System.out.printf("%4d: OK (%d)\n", k + 1000*b, ans);
				} else {
					System.out.printf("%4d: WA (%d/%d)\n", k + 1000*b, ans, given);
					break;
				}
			}
		}
	}
}