#include <bits/stdc++.h>

using namespace std;

//////// ---- Woosung Song's Source Code ---- ////////
#ifndef FAST_FOURIER_DEFINED
namespace fast_fourier {
	typedef int dtype;
	typedef complex<double> base;

	const double FFT_PI = atan(1) * 4;

	void transform(vector<base> &a, bool invert) {
		int n = a.size();
		for (int i=1, j=0, bit; i<n; i++) {
			for (bit=n>>1; j>=bit; bit>>=1) j -= bit;
			j += bit;
			if (i<j) swap(a[i], a[j]);
		}
		for (int len=2; len<=n; len<<=1) {
			double th = 2 * FFT_PI / len * (invert? -1 : 1);
			base wlen(cos(th), sin(th));
			for (int i=0; i<n; i+=len) {
				base w(1, 0);
				for (int j=0; j<len/2; j++) {
					int ai = i + j, bi = i + j + len/2;
					base u = a[ai], v = a[bi] * w;
					a[ai] = u + v;
					a[bi] = u - v;
					w *= wlen;
				}
			}
		}
		if (invert) for (int i=0; i<n; i++) a[i] /= n;
	}

	vector<base>& __extend(vector<dtype> &a, int sz) {
		vector<base> *pv = new vector<base>(sz, 0), &v = *pv;
		for (int i=0; i<(int) a.size(); i++) v[i] = a[i];
		return v;
	}

	vector<dtype>& multiply(vector<dtype> &a, vector<dtype> &b) {
		int n = max((int) a.size(), (int) b.size()), m;
		for (m=1; m<n; m<<=1);
		n = m << 1;
		vector<base> &u = __extend(a, n), &v = __extend(b, n);
		transform(u, 0), transform(v, 0);
		for (int i=0; i<n; i++) u[i] *= v[i];
		transform(u, 1);
		vector<dtype> *pc = new vector<dtype>(n, 0), &c = *pc;
		for (int i=0; i<n; i++)
			c[i] = (u[i].real() + (u[i].real() > 0? 0.5 : -0.5));
		while (c.size() && c.back() == 0) c.pop_back();
		u.clear(), v.clear();
		return c;
	}
}
#endif

#define FAST_FOURIER_DEFINED
//////// ---- Woosung Song's Source Code ---- ////////


int main() {
	string s, t;
	cin >> s >> t;
	vector<int> a((int) s.size()), b((int) t.size());
	for (int i=0; i<(int) s.size(); i++) a[i] = s[i] - '0';
	for (int i=0; i<(int) t.size(); i++) b[i] = t[i] - '0';
	reverse(a.begin(), a.end());
	reverse(b.begin(), b.end());
	vector<int> &c = fast_fourier::multiply(a, b);
	int oc = c.size();
	c.resize(oc + 10, 0);

	for (int i=0; i<oc; i++) {
		int val = c[i];
		c[i] = val % 10, val /= 10;
		for (int j=i+1; val; j++, val/=10) c[j] += val % 10;
	}
	while (c.size() && c.back() == 0) c.pop_back();

	if (c.size()) {
		reverse(c.begin(), c.end());
		for (int i=0; i<(int)c.size(); i++) cout << c[i];
		cout << endl;
	} else
		cout << 0 << endl;
}
