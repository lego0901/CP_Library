#include <bits/stdc++.h>

using namespace std;


//////// ---- Woosung Song's Source Code ---- ////////

// graph(undirected) class with efficient algorithms
// supports connectivity, cycle existence, ... e.t.c.

#ifndef GRAPH_D_DEFINED
class graph_d {
	private:
		typedef vector<vector<int> > edge_type;
		typedef vector<pair<int,int> > edges_type;

	public:
		int max_node;
		int size;
		bool dense_vertices;
		set<int> vertices;
		edge_type edge;
		edges_type edges;
		vector<int> in_deg;
		
		graph_d(int max_node, bool dense_vertices=1) {/*{{{*/
			this->max_node = max_node;
			this->dense_vertices = dense_vertices;
			vertices.clear();
			if (dense_vertices)
				for (int a=1; a<=max_node; a++) add_vertex(a);
			edge.assign(max_node+1, vector<int>());
			in_deg.assign(max_node+1, 0);
			edges.resize(0);
		}/*}}}*/

		void add_vertex(int a) {/*{{{*/
			if (vertices.find(a) != vertices.end()) {
				vertices.insert(a);
				size++;
				if (size == max_node) dense_vertices = 1;
			}
		}/*}}}*/

		void add_edge(int a, int b) {/*{{{*/
			assert(a <= max_node && b <= max_node && a >= 0 && b >= 0);
			add_vertex(a); add_vertex(b);
			edge[a].push_back(b);
			in_deg[b]++;
			edges.emplace_back(a, b);
		}/*}}}*/

		graph_d& copy() {/*{{{*/
			return subgraph_with_edge(edge);
		}/*}}}*/

		graph_d& subgraph_with_edge(edge_type sub_edge) {/*{{{*/
			graph_d *psubgraph = new graph_d(max_node, dense_vertices);
			graph_d &subgraph = *psubgraph;
			if (dense_vertices) {
				for (int a=1; a<=max_node; a++)
					for (int b : sub_edge[a]) subgraph.add_edge(a, b);
			} else {
				for (const int &a : vertices)
					for (int b : sub_edge[a]) subgraph.add_edge(a, b);
			}
			return subgraph;
		}/*}}}*/

		graph_d& subgraph_with_edges(edges_type sub_edges) {/*{{{*/
			graph_d *psubgraph = new graph_d(max_node, dense_vertices);
			graph_d &subgraph = *psubgraph;
			for (pair<int,int> p : sub_edges)
				subgraph.add_edge(p.first, p.second);
			return subgraph;
		}/*}}}*/


	private:
		vector<int> __vst_dfs;
		vector<int> __low_dfs;
		bool __vst_dfs_make_done = 0;
		stack<int> __stack_dfs;
		stack<int> __clear_dfs;
		int __vst_number = 0;

		void __make_vst_dfs(bool pre_reset=0) {/*{{{*/
			if (!__vst_dfs_make_done || pre_reset) {
				if ((int) __vst_dfs.size() == max_node+1) {
					fill(__vst_dfs.begin(), __vst_dfs.end(), 0);
					fill(__low_dfs.begin(), __low_dfs.end(), 0);
				} else {
					__vst_dfs.assign(max_node+1, 0);
					__low_dfs.assign(max_node+1, 0);
				}
				__vst_number = 0;
				__vst_dfs_make_done = 1;
			}
		}/*}}}*/

		void __clear_vst_dfs(bool post_reset=1) {/*{{{*/
			if (post_reset) {
				while (!__clear_dfs.empty()) {
					__vst_dfs[__clear_dfs.top()] = 0;
					__low_dfs[__clear_dfs.top()] = 0;
					__clear_dfs.pop();
				}
			}
		}/*}}}*/

		int __dfs_component_get_size_recursive(int a) {/*{{{*/
			__vst_dfs[a] = 1;
			__clear_dfs.push(a);
			int r = 1;
			for (int &b : edge[a]) {
				if (__vst_dfs[b]) continue;
				r += __dfs_component_get_size_recursive(b);
			}
			return r;
		}/*}}}*/

		bool __dfs_is_reachable(int a, int b) {/*{{{*/
			__vst_dfs[a] = 1;
			__clear_dfs.push(a);
			if (a == b) return 1;
			for (int &c : edge[a]) {
				if (__vst_dfs[c]) continue;
				if (__dfs_is_reachable(c, b)) return 1;
			}
			return 0;
		}/*}}}*/

