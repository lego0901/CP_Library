#include <bits/stdc++.h>

using namespace std;

// should be verified (not yet!!)

//////// ---- Woosung Song's Source Code ---- ////////
#ifndef DINIC_DEFINED
template <class ctype>
class dinic {
	private:
		typedef tuple<int,int,ctype> etype;
		vector<int> lv, wo;
		queue<int> q;

		bool bfs(int src, int dst) {
			lv.assign(max_node+1, 0);
			lv[src] = 1;
			q.push(src);
			while (!q.empty()) {
				int a = q.front(); q.pop();
				for (auto &e : edge[a]) {
					int b, _r; ctype c; tie(b, _r, c) = e;
					if (!lv[b] && c > 0) {
						lv[b] = lv[a] + 1;
						q.push(b);
					}
				}
			}
			return lv[dst] > 0;
		}

		ctype dfs(int a, int dst, ctype curc) {
			if (a == dst) return curc;
			for (int &i=wo[a]; i<(int)edge[a].size(); i++) {
				int b = get<0>(edge[a][i]), r = get<1>(edge[a][i]);
				ctype &c = get<2>(edge[a][i]);
				if (lv[b] == lv[a] + 1 && c > 0) {
					int cap = dfs(b, dst, min(curc, c));
					if (cap > 0) {
						c -= cap;
						get<2>(edge[b][r]) += cap;
						return cap;
					}
				}
			}
			return 0;
		}
	
	public:
		int max_node;
		vector<vector<etype> > edge;

		dinic() {}
		dinic(int max_node) {
			this->max_node = max_node;
			edge.assign(max_node+1, vector<etype>());
			lv.assign(max_node+1, 0);
			wo.assign(max_node+1, 0);
		}

		void add_edge(int a, int b, ctype c) {
			edge[a].emplace_back(b, (int) edge[b].size(), c);
			edge[b].emplace_back(a, (int) edge[a].size()-1, 0);
			//edge[b].emplace_back(a, (int) edge[a].size()-1, -c);
		}

		ctype get_dinic(int src, int dst) {
			ctype f = 0;
			while (bfs(src, dst)) {
				wo.assign(max_node+1, 0);
				while (1) {
					int curf = dfs(src, dst, numeric_limits<ctype>::max());
					if (!curf) break;
					f += curf;
				}
			}
			return f;
		}
};
#endif

#define DINIC_DEFINED
//////// ---- Woosung Song's Source Code ---- ////////

int main() {

}
