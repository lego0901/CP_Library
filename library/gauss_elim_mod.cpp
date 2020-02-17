#include <bits/stdc++.h>

using namespace std;

//////// ---- Woosung Song's Source Code ---- ////////
#ifndef GAUSS_ELIM_MOD_DEFINED
namespace gauss_elim_mod {
	typedef long long ll;

	ll MOD = 1e9 + 7;

	ll _gcd_two(ll a, ll b) {/*{{{*/
		return a<0ll? _gcd_two(-a, b) :
			   b<0ll? _gcd_two(a, -b) :
			   a<b? _gcd_two(b, a) : b? _gcd_two(b, a%b) : a;
	}
	
	ll _gcd(ll a) { return a<0ll? -a : a; }
	template<typename ... rests> ll _gcd(ll a, const rests&... rest) {
		ll b = _gcd(rest...);
		return _gcd_two(a, b);
	}
	
	void _extended_euclid(ll a, ll b, ll &x, ll &y) {
		if (a<0) { _extended_euclid(-a, b, x, y); x = -x; }
		else if (b<0) { _extended_euclid(a, -b, x, y); y = -y; }
		else if (a<b) { _extended_euclid(b, a, y, x); }
		else if (b>0) {
			_extended_euclid(b, a%b, y, x);
			y -= a/b*x;
		} else { x = 1, y = 0; }
	}

	ll _mod_clip(ll a, ll m) { return (a % m + m) % m; }
	ll _mod_clip(ll a) { return _mod_clip(a, MOD); }

	ll _mod_inverse(ll a, ll m) {
		// returns b s.t. a * b mod m = 1
		assert(_gcd(a, m) == 1);
		ll x, y;
		_extended_euclid(a, m, x, y);
		return _mod_clip(x, m);
	}
	ll _mod_inverse(ll a) { return _mod_inverse(a, MOD); }

	ll _mod_mult(ll a, ll b, ll m) {
		if (b<0) return _mod_clip(-_mod_mult(a, -b, m), m);
		ll r = 0, x = a;
		while (b) {
			if (b & 1) r = (r + x) % m;
			x = (x + x) % m; b >>= 1;
		}
		return _mod_clip(r, m);
	}
	ll _mod_mult(ll a, ll b) { return _mod_mult(a, b, MOD); }

	ll _mod_mults(ll a) { return _mod_clip(a); }
	template<typename ... rests> ll _mod_mults(ll a, const rests&... rest) {
		ll b = _mod_mults(rest...);
		return _mod_mult(a, b);
	}

	ll _mod_div(ll a, ll b, ll m) {
		assert(b != 0);
		return _mod_mult(a, _mod_inverse(b, m), m);
	}
	ll _mod_div(ll a, ll b) { return _mod_div(a, b, MOD); }/*}}}*/

	void get_gauss_elim_mod(vector<vector<ll> > &v) {
		int n = v.size(), m = v[0].size();
		vector<vector<ll>*> w;
		vector<int> first0;
		
		for (int i=0; i<n; i++)
			w.push_back(&v[i]);

		int r = 0;
		for (int j=0; j<m-1; j++) {
			int i;
			for (i=r; i<n; i++) if ((*w[i])[j] != 0) break;
			if (i == n) continue;
			swap(w[r], w[i]);
			first0.push_back(j);

			ll inv = _mod_inverse((*w[r])[j]);
			for (int k=0; k<m; k++)
				(*w[r])[k] = _mod_mult((*w[r])[k], inv);

			for (i=r+1; i<n; i++) {
				ll prod = (*w[i])[j];
				for (int k=0; k<m; k++)
					(*w[i])[k] = _mod_clip((*w[i])[k] - _mod_mult(prod, (*w[r])[k]));
			}
			r++;
		}
		while ((int) first0.size() < n) first0.push_back(m); // zero row

		for (int r=n-1; r>=0; r--) {
			int j = first0[r];
			if (j == m) continue;

			for (int i=0; i<r; i++) {
				ll prod = (*w[i])[j];
				for (int k=0; k<m; k++)
					(*w[i])[k] = _mod_clip((*w[i])[k] - _mod_mult(prod, (*w[r])[k]));
			}
		}

		vector<vector<ll> > u;
		for (int i=0; i<n; i++) {
			u.push_back(*w[i]);
		}
		swap(v, u);
	}

	vector<ll> one_solution(vector<vector<ll> > &v, bool run_gauss_elim=1) {
		if (run_gauss_elim) get_gauss_elim_mod(v);
		int n = v.size(), m = v[0].size();
		vector<ll> soln(m-1, 0);

		vector<int> first0;
		for (int i=0; i<n; i++) {
			for (int j=0; j+1<m; j++) {
				if (v[i][j] == 1) {
					first0.push_back(j);
					break;
				}
			}
			if ((int) first0.size() == i) first0.push_back(m); // zero row
		}

		for (int r=n-1; r>=0; r--) {
			int j = first0[r];
			if (j == m) {
				if (v[r][m-1] != 0) {
					soln.clear(); // no solution
					return soln;
				}
			} else {
				soln[j] = v[r][m-1];
				for (int k=j+1; k+1<m; k++) {
					soln[j] = _mod_clip(soln[j] - _mod_mult(soln[k], v[r][k]));
				}
			}
		}

		return soln;
	}

	bool invertible(vector<vector<ll> > &v, bool run_gauss_elim=1) {
		if (run_gauss_elim) get_gauss_elim_mod(v);
		int n = v.size(), m = v[0].size();
		if (m != n+1) return 0;

		for (int i=0; i<n; i++) {
			int flag = 1;
			for (int j=0; j+1<m; j++) {
				if (v[i][j] == 1) { flag = 0; break; }
			}
			if (flag) return 0;
		}
		return 1;
	}
};
#endif

#define GAUSS_ELIM_MOD_DEFINED
//////// ---- Woosung Song's Source Code ---- ////////

typedef long long ll;

int main() {
	vector<vector<ll> > v;
	int n, m;
	cin >> n >> m;
	for (int i=0; i<n; i++) {
		vector<ll> y;
		for (int j=0; j<=m; j++) {
			ll x;
			cin >> x;
			y.push_back(x);
		}
		v.push_back(y);
	}
	gauss_elim_mod::get_gauss_elim_mod(v);
	for (int i=0; i<n; i++) {
		for (int j=0; j<=m; j++) {
			cout << v[i][j] << ' ';
		}
		cout << endl;
	}

	vector<ll> soln = gauss_elim_mod::one_solution(v, 0);
	for (int i=0; i<(int) soln.size(); i++) cout << soln[i] << ' ';
	cout << endl;

}
