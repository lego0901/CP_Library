#include <bits/stdc++.h>

using namespace std;


//////// ---- Woosung Song's Source Code ---- ////////

// tree class with efficient algorithms
// NOT IMPLEMENTED!!!

#ifndef TREE_LCA_DEFINED
class tree_lca {
	private:
		typedef vector<vector<int> > edge_type;
		typedef vector<pair<int,int> > edges_type;
	
	public:
		int max_node, root, log_node;
		edge_type edge;
		edges_type edges;

		tree_lca(int max_node, int root=1) {/*{{{*/
			this->max_node = max_node;
			this->root = root;
			for (log_node=1; (1<<(log_node-1))<=max_node; log_node++);
			edge.assign(max_node+1, vector<int>());
			edges.resize(0);
		}/*}}}*/

		void add_edge(int a, int b) {/*{{{*/
			edge[a].push_back(b); edge[b].push_back(a);
			edges.emplace_back(a, b);
		}/*}}}*/
	
	private:
		vector<int> __vst_dfs;
		vector<int> __num_sib;
		vector<int> __lv_dfs;
		vector<vector<int> > __anc;
		vector<bool> __dfs_banned_vertex;
		int __vst_dfs_make_done = 0;
		stack<int> __clear_dfs;
		stack<int> __stack_dfs;

		void __make_vst_dfs(bool pre_reset=0) {
			if (!__vst_dfs_make_done || pre_reset) {
				if ((int) __vst_dfs.size() == max_node+1) {
					fill(__vst_dfs.begin(), __vst_dfs.end(), 0);
					fill(__num_sib.begin(), __num_sib.end(), 0);
					fill(__lv_dfs.begin(), __lv_dfs.end(), 0);
					fill(__dfs_banned_vertex.begin(), __dfs_banned_vertex.end(), 0);
					for (auto &v : __anc) fill(v.begin(), v.end(), 0);
				} else {
					__vst_dfs.assign(max_node+1, 0);
					__num_sib.assign(max_node+1, 0);
					__lv_dfs.assign(max_node+1, 0);
					__dfs_banned_vertex.assign(max_node+1, 0);
					__anc.assign(max_node+1, vector<int>(log_node, 0));
				}
				__vst_dfs_make_done = -1;
			}
		}

		void __clear_vst_dfs(bool post_reset=1) {
			if (post_reset) {
				while (!__clear_dfs.empty()) {
					__vst_dfs[__clear_dfs.top()] = 0;
					__clear_dfs.pop();
				}
			}
		}

		void __dfs_lca(int a=-1, int p=-1) {
			if (a == -1) a = root;
			if (p == -1) __vst_dfs_make_done = a, __lv_dfs[a] = 1; // saves the root number
			else {
				__lv_dfs[a] = __lv_dfs[p] + 1;
				__anc[a][0] = p;
				for (int i=1; i<log_node; i++)
					__anc[a][i] = __anc[__anc[a][i-1]][i-1];
			}
			__vst_dfs[a] = 1;
			__clear_dfs.push(a);
			__num_sib[a] = 1;
			for (int &b : edge[a]) {
				if (__dfs_banned_vertex[b] || b==p) continue;
				__dfs_lca(b, a);
				__num_sib[a] += __num_sib[b];
			}
		}
	
	public:
		int get_level(int a) {
			if (!__vst_dfs_make_done) { __make_vst_dfs(0); __dfs_lca(); __clear_vst_dfs(1); }
			return __lv_dfs[a];
		}

		int lca(int a, int b) {
			if (!__vst_dfs_make_done) { __make_vst_dfs(0); __dfs_lca(); __clear_vst_dfs(1); }
			if (__lv_dfs[a] > __lv_dfs[b]) swap(a, b);
			for (int i=log_node-1; i>=0; i--) {
				if (__lv_dfs[a] <= __lv_dfs[__anc[b][i]]) b = __anc[b][i];
			}
			if (a == b) return a;
			for (int i=log_node-1; i>=0; i--) {
				if (__anc[a][i] != __anc[b][i]) a = __anc[a][i], b = __anc[b][i];
			}
			return __anc[a][0];
		}
};
#endif

#define TREE_LCA_DEFINED

//////// ---- Woosung Song's Source Code ---- ////////


int main() {
	int n;
	cin >> n;
	tree_lca tr(n, 1);
	for (int i=1; i<n; i++) {
		int a, b;
		cin >> a >> b;
		tr.add_edge(a, b);
	}
	int m;
	cin >> m;
	while (m--) {
		int a, b;
		cin >> a >> b;
		cout << tr.lca(a, b) << endl;
	}
}
