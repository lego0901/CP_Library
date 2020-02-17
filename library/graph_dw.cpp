#include <bits/stdc++.h>

using namespace std;

//////// ---- Woosung Song's Source Code ---- ////////

// graph(undirected) class with efficient algorithms
// supports connectivity, cycle existence, ... e.t.c.

#ifndef GRAPH_DW_DEFINED
template <class weight_type>
class graph_dw {
	private:
		typedef pair<int,weight_type> intw;
		typedef tuple<int,int,weight_type> iintw;
		typedef vector<vector<intw> > edge_type;
		typedef vector<vector<iintw> > residual_edge_type;
		typedef vector<iintw> edges_type;

		typedef weight_type (*weight_op) (weight_type, weight_type);
		map<pair<int,int>, weight_type> edge_to_weight;

	public:
		int max_node;
		int size;
		weight_type total_weight;
		bool dense_vertices;
		set<int> vertices;
		edge_type edge;
		edges_type edges;
		vector<int> in_deg;
		residual_edge_type res_edge;

		weight_op add_weight_op = [](weight_type x, weight_type y) { return x + y; };
		weight_op min_weight_op = [](weight_type x, weight_type y) { return min(x, y); };
		weight_op max_weight_op = [](weight_type x, weight_type y) { return max(x, y); };
		weight_type add_weight_id = 0;
		weight_type min_weight_id = numeric_limits<weight_type>::max();
		weight_type max_weight_id = numeric_limits<weight_type>::min();
		
		graph_dw(int max_node, bool dense_vertices=1) {/*{{{*/
			this->max_node = max_node;
			this->dense_vertices = dense_vertices;
			total_weight = 0;
			vertices.clear();
			if (dense_vertices)
				for (int a=1; a<=max_node; a++) add_vertex(a);
			edge.assign(max_node+1, vector<intw>());
			in_deg.assign(max_node+1, 0);
			res_edge.assign(max_node+1, vector<iintw>());
			edges.resize(0);
		}/*}}}*/

		void add_vertex(int a) {/*{{{*/
			if (vertices.find(a) != vertices.end()) {
				vertices.insert(a);
				size++;
				if (size == max_node) dense_vertices = 1;
			}
		}/*}}}*/

		void add_edge(int a, int b, weight_type w) {/*{{{*/
			assert(a <= max_node && b <= max_node && a >= 0 && b >= 0);
			add_vertex(a); add_vertex(b);
			edge_to_weight[{a,b}] = w;
			res_edge[a].emplace_back(b, (int) res_edge[b].size(), w);
			res_edge[b].emplace_back(a, (int) res_edge[a].size()-1, 0);
			edge[a].emplace_back(b, w);
			in_deg[b]++;
			total_weight += w;
			edges.emplace_back(a, b, w);
		}/*}}}*/

		void add_edge(int a, intw bw) {/*{{{*/
			int b; weight_type w; tie(b, w) = bw;
			add_edge(a, b, w);
		}/*}}}*/

		void add_edge(tuple<int,int,weight_type> abw) {/*{{{*/
			int a, b; weight_type w; tie(a, b, w) = abw;
			add_edge(a, b, w);
		}/*}}}*/

		graph_dw& copy() {/*{{{*/
			return subgraph_with_edge(edge);
		}/*}}}*/

		graph_dw& subgraph_with_edge(edge_type sub_edge) {/*{{{*/
			graph_dw *psubgraph = new graph_dw(max_node, dense_vertices);
			graph_dw &subgraph = *psubgraph;
			if (dense_vertices) {
				for (int a=1; a<=max_node; a++)
					for (intw bw : sub_edge[a]) subgraph.add_edge(a, bw);
			} else {
				for (const int &a : vertices)
					for (intw bw : sub_edge[a]) subgraph.add_edge(a, bw);
			}
			return subgraph;
		}/*}}}*/

		graph_dw& subgraph_with_edges(edges_type sub_edges) {/*{{{*/
			graph_dw *psubgraph = new graph_dw(max_node, dense_vertices);
			graph_dw &subgraph = *psubgraph;
			for (iintw p : sub_edges)
				subgraph.add_edge(p);
			return subgraph;
		}/*}}}*/


