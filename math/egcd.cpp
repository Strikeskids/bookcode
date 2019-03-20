tuple<ll,ll,ll> egcd(ll n, ll m) {
	assert(n>=0&&m>=0);
	ll q,a2,x2,y2,a0=n,a1=m,x0=1,x1=0,y0=0,y1=1;
	while (a1) {
		q = a0 / a1;
		a2 = a0 - a1*q, a0 = a1, a1 = a2;
		x2 = x0 - x1*q, x0 = x1, x1 = x2;
		y2 = y0 - y1*q, y0 = y1, y1 = y2;
	}
	assert(-m<x0&&x0<m && -n<y0&&y0<n);
	assert(n*x0 + m*y0 == a0 && gcd(n, m) == a0);
	return make_tuple(a0,x0,y0);
}
