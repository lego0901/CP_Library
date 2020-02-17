#include <bits/stdc++.h>

using namespace std;

//////// ---- Woosung Song's Source Code ---- ////////
#ifndef LIS_DEFINED
namespace lis {
	template <class elt>
	vector<elt> get(vector<elt> &a) {
		const elt LIS_INF = numeric_limits<elt>::max();
		int n = (int) a.size();
		vector<elt> d(n, 0), p(n, 0);
		for (int i=0; i<n; i++) {
			d[i] = LIS_INF;
			d[p[i] = (int) (lower_bound(d.begin(), d.begin()+i, a[i]) - d.begin())] = a[i];
		}
		int s = (int) (lower_bound(d.begin(), d.end(), LIS_INF) - d.begin());

		vector<elt> v;
		for (int i=n-1, j=s-1; j>=0; i--) {
			if (p[i] == j) v.push_back(a[i]), j--;
		}
		reverse(v.begin(), v.end());
		d.clear(), p.clear();
		return v;
	}

	template <class elt>
	int size(vector<elt> &a) {
		vector<elt> v = get(a);
		int s = v.size();
		v.clear();
		return s;
	}
};
#endif

#define LIS_DEFINED
//////// ---- Woosung Song's Source Code ---- ////////

int main() {
	vector<int> a;
	int n; cin >> n;
	for (int i=0; i<n; i++) {
		int x; cin >> x; a.push_back(x);
	}
	cout << lis::size(a) << endl;
	vector<int> g = lis::get(a);
	for (auto &x : g) cout << x << ' ';
	cout << endl;
}