	private:
		vector<int> __vst_dfs;
		vector<int> __low_dfs;
		bool __vst_dfs_make_done = 0;
		stack<int> __stack_dfs;
		stack<int> __clear_dfs;
		int __vst_number = 0;
		
		int __dijkstra_done = 0;
		int __bellman_ford_done = 0;
		int __floyd_warshall_done = 0;
		vector<weight_type> __dijkstra_dist;
		vector<weight_type> __bellman_ford_dist;
		vector<vector<weight_type> > __floyd_warshall_dist;
		vector<int> __dijkstra_path_prev;
		vector<int> __bellman_ford_path_prev;
		vector<vector<int> > __floyd_warshall_path_prev;

		vector<int> __lv_flow, __w_flow;
		bool __flow_make_done = 0;
		

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

		void __make_flow(bool pre_reset=0) {/*{{{*/
			if (!__flow_make_done || pre_reset) {
				if ((int) __lv_flow.size() == max_node + 1) {
					fill(__lv_flow.begin(), __lv_flow.end(), 0);
					fill(__w_flow.begin(), __w_flow.end(), 0);
				} else {
					__lv_flow.assign(max_node+1, 0);
					__w_flow.assign(max_node+1, 0);
				}
				__flow_make_done = 1;
			}
		}/*}}}*/

		void __clear_flow(bool post_reset=1) {/*{{{*/
			if (post_reset) {
				for (int a=1; a<=max_node; a++) res_edge[a].clear();
				for (auto &e : edges) {
					int a, b; weight_type w;
					tie(a, b, w) = e;
					res_edge[a].emplace_back(b, (int) res_edge[b].size(), w);
					res_edge[b].emplace_back(a, (int) res_edge[a].size()-1, 0);
				}
			}
		}/*}}}*/

		int __dfs_component_get_size_recursive(int a) {/*{{{*/
			__vst_dfs[a] = 1;
			__clear_dfs.push(a);
			int r = 1;
			for (intw &bw : edge[a]) {
				int b = bw.first;
				if (__vst_dfs[b]) continue;
				r += __dfs_component_get_size_recursive(b);
			}
			return r;
		}/*}}}*/

		bool __dfs_is_reachable(int a, int b) {/*{{{*/
			__vst_dfs[a] = 1;
			__clear_dfs.push(a);
			if (a == b) return 1;
			for (intw &cw : edge[a]) {
				int c = cw.first;
				if (__vst_dfs[c]) continue;
				if (__dfs_is_reachable(c, b)) return 1;
			}
			return 0;
		}/*}}}*/

