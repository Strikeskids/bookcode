int wa[maxn],wb[maxn],wv[maxn],ws[maxn]; // set maxn with +1 extra

int sacmp(int *r,int a,int b,int l) {
	return r[a]==r[b]&&r[a+l]==r[b+l];
}

// input: r: array of length n+1, 0 <= r[i] < m, r[n] < 0
// output: sa: suffix array (suffixes sorted lexicographically)
void compute_sa(int n,int m,int *r,int *sa) {
	int i,j,p,*x=wa,*y=wb,*t;
	assert(n<maxn&&m<maxn&&r[n]<0);
	for(i=0;i<n;++i) assert(r[i]<m);
	for(i=0;i<m;i++) ws[i]=0;
	for(i=0;i<n;i++) ws[x[i]=r[i]]++;
	y[n]=x[n]=-1;
	for(i=1;i<m;i++) ws[i]+=ws[i-1];
	for(i=n-1;i>=0;i--) sa[--ws[x[i]]]=i;
	for(j=1,p=1;p<n;j*=2,m=p) {
		assert(j<=n);
		for(p=0,i=n-j;i<n;i++) y[p++]=i;
		for(i=0;i<n;i++) if(sa[i]>=j) y[p++]=sa[i]-j;
		for(i=0;i<n;i++) wv[i]=x[y[i]];
		for(i=0;i<m;i++) ws[i]=0;
		for(i=0;i<n;i++) ws[wv[i]]++;
		for(i=1;i<m;i++) ws[i]+=ws[i-1];
		for(i=n-1;i>=0;i--) sa[--ws[wv[i]]]=y[i];
		for(t=x,x=y,y=t,p=1,x[sa[0]]=0,i=1;i<n;i++)
			x[sa[i]]=sacmp(y,sa[i-1],sa[i],j)?p-1:p++;
	}
	sa[n] = n;
}

// input: r: array of length n+1, sa: suffix array, r[n] = -1
// output: h: h[i] = lcp(sa[i],sa[i+1])
void compute_lcp(int n,int *r,int *sa,int *h) {
	int i,j,k=0;
	for(i=0;i<n;i++) wa[sa[i]]=i;
	for(i=0;i<n;h[wa[i++]]=k)
		for(k?k--:0,j=sa[wa[i]+1];r[i+k]==r[j+k];k++) {}
}