		bool __dfs_reversed_path(int a, int b, vector<int> &rp) {/*{{{*/
			__vst_dfs[a] = 1;
			__clear_dfs.push(a);
			if (a == b) { rp.push_back(a); return 1; }
			for (int &c : edge[a]) {
				if (__vst_dfs[c]) continue;
				if (__dfs_reversed_path(c, b, rp)) {
					rp.push_back(a); return 1;
				}
			}
			return 0;
		}/*}}}*/

		bool __dfs_cycle_detection(int a, vector<int> &cycle, int p=-1) {/*{{{*/
			__vst_dfs[a] = 1;
			__clear_dfs.push(a);
			__stack_dfs.push(a);
			for (int &b : edge[a]) {
				if (b == p) continue;
				if (__vst_dfs[b] == 1) {
					while (!__stack_dfs.empty() && __stack_dfs.top() != b) {
						cycle.push_back(__stack_dfs.top());
						__stack_dfs.pop();
					}
					cycle.push_back(b);
					reverse(cycle.begin(), cycle.end());
					return 1;
				} else if (__dfs_cycle_detection(b, cycle, a))
					return 1;
			}
			__vst_dfs[a] = 2;
			__stack_dfs.pop();
			return 0;
		}/*}}}*/

		bool __dfs_cycle_detection_containing_vertex(int target, int a, vector<int> &cycle, int p=-1) {/*{{{*/
			__vst_dfs[a] = ++__vst_number;
			__clear_dfs.push(a);
			__stack_dfs.push(a);
			for (int &b : edge[a]) {
				if (b == p) continue;
				if (__vst_dfs[b]) {
					if (__vst_dfs[b] > 0 &&__vst_dfs[b] <= __vst_dfs[target]) {
						while (!__stack_dfs.empty() && __stack_dfs.top() != b) {
							cycle.push_back(__stack_dfs.top());
							__stack_dfs.pop();
						}
						cycle.push_back(b);
						reverse(cycle.begin(), cycle.end());
						return 1;
					}
				} else if (__dfs_cycle_detection_containing_vertex(target, b, cycle, a))
					return 1;
			}
			__vst_dfs[a] = -__vst_dfs[a];
			__stack_dfs.pop();
			return 0;
		}/*}}}*/

		bool __has_cycle_connected_to_vertex_a(int a, vector<int> &cycle) {/*{{{*/
			return __dfs_cycle_detection(a, cycle);
		}/*}}}*/

		bool __has_cycle_containing_to_vertex_a(int a, vector<int> &cycle) {/*{{{*/
			return __dfs_cycle_detection_containing_vertex(a, a, cycle);
		}/*}}}*/

		int __dfs_tarjan(int a, vector<vector<int> > &rsccs) {/*{{{*/
			__vst_dfs[a] = __low_dfs[a] = ++__vst_number;
			__clear_dfs.push(a);
			__stack_dfs.push(a);
			for (int &b : edge[a]) {
				if (__vst_dfs[b]) {
					if (__vst_dfs[b] > 0) __low_dfs[a] = min(__low_dfs[a], __vst_dfs[b]);
				} else {
					__dfs_tarjan(b, rsccs);
					__low_dfs[a] = min(__low_dfs[a], __low_dfs[b]);
				}
			}
			if (__vst_dfs[a] == __low_dfs[a]) {
				rsccs.emplace_back();
				while (1) {
					int b = __stack_dfs.top(); __stack_dfs.pop();
					__vst_dfs[b] = -__vst_dfs[b];
					rsccs.back().push_back(b);
					if (b == a) break;
				}
			}
			return __low_dfs[a];
		}/*}}}*/
	
	public:
		int degree(int a) {/*{{{*/
			assert(1 <= a && a <= max_node);
			return (int) edge[a].size();
		}/*}}}*/

		int in_degree(int a) {/*{{{*/
			assert(1 <= a && a <= max_node);
			return in_deg[a];
		}/*}}}*/

		vector<vector<int> >& strongly_connected_components(bool pre_reset=0, bool post_reset=1) {/*{{{*/
			__make_vst_dfs(pre_reset);
			vector<vector<int> > *psccs = new vector<vector<int> >();
			vector<vector<int> > &sccs = *psccs;
			for (int n=1; n<=max_node; n++) {
				if (!dense_vertices && vertices.find(n) == vertices.end()) continue;
				if (__vst_dfs[n]) continue;
				__dfs_tarjan(n, sccs);
			}
			reverse(sccs.begin(), sccs.end());
			__clear_vst_dfs(post_reset);
			return sccs;
		}/*}}}*/

