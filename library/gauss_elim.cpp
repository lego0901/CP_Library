#include <bits/stdc++.h>

using namespace std;

//////// ---- Woosung Song's Source Code ---- ////////
#ifndef GAUSS_ELIM_DEFINED
namespace gauss_elim {
	typedef long double dtype;
	// use double or float for speed

	const dtype EPS = 1e-6;

	inline bool _eq(dtype x, dtype y) {
		return -EPS <= x-y && x-y <= EPS;
	}

	void get_gauss_elim(vector<vector<dtype> > &v) {
		int n = v.size(), m = v[0].size();
		vector<vector<dtype>*> w;
		vector<int> first0;
		
		for (int i=0; i<n; i++)
			w.push_back(&v[i]);

		int r = 0;
		for (int j=0; j<m-1; j++) {
			int i;
			for (i=r; i<n; i++) if (!_eq((*w[i])[j], 0)) break;
			if (i == n) continue;
			swap(w[r], w[i]);
			first0.push_back(j);

			dtype inv = 1 / (*w[r])[j];
			for (int k=0; k<m; k++)
				(*w[r])[k] *= inv;

			for (i=r+1; i<n; i++) {
				dtype prod = (*w[i])[j];
				for (int k=0; k<m; k++)
					(*w[i])[k] -= prod * (*w[r])[k];
			}
			r++;
		}
		while ((int) first0.size() < n) first0.push_back(m); // zero row

		for (int r=n-1; r>=0; r--) {
			int j = first0[r];
			if (j == m) continue;

			for (int i=0; i<r; i++) {
				dtype prod = (*w[i])[j];
				for (int k=0; k<m; k++)
					(*w[i])[k] -= prod * (*w[r])[k];
			}
		}

		vector<vector<dtype> > u;
		for (int i=0; i<n; i++) {
			u.push_back(*w[i]);
		}
		swap(v, u);
	}

	vector<dtype> one_solution(vector<vector<dtype> > &v, bool run_gauss_elim=1) {
		if (run_gauss_elim) get_gauss_elim(v);
		int n = v.size(), m = v[0].size();
		vector<dtype> soln(m-1, 0);

		vector<int> first0;
		for (int i=0; i<n; i++) {
			for (int j=0; j+1<m; j++) {
				if (_eq(v[i][j], 1)) {
					first0.push_back(j);
					break;
				}
			}
			if ((int) first0.size() == i) first0.push_back(m); // zero row
		}

		for (int r=n-1; r>=0; r--) {
			int j = first0[r];
			if (j == m) {
				if (!_eq(v[r][m-1], 0)) {
					soln.clear(); // no solution
					return soln;
				}
			} else {
				soln[j] = v[r][m-1];
				for (int k=j+1; k+1<m; k++) {
					soln[j] -= soln[k] * v[r][k];
				}
			}
		}

		return soln;
	}

	bool invertible(vector<vector<dtype> > &v, bool run_gauss_elim=1) {
		if (run_gauss_elim) get_gauss_elim(v);
		int n = v.size(), m = v[0].size();
		if (m != n+1) return 0;

		for (int i=0; i<n; i++) {
			int flag = 1;
			for (int j=0; j+1<m; j++) {
				if (_eq(v[i][j], 1)) { flag = 0; break; }
			}
			if (flag) return 0;
		}
		return 1;
	}
};
#endif

#define GAUSS_ELIM_DEFINED
//////// ---- Woosung Song's Source Code ---- ////////


int main() {
	typedef long double ld;
	vector<vector<ld> > v;
	int n, m;
	cin >> n >> m;
	for (int i=0; i<n; i++) {
		vector<ld> y;
		for (int j=0; j<=m; j++) {
			ld x;
			cin >> x;
			y.push_back(x);
		}
		v.push_back(y);
	}
	gauss_elim::get_gauss_elim(v);
	for (int i=0; i<n; i++) {
		for (int j=0; j<=m; j++) {
			cout << v[i][j] << ' ';
		}
		cout << endl;
	}

	vector<ld> soln = gauss_elim::one_solution(v, 0);
	for (int i=0; i<(int) soln.size(); i++) cout << soln[i] << ' ';
	cout << endl;

}
