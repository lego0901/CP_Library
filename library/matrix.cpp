#include <bits/stdc++.h>

using namespace std;


//////// ---- Woosung Song's Source Code ---- ////////

#ifndef MATRIX_DEFINED

bool ENABLE_MATRIX_MOD = 1;
bool MULTIPLY_MATRIX_IN_SAFE_WAY = 0;
long long MATRIX_MOD = 1e9 + 7;

template <class element_type>
class matrix {
	private:
		typedef long long ll;

	public:
		int r, c;
		vector<vector<element_type> > mat;

		matrix<element_type>() {}

		matrix<element_type>(int r, int c) {
			this->r = r, this->c = c;
			mat.assign(r, vector<element_type>(c, 0));
		}

		void print() {/*{{{*/
			cout << "{";
			for (int i=0; i<r; i++) {
				cout << "{";
				for (int j=0; j<c; j++) {
					cout << mat[i][j];
					if (j < c-1) cout << ", ";
					else cout << "}";
				}
				if (i < r-1) cout << ", ";
				else cout << "}";
			}
			cout << endl;
		}/*}}}*/

		matrix<element_type>& copy() {/*{{{*/
			matrix<element_type> *pm = new matrix<element_type>(r, c), &m = *pm;
			for (int i=0; i<r; i++) for (int j=0; j<c; j++)
				m.mat[i][j] = mat[i][j];
			return m;
		}/*}}}*/

		static matrix<element_type> I(int n) {/*{{{*/
			matrix<element_type> m(n, n);
			for (int i=0; i<n; i++) m.mat[i][i] = 1;
			return m;
		}/*}}}*/

		static matrix<element_type> O(int r, int c) {/*{{{*/
			matrix<element_type> m(r, c);
			return m;
		}/*}}}*/

		matrix<element_type> transpose() {/*{{{*/
			matrix<element_type> m(c, r);
			for (int i=0; i<r; i++) for (int j=0; j<c; j++)
				m.mat[j][i] = mat[i][j];
			return m;
		}/*}}}*/

		matrix<element_type> negative() {/*{{{*/
			matrix<element_type> m(r, c);
			for (int i=0; i<r; i++) for (int j=0; j<c; j++)
				m.mat[i][j] = -mat[i][j];
			return m;
		}/*}}}*/

		matrix<element_type> operator+(matrix<element_type> &o) {/*{{{*/
			assert(r == o.r && c == o.c);
			matrix<element_type> m(r, c);
			if (ENABLE_MATRIX_MOD) {
				for (int i=0; i<r; i++) for (int j=0; j<c; j++)
					m.mat[i][j] = __mod_clip(mat[i][j] + o.mat[i][j]);
			} else {
				for (int i=0; i<r; i++) for (int j=0; j<c; j++)
					m.mat[i][j] = mat[i][j] + o.mat[i][j];
			}
			return m;
		}/*}}}*/
		
		matrix<element_type> operator-(matrix<element_type> &o) {/*{{{*/
			assert(r == o.r && c == o.c);
			matrix<element_type> m(r, c);
			if (ENABLE_MATRIX_MOD) {
				for (int i=0; i<r; i++) for (int j=0; j<c; j++)
					m.mat[i][j] = __mod_clip(mat[i][j] - o.mat[i][j]);
			} else {
				for (int i=0; i<r; i++) for (int j=0; j<c; j++)
					m.mat[i][j] = mat[i][j] - o.mat[i][j];
			}
			return m;
		}/*}}}*/

		matrix<element_type> operator*(matrix<element_type> &o) {/*{{{*/
			assert(c == o.r);
			matrix<element_type> m(r, o.c);
			if (ENABLE_MATRIX_MOD) {
				if (MULTIPLY_MATRIX_IN_SAFE_WAY) {
					for (int i=0; i<r; i++) for (int j=0; j<o.c; j++) for (int k=0; k<c; k++)
						m.mat[i][j] = __mod_clip(m.mat[i][j] + __mod_mult(mat[i][k], o.mat[k][j]));
				} else {
					for (int i=0; i<r; i++) for (int j=0; j<o.c; j++) for (int k=0; k<c; k++)
						m.mat[i][j] = __mod_clip(m.mat[i][j] + mat[i][k] * o.mat[k][j]);
				}
			} else {
				for (int i=0; i<r; i++) for (int j=0; j<o.c; j++) for (int k=0; k<c; k++)
					m.mat[i][j] += mat[i][k] * o.mat[k][j];
			}
			return m;
		}/*}}}*/