		bool is_strongly_connected(bool pre_reset=0, bool post_reset=1) {/*{{{*/
			vector<vector<int> > sccs = strongly_connected_components(pre_reset, post_reset);
			bool r = (int) sccs.size() == 1;
			for (auto &c : sccs) c.clear();
			sccs.clear();
			return r;
		}/*}}}*/

		bool is_reachable(int a, int b, bool pre_reset=0, bool post_reset=1) {/*{{{*/
			__make_vst_dfs(pre_reset);
			bool r = __dfs_is_reachable(a, b);
			__clear_vst_dfs(post_reset);
			return r;
		}/*}}}*/

		vector<int>& reachable_from(int a, bool pre_reset=0, bool post_reset=1) {/*{{{*/
			__make_vst_dfs(pre_reset);
			__dfs_component_get_size_recursive(a);
			vector<int> *pr = new vector<int>(), &r = *pr;
			for (int i=1; i<=max_node; i++)
				if (__vst_dfs[i]) r.push_back(i);
			__clear_vst_dfs(post_reset);
			return r;
		}/*}}}*/

		vector<int>& path(int a, int b, bool pre_reset=0, bool post_reset=1) {/*{{{*/
			__make_vst_dfs(pre_reset);
			vector<int> *pp = new vector<int>(), &p = *pp;
			__dfs_reversed_path(a, b, p); reverse(p.begin(), p.end());
			__clear_vst_dfs(post_reset);
			return p;
		}/*}}}*/

		int hub(bool pre_reset=0, bool post_reset=1) {/*{{{*/
			vector<vector<int> > sccs = strongly_connected_components(pre_reset, 1);
			int a = sccs.front().front();
			if (size > __dfs_component_get_size_recursive(a)) a = 0;
			__clear_vst_dfs(post_reset);
			return a;
		}/*}}}*/

		vector<int>& find_cycle(int a=-1, bool pre_reset=0, bool post_reset=1) {/*{{{*/
			__make_vst_dfs(pre_reset);
			vector<int> *pcycle = new vector<int>();
			vector<int> &cycle = *pcycle;
			if (a == -1) {
				for (a=1; a<=max_node; a++) {
					if (!dense_vertices && vertices.find(a) == vertices.end()) continue;
					if (__vst_dfs[a]) continue;
					if (__has_cycle_connected_to_vertex_a(a, cycle)) break;
				}
			} else
				__has_cycle_containing_to_vertex_a(a, cycle);
			__clear_vst_dfs(post_reset);
			return cycle;
		}/*}}}*/

		bool has_cycle(int a=-1, bool pre_reset=0, bool post_reset=1) {/*{{{*/
			__make_vst_dfs(pre_reset);
			vector<int> cycle = find_cycle(a);
			__clear_vst_dfs(post_reset);
			return (int) cycle.size() != 0;
		}/*}}}*/

		edges_type& bfs_tree_edges(int root=-1, bool pre_reset=0, bool post_reset=1) {/*{{{*/
			assert(is_strongly_connected(pre_reset, 1));
			if (root != -1) assert(__dfs_component_get_size_recursive(root) == size);
			else root = hub(0, 1);
			queue<int> bq; bq.push(root); __vst_dfs[root] = 1;
			edges_type *pbfs = new edges_type(), &bfs = *pbfs;
			while (!bq.empty()) {
				int a = bq.front(); bq.pop();
				__clear_dfs.push(a);
				for (int &b : edge[a]) {
					if (!__vst_dfs[b]) {
						__vst_dfs[b] = 1;
						bfs.emplace_back(min(a, b), max(a, b));
						bq.push(b);
					}
				}
			}
			__clear_vst_dfs(post_reset);
			return bfs;
		}/*}}}*/

		graph_d& bfs_tree_graph_d(int root=-1, bool pre_reset=0, bool post_reset=1) {/*{{{*/
			return subgraph_with_edges(bfs_tree_edges(root, pre_reset, post_reset));
		}/*}}}*/

};
#endif

#define GRAPH_D_DEFINED

//////// ---- Woosung Song's Source Code ---- ////////

int main() {
	graph_d gr(10);
	return 0;
}