		bool __dfs_reversed_path(int a, int b, vector<int> &rp) {/*{{{*/
			__vst_dfs[a] = 1;
			__clear_dfs.push(a);
			if (a == b) { rp.push_back(a); return 1; }
			for (intw &cw : edge[a]) {
				int c = cw.first;
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
			for (intw &bw : edge[a]) {
				int b = bw.first;
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
			for (intw &bw : edge[a]) {
				int b = bw.first;
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
			for (intw &bw : edge[a]) {
				int b = bw.first;
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

		vector<weight_type>& __bellman_ford_one_iteration(vector<weight_type> &d) {/*{{{*/
			vector<weight_type> *pr = new vector<weight_type>(max_node+1), &r = *pr;
			for (int i=0; i<=max_node; i++) r[i] = d[i];
			for (iintw &e : edges) {
				int a, b; weight_type w; tie(a, b, w) = e;
				if (d[a] != min_weight_id) {
					if (!__vst_dfs[a]) { __vst_dfs[a] = 1; __clear_dfs.push(a); }
					if (r[b] > d[a] + w) {
						r[b] = d[a] + w; __bellman_ford_path_prev[b] = a;
						if (!__vst_dfs[b]) { __vst_dfs[b] = 1; __clear_dfs.push(b); }
					}
				}
			}
			return r;
		}/*}}}*/

		bool __bfs_flow(int src, int dst) {/*{{{*/
			fill(__lv_flow.begin(), __lv_flow.end(), 0);
			__lv_flow[src] = 1;
			queue<int> q; q.push(src);
			while (!q.empty()) {
				int a = q.front(); q.pop();
				for (iintw &e : res_edge[a]) {
					int b, r; weight_type c; tie(b, r, c) = e;
					if (__lv_flow[b] == 0 && c > 0) {
						__lv_flow[b] = __lv_flow[a] + 1;
						q.push(b);
					}
				}
			}
			return __lv_flow[dst] != 0;
		}/*}}}*/

		weight_type __dfs_flow(int a, int dst, weight_type cur) {/*{{{*/
			if (a == dst) return cur;
			for (int &i=__w_flow[a]; i<(int) res_edge[a].size(); i++) {
				iintw &e = res_edge[a][i];
				int &b = get<0>(e), &r = get<1>(e); weight_type &c = get<2>(e);
				if (__lv_flow[a] + 1 == __lv_flow[b] && c > 0) {
					weight_type cap = __dfs_flow(b, dst, min(cur, c));
					if (cap > 0) {
						c -= cap;
						get<2>(res_edge[b][r]) += cap;
						return cap;
					}
				}
			}
			return 0;
		}/*}}}*/

		void __dfs_residual_reachable(int a) {/*{{{*/
			__vst_dfs[a] = 1;
			__clear_dfs.push(a);
			for (iintw &brw : res_edge[a]) {
				int b = get<0>(brw); weight_type w = get<2>(brw);
				if (w == 0 || __vst_dfs[b]) continue;
				__dfs_residual_reachable(b);
			}
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

		int hub(bool pre_reset=0, bool post_reset=1) {/*{{{*/
			vector<vector<int> > sccs = strongly_connected_components(pre_reset, 1);
			int a = sccs.front().front();
			if (size > __dfs_component_get_size_recursive(a)) a = 0;
			__clear_vst_dfs(post_reset);
			return a;
		}/*}}}*/

		vector<int>& path(int a, int b, bool pre_reset=0, bool post_reset=1) {/*{{{*/
			__make_vst_dfs(pre_reset);
			vector<int> *pp = new vector<int>(), &p = *pp;
			__dfs_reversed_path(a, b, p); reverse(p.begin(), p.end());
			__clear_vst_dfs(post_reset);
			return p;
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

		edges_type& bfs_tree_edges(int root=0, bool pre_reset=0, bool post_reset=1) {/*{{{*/
			assert(is_strongly_connected(pre_reset, 1));
			if (root != 0) assert(__dfs_component_get_size_recursive(root) == size);
			else root = hub(0, 1);
			queue<int> bq; bq.push(root); __vst_dfs[root] = 1;
			edges_type *pbfs = new edges_type(), &bfs = *pbfs;
			while (!bq.empty()) {
				int a = bq.front(); bq.pop();
				__clear_dfs.push(a);
				for (intw &bw : edge[a]) {
					int b = bw.first;
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

		graph_dw& bfs_tree_graph_dw(int root=0, bool pre_reset=0, bool post_reset=1) {/*{{{*/
			return subgraph_with_edges(bfs_tree_edges(root, pre_reset, post_reset));
		}/*}}}*/
	
		vector<weight_type>& dijkstra(int a, weight_op add_op=0, weight_type add_id=0,/*{{{*/
				weight_op min_op=0, weight_type min_id=0, bool pre_reset=0, bool post_reset=1) {
			// works fine for non-negative cycles
			if (a == __dijkstra_done) return __dijkstra_dist;
			if (!add_op) add_op = add_weight_op, add_id = add_weight_id;
			if (!min_op) min_op = min_weight_op, min_id = min_weight_id;
			__make_vst_dfs(pre_reset);
			vector<weight_type> *pdist = new vector<weight_type>(max_node+1, min_id), &dist = *pdist;
			__dijkstra_path_prev.assign(max_node+1, 0);
			if (min_op(0, 1) == 0) {
				priority_queue<intw, vector<intw>, greater<intw> > pq; pq.emplace(a, add_id);
				while (!pq.empty()) {
					int a; weight_type d; tie(a, d) = pq.top(); pq.pop();
					if (__vst_dfs[a]) continue;
					__vst_dfs[a] = 1; dist[a] = d;
					for (intw bw : edge[a]) {
						int b; weight_type w; tie(b, w) = bw;
						weight_type newd = add_op(d, w);
						if (!__vst_dfs[b] && min_op(dist[b], newd) == newd) {
							pq.emplace(b, newd);
							dist[b] = newd;
							__dijkstra_path_prev[b] = a;
						}
					}
				}
			} else {
				priority_queue<intw, vector<intw>, less<intw> > pq; pq.emplace(a, add_id);
				while (!pq.empty()) {
					int a; weight_type d; tie(a, d) = pq.top(); pq.pop();
					if (__vst_dfs[a]) continue;
					__vst_dfs[a] = 1; dist[a] = d;
					for (intw bw : edge[a]) {
						int b; weight_type w; tie(b, w) = bw;
						weight_type newd = add_op(d, w);
						if (!__vst_dfs[b] && min_op(dist[b], newd) == newd) {
							pq.emplace(b, newd);
							dist[b] = newd;
							__dijkstra_path_prev[b] = a;
						}
					}
				}
			}
			__clear_vst_dfs(post_reset);
			__dijkstra_done = a;
			__dijkstra_dist = dist;
			return dist;
		}/*}}}*/

		vector<weight_type>& bellman_ford(int a, bool pre_reset=0, bool post_reset=1) {/*{{{*/
			// also works fine for negative edges
			if (a == __bellman_ford_done) return __bellman_ford_dist;
			__make_vst_dfs(pre_reset);
			vector<weight_type> *pdist = new vector<weight_type>(max_node+1, min_weight_id), &dist = *pdist;
			__bellman_ford_path_prev.assign(max_node+1, 0);
			dist[a] = 0;
			for (int it=1; it<=max_node-1; it++) {
				vector<weight_type> next_dist = __bellman_ford_one_iteration(dist);
				swap(next_dist, dist);
				next_dist.clear();
			}
			__clear_vst_dfs(post_reset);
			__bellman_ford_done = a;
			__bellman_ford_dist = dist;
			return dist;
		}/*}}}*/

		bool has_negative_cycle(bool pre_reset=0, bool post_reset=1) {/*{{{*/
			vector<vector<int> > sccs = strongly_connected_components();
			vector<weight_type> dist(max_node, numeric_limits<weight_type>::max());
			for (auto &scc : sccs) {
				int a = scc.front(); if (__vst_dfs[a]) continue;
				__dfs_component_get_size_recursive(a);
				dist[a] = 0;
			}
			for (int it=1; it<=max_node-1; it++) {
				vector<weight_type> next_dist = __bellman_ford_one_iteration(dist);
				swap(next_dist, dist);
				next_dist.clear();
			}
			vector<weight_type> next_dist = __bellman_ford_one_iteration(dist);
			bool flag = 0;
			for (int i=1; i<=max_node; i++) if (next_dist[i] != dist[i]) { flag = 1; break; }
			next_dist.clear(); dist.clear();
			__clear_vst_dfs(post_reset);
			return flag;
		}/*}}}*/

		vector<vector<weight_type> >& floyd_warshall(weight_op add_op=0, weight_type add_id=0,/*{{{*/
				weight_op min_op=0, weight_type min_id=0, bool pre_reset=0, bool post_reset=1) {
			if (__floyd_warshall_done) return __floyd_warshall_dist;
			if (!add_op) add_op = add_weight_op, add_id = add_weight_id;
			if (!min_op) min_op = min_weight_op, min_id = min_weight_id;
			vector<vector<weight_type> > *pdist = new vector<vector<weight_type> >(max_node+1, vector<weight_type>(max_node+1, min_id)),
				&dist = *pdist;
			__floyd_warshall_path_prev.assign(max_node+1, vector<int>(max_node+1, 0));
			for (int i=1; i<=max_node; i++) dist[i] = add_id;
			for (iintw e : edges) {
				int a, b; weight_type w; tie(a, b, w) = e;
				dist[a][b] = w;
				__floyd_warshall_path_prev[a][b] = a;
			}
			for (int k=1; k<=max_node; k++) {
				for (int a=1; a<=max_node; a++) { if (dist[a][k] == min_id) continue;
					for (int b=1; b<=max_node; b++) { if (dist[k][b] == min_id) continue;
						weight_type newd = add_op(dist[a][k], dist[k][b]);
						if (dist[a][b] == min_op(dist[a][b], newd)) {
							dist[a][b] = newd;
							__floyd_warshall_path_prev[a][b] = k;
						}
					}
				}
			}
			__floyd_warshall_done = 1;
			__floyd_warshall_dist = dist;
			return dist;
		}/*}}}*/

		vector<int>& path_dijkstra(int a, int b) {/*{{{*/
			if (a != __dijkstra_done) {
				clear_path_problem_caches("dijkstra");
				dijkstra(a);
			}
			vector<int> *pp = new vector<int>(), &p = *pp;
			p.push_back(b);
			while (b && b != a) {
				b = __dijkstra_path_prev[b];
				p.push_back(b);
			}
			reverse(p.begin(), p.end());
			if (p.front() != a) p.clear();
			return p;
		}/*}}}*/

		vector<int>& path_bellman_ford(int a, int b) {/*{{{*/
			if (a != __bellman_ford_done) {
				clear_path_problem_caches("bellman_ford");
				bellman_ford(a);
			}
			vector<int> *pp = new vector<int>(), &p = *pp;
			p.push_back(b);
			while (b && b != a) {
				b = __bellman_ford_path_prev[b];
				p.push_back(b);
			}
			reverse(p.begin(), p.end());
			if (p.front() != a) p.clear();
			return p;
		}/*}}}*/

		vector<int>& path_floyd_warshall(int a, int b) {/*{{{*/
			if (!__floyd_warshall_done) {
				clear_path_problem_caches("floyd_warshall");
				floyd_warshall();
			}
			vector<int> *pp = new vector<int>(), &p = *pp;
			if (__floyd_warshall_path_prev[a][b]) {
				p.push_back(b);
				while (b && b != a) {
					b = __floyd_warshall_path_prev[a][b];
					p.push_back(b);
				}
				reverse(p.begin(), p.end());
			}
			return p;
		}/*}}}*/

		void clear_path_problem_caches(string problem="") {/*{{{*/
			if (problem == "" || problem == "dijkstra") __dijkstra_path_prev.clear(), __dijkstra_done = 0;
			if (problem == "" || problem == "bellman_ford") __bellman_ford_path_prev.clear(), __bellman_ford_done = 0;
			if (problem == "" || problem == "floyd_warshall") {
				for (auto &v : __floyd_warshall_path_prev) v.clear();
				__floyd_warshall_path_prev.clear(), __floyd_warshall_done = 0;
			}
		}/*}}}*/

		weight_type dinic(int src, int dst, bool pre_reset=0, bool post_reset=1) {/*{{{*/
			__make_flow(pre_reset);
			weight_type flow = 0;
			while (__bfs_flow(src, dst)) {
				fill(__w_flow.begin(), __w_flow.end(), 0);
				while (1) {
					int cur = __dfs_flow(src, dst, numeric_limits<weight_type>::max());
					if (!cur) break;
					flow += cur;
				}
			}
			__clear_flow(post_reset);
			return flow;
		}/*}}}*/

		pair<vector<int>, vector<int> > min_cut(int src, int dst, bool pre_reset=0, bool post_reset=1) {/*{{{*/
			if (!__flow_make_done) dinic(src, dst, pre_reset, 1);
			__dfs_residual_reachable(src);
			vector<int> *pa = new vector<int>(), *pb = new vector<int>(), &a = *pa, &b = *pb;
			for (int i=1; i<=max_node; i++)
				(__vst_dfs[i]? a : b).push_back(i);
			return make_pair(a, b);
		}/*}}}*/

};
#endif

#define GRAPH_DW_DEFINED
//////// ---- Woosung Song's Source Code ---- ////////


int main() {
	graph_dw<int> gr(10);
	return 0;
}