		matrix<element_type> pow(ll p) {/*{{{*/
			assert(r == c);
			matrix<element_type> u = I(r), v = copy();
			while (p) {
				if (p&1) u = u * v;
				v = v * v;
				p >>= 1;
			}
			return u;
		}/*}}}*/

		vector<element_type> mult(vector<element_type> v) {/*{{{*/
			assert(c == (int) v.size());
			vector<element_type> u(r, 0);
			if (ENABLE_MATRIX_MOD) {
				if (MULTIPLY_MATRIX_IN_SAFE_WAY) {
					for (int i=0; i<r; i++) for (int j=0; j<c; j++)
						u[i] = __mod_clip(u[i] + __mod_mult(mat[i][j], v[j]));
				} else {
					for (int i=0; i<r; i++) for (int j=0; j<c; j++)
						u[i] = __mod_clip(u[i] + mat[i][j] * v[j]);
				}
			} else {
				for (int i=0; i<r; i++) for (int j=0; j<c; j++)
					u[i] += mat[i][j] * v[j];
			}
			return u;
		}/*}}}*/

		
	private: // simple modular functions
		static ll __gcd_two(ll a, ll b) {/*{{{*/
			return a<0ll? __gcd_two(-a, b) :
				   b<0ll? __gcd_two(a, -b) :
				   a<b? __gcd_two(b, a) : b? __gcd_two(b, a%b) : a;
		}

		static void __extended_euclid(ll a, ll b, ll &x, ll &y) {
			if (a<0) { __extended_euclid(-a, b, x, y); x = -x; }
			else if (b<0) { __extended_euclid(a, -b, x, y); y = -y; }
			else if (a<b) { __extended_euclid(b, a, y, x); }
			else if (b>0) {
				__extended_euclid(b, a%b, y, x);
				y -= a/b*x;
			} else { x = 1, y = 0; }
		}

		static ll __mod_clip(ll a) { return (a % MATRIX_MOD + MATRIX_MOD) % MATRIX_MOD; }

		static ll __mod_inverse(ll a) {
			assert(a != 0);
			ll x, y;
			__extended_euclid(a, MATRIX_MOD, x, y);
			return __mod_clip(x);
		}

		static ll __mod_mult(ll a, ll b) {
			if (b<0) return __mod_clip(-__mod_mult(a, -b));
			ll r = 0, x = a;
			while (b) {
				if (b & 1) r = (r + x) % MATRIX_MOD;
				x = (x + x) % MATRIX_MOD; b >>= 1;
			}
			return __mod_clip(r);
		}/*}}}*/
};
		
template <class element_type>/*{{{*/
ostream& operator<<(ostream &o, const matrix<element_type> &m) {
	o << "{";
	for (int i=0; i<m.r; i++) {
		o << "{";
		for (int j=0; j<m.c; j++) {
			o << m.mat[i][j];
			if (j < m.c-1) o << ", ";
			else o << "}";
		}
		if (i < m.r-1) o << ", ";
		else o << "}";
	}
	o << endl;
	return o;
}/*}}}*/
#endif

#define MATRIX_DEFINED

//////// ---- Woosung Song's Source Code ---- ////////



int main() {
	int a = 0;
	for (int it=0; it<10000000; it++) {
		matrix<int> I = matrix<int>::I(10);
		a += I.mat[0][0];
	}

#ifdef LOCAL_DEFINE/*{{{*/
	cerr << "Time elapsed: " << 1.0 * clock() / CLOCKS_PER_SEC << "s.\n";
#endif
	return 0;/*}}}*/
}
