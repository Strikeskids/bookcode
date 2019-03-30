ld F(ld x) {}
ld simpson(ld a, ld b) {
  ld c = a + (b-a)/2;
  return (F(a)+4*F(c)+F(b))*(b-a)/6;
}
ld asr(ld a, ld b, ld eps, ld A) {
  ld c = a + (b-a)/2;
  ld L = simpson(a, c), R = simpson(c, b);
  if(fabs(L+R-A) <= 15*eps) return L+R+(L+R-A)/15.0;
  return asr(a, c, eps/2, L) + asr(c, b, eps/2, R);
}
ld asr(ld a, ld b, ld eps) {
  return asr(a, b, eps, simpson(a, b));
}
