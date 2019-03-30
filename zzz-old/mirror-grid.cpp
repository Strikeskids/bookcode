/* [mirror-grid.cpp] */
// Simulates shooting a laser from (r,c) in direction (dr,dc) until
// it hits an obstacle # or gets out of the grid
bool sim(int r, int c, int dr, int dc) {
	for (;;) {
		r += dr;
		c += dc;
		if (r < 0 || c < 0 || r >= R || c >= C) return true;
		if (G[r][c] == '#') return true;
		if (G[r][c] == '/') {
			swap(dr, dc);
			dr = -dr;
			dc = -dc;
		} else if (G[r][c] == '\\') {
			swap(dr, dc);
		}
	}
}

