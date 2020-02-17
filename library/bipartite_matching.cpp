#include <bits/stdc++.h>

using namespace std;

//////// ---- Woosung Song's Source Code ---- ////////
#ifndef BIPARTITE_MATCHING_DEFINED
namespace bipartite_matching {
	vector<int> __vst, __a, __b;
	vector<bool> __cover_a, __cover_b;
	int __i;

	bool __dfs(int a, vector<vector<int> > &edge) {
		__vst[a] = __i;
		for (auto &b : edge[a]) {
			if (!__b[b] || (__vst[__b[b]]^__i && __dfs(__b[b], edge))) {
				__a[a] = b, __b[b] = a;
				return 1;
			}
		}
		return 0;
	}

	void __dfs_cover(int a, vector<vector<int> > &edge) {
		__vst[a] = 1;
		for (auto &b : edge[a]) {
			__cover_b[b] = 1;
			if (!__vst[__b[b]])
				__dfs_cover(__b[b], edge);
		}
	}

	vector<pair<int,int> > matching(int n, int m, vector<vector<int> > &edge) {
		__vst.assign(n+1, 0);
		__a.assign(n+1, 0);
		__b.assign(m+1, 0);

		for (__i=1; __i<=n; __i++) __dfs(__i, edge);

		vector<pair<int,int> > v;
		for (int i=1; i<=n; i++) if (__a[i]) v.emplace_back(i, __a[i]);
		return v;
	}

	int matching_size(int n, int m, vector<vector<int> > &edge) {
		__vst.assign(n+1, 0);
		__a.assign(n+1, 0);
		__b.assign(m+1, 0);

		int ans = 0;
		for (__i=1; __i<=n; __i++) ans += __dfs(__i, edge);
		return ans;
	}

	pair<vector<int>,vector<int>> vertex_cover(int n, int m, vector<vector<int> > &edge) {
		matching_size(n, m, edge);
		__vst.assign(n+1, 0);
		__cover_a.assign(n+1, 0);
		__cover_b.assign(m+1, 0);

		for (int a=1; a<=n; a++) {
			if (__a[a] == 0)
				__dfs_cover(a, edge);
		}

		for (int a=1; a<=n; a++) {
			if (__a[a] != 0 && !__vst[a])
				__cover_a[a] = 1;
		}

		vector<int> L, R;
		for (int a=1; a<=n; a++) if (__cover_a[a]) L.push_back(a);
		for (int b=1; b<=m; b++) if (__cover_b[b]) R.push_back(b);
		return make_pair(L, R);
	}
};
#endif

#define BIPARTITE_MATCHING_DEFINED
//////// ---- Woosung Song's Source Code ---- ////////

int main() {
	int n, m;
	cin >> n >> m;
	vector<vector<int> > edge(n+1, vector<int>());
	for (int a=1; a<=n; a++) {
		int cnt; cin >> cnt;
		while (cnt--) {
			int b; cin >> b;
			edge[a].push_back(b);
		}
	}

	vector<pair<int,int> > match = bipartite_matching::matching(n, m, edge);

	cout << ((int) match.size()) << endl;
	/*
	for (auto &p : match) {
		cout << p.first << ' ' << p.second << endl;
	}
	*/
}
