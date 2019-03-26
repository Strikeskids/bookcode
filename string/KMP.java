public class KMP {
	public static int[] makeLookback(String s) {
		int[] kmp = new int[s.length() + 1];
		kmp[0] = -1;

		for (int i = 1, j = 0; i < s.length(); i++, j++) {
			if (s.charAt(i) == s.charAt(j)) {
				kmp[i] = kmp[j];
			} else {
				kmp[i] = j;
				while (j >= 0 && s.charAt(i) != s.charAt(j)) {
					j = kmp[j];
				}
			}
		}

		return kmp;
	}

	// Returns the lowest index i such that t appears in s at index i,
	// or -1 if no such index exists
	public static int search(String s, String t) {
		int[] f = makeLookback(t);
		int i = 0;
		int j = 0;

		while (i < s.length()) {
			while (j >= 0 && s.charAt(i) != t.charAt(j)) {
				j = f[j];
			}

			i++;
			j++;

			if (j == t.length()) {
				return i - t.length();
			}
		}

		return -1;
	}
}