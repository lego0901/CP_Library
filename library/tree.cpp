#include <bits/stdc++.h>

using namespace std;


//////// ---- Woosung Song's Source Code ---- ////////

// tree class with efficient algorithms
// supports

#ifndef TREE_DEFINED
class tree {
	private:
		typedef vector<vector<int> > edge_type;
		typedef vector<pair<int,int> > edges_type;
	
	public:
		int max_node, root;
		edge_type edge;
		edges_type edges;

		tree(int max_node, int root=1) {
			this->max_node = max_node;
			this->root = root;
			edge.assign(max_node+1, vector<int>());
			edges.resize(0);
		}

		void add_edge(int a, int b) {
			edge[a].push_back(b); edge[b].push_back(a);
			edges.emplace_back(a, b);
		}

		bool is_tree() {
			return (int) edges.size() == max_node-1 && __dfs_num_sib() == max_node;
		}
	
	private:
		vector<int> __vst_dfs;
		vector<int> __num_sib;
		vector<bool> __dfs_banned_vertex;
		int __vst_dfs_make_done = 0;
		stack<int> __clear_dfs;
		stack<int> __stack_dfs;

		void __make_vst_dfs(bool pre_reset=0) {
			if (!__vst_dfs_make_done || pre_reset) {
				if ((int) __vst_dfs.size() == max_node+1) {
					fill(__vst_dfs.begin(), __vst_dfs.end(), 0);
					fill(__num_sib.begin(), __num_sib.end(), 0);
					fill(__dfs_banned_vertex.begin(), __dfs_banned_vertex.end(), 0);
				} else {
					__vst_dfs.assign(max_node+1, 0);
					__num_sib.assign(max_node+1, 0);
					__dfs_banned_vertex.assign(max_node+1, 0);
				}
				__vst_dfs_make_done = -1;
			}
		}

		void __clear_vst_dfs(bool post_reset=1) {
			if (post_reset) {
				while (!__clear_dfs.empty()) {
					__vst_dfs[__clear_dfs.top()] = 0;
					__num_sib[__clear_dfs.top()] = 0;
					__clear_dfs.pop();
				}
			}
		}

		int __dfs_num_sib(int a=-1, int p=-1) {
			if (a == -1) a = root;
			if (p == -1) __vst_dfs_make_done = a; // saves the root number
			__vst_dfs[a] = 1;
			__clear_dfs.push(a);
			__num_sib[a] = 1;
			for (int &b : edge[a]) {
				if (__dfs_banned_vertex[b] || b==p) continue;
				__num_sib[a] += __dfs_num_sib(b, a);
			}
			return __num_sib[a];
		}

		int __dfs_find_centroid(int a, int size=-1, int p=-1) {
			if (size == -1) size = max_node;
			for (int &b : edge[a]) {
				if (b == p || __dfs_banned_vertex[b]) continue;
				if (__num_sib[b] * 2 > size)
					return __dfs_find_centroid(b, size, a);
			}
			return a;
		}
	
	public:
		int centroid(int a=-1, bool pre_reset=0, bool post_reset=1) {
			if (a == -1) a = root;
			__make_vst_dfs(pre_reset);
			int size = (__num_sib[a] == 0 || __vst_dfs_make_done != a)? __dfs_num_sib(a) : __num_sib[a];
			int b = __dfs_find_centroid(a, size);
			__clear_vst_dfs(post_reset);
			return b;
		}

		/*
		// One possible way of tree-DP
		int tree_dp_dfs(vector<int>& dp, int a=-1) {
			if (a == -1) a = root;
			a = centroid(a);  // 자동으로 vst 배열 갱신하고 초기화함
			int &d = dp[a] = 1;
			__dfs_banned_vertex[a] = 1;
			for (int &b : edge[a]) {
				if (__dfs_banned_vertex[b]) continue;
				d += tree_dp_dfs(dp, b);
			}
			return d;
		}
		vector<int>& tree_dp() {
			vector<int> *pdp = new vector<int>(), &dp = *pdp;
			tree_dp_dfs(dp);
			return dp;
		}
		*/
		


};
#endif

#define TREE_DEFINED

//////// ---- Woosung Song's Source Code ---- ////////
