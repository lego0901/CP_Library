#include <bits/stdc++.h>

using namespace std;


//////// ---- Woosung Song's Source Code ---- ////////

// library 'graph_d_for_sat2.cpp'{{{
//
//////// ---- Woosung Song's Source Code ---- ////////

// graph(undirected) class with efficient algorithms
// supports connectivity, cycle existence, ... e.t.c.

#ifndef GRAPH_D_FOR_SAT2_DEFINED
class graph_d_for_sat2 {
	private:
		typedef vector<vector<int> > edge_type;

	public:
		int max_node;
		int size;
		edge_type edge;
		vector<int> in_deg;
		
		graph_d_for_sat2() {}
		
		graph_d_for_sat2(int max_node) {
			this->max_node = max_node;
			edge.assign(max_node+1, vector<int>());
			in_deg.assign(max_node+1, 0);
		}

		void add_edge(int a, int b) {/*{{{*/
			assert(a <= max_node && b <= max_node && a >= 0 && b >= 0);
			edge[a].push_back(b);
			in_deg[b]++;
		}/*}}}*/

		graph_d_for_sat2& copy() {/*{{{*/
			return subgraph_with_edge(edge);
		}/*}}}*/

		graph_d_for_sat2& subgraph_with_edge(edge_type sub_edge) {/*{{{*/
			graph_d_for_sat2 *psubgraph = new graph_d_for_sat2(max_node);
			graph_d_for_sat2 &subgraph = *psubgraph;
			for (int a=1; a<=max_node; a++)
				for (int b : sub_edge[a]) subgraph.add_edge(a, b);
			return subgraph;
		}/*}}}*/

		vector<int> __vst_dfs;
		vector<int> __low_dfs;
		bool __vst_dfs_make_done = 0;
		stack<int> __stack_dfs;
		stack<int> __clear_dfs;
		int __vst_number = 0;

	public:
		// now the following __ methods are public
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

		int __dfs_tarjan(int a, vector<int>& scc_number, int &scc_count) {/*{{{*/
			__vst_dfs[a] = __low_dfs[a] = ++__vst_number;
			__clear_dfs.push(a);
			__stack_dfs.push(a);
			for (int &b : edge[a]) {
				if (__vst_dfs[b]) {
					if (__vst_dfs[b] > 0) __low_dfs[a] = min(__low_dfs[a], __vst_dfs[b]);
				} else {
					__dfs_tarjan(b, scc_number, scc_count);
					__low_dfs[a] = min(__low_dfs[a], __low_dfs[b]);
				}
			}
			if (__vst_dfs[a] == __low_dfs[a]) {
				scc_count++;
				while (1) {
					int b = __stack_dfs.top(); __stack_dfs.pop();
					__vst_dfs[b] = -__vst_dfs[b];
					scc_number[b] = scc_count;
					if (b == a) break;
				}
			}
			return __low_dfs[a];
		}/*}}}*/
		
};
#endif

#define GRAPH_D_FOR_SAT2_DEFINED
/*}}}*/

#ifndef SAT2_DEFINED
class sat2 {
	public:
		int max_node;
		graph_d_for_sat2 gr;

		sat2(int max_node) {
			this->max_node = max_node;
			gr = graph_d_for_sat2(max_node*2 + 1);
		}

		static inline int not_node(int a) {
			return (a&1)? a+1 : a-1;
		}

		void add_implication(bool truth_a, int a, bool truth_b, int b) {
			// add implication of (truth_a a) => (truth_b b)
			a = a * 2 - truth_a;
			b = b * 2 - truth_b;
			int not_a = not_node(a), not_b = not_node(b);
			gr.add_edge(a, b);
			gr.add_edge(not_b, not_a); // contrapositive
		}
		
		void add_implication(int a, int b) {
			// +a : a, -a : not a
			add_implication(a>0, abs(a), b>0, abs(b));
		}

		void fix_tf_value(bool truth, int a) {
			// fix a = truth(true or false)
			add_implication(!truth, a, truth, a);
		}

		void fix_tf_value(int a) {
			// +a : a = true, -a : a = false
			fix_tf_value(a>0, abs(a));
		}

		void add_or(bool truth_a, int a, bool truth_b, int b) {
			// add (truth_a a) \/ (truth_b b)
			add_implication(!truth_a, a, truth_b, b);
		}

		void add_or(int a, int b) {
			// +a : a, -a : not a
			add_or(a>0, abs(a), b>0, abs(b));
		}

	private:
		vector<int>& strongly_connected_components(bool pre_reset=0, bool post_reset=1) {
			vector<int> *pscc_number = new vector<int>(max_node*2+1, 0), &scc_number = *pscc_number;
			int scc_count = 0;
			gr.__make_vst_dfs(pre_reset);
			for (int n=1; n<=gr.max_node; n++) {
				if (gr.__vst_dfs[n]) continue;
				gr.__dfs_tarjan(n, scc_number, scc_count);
			}
			for (int n=1; n<=gr.max_node; n++)
				scc_number[n] = scc_count - scc_number[n];
			gr.__clear_vst_dfs(post_reset);
			return scc_number;
		}

	public:
		bool is_satisfiable(vector<bool>& table, bool pre_reset=0, bool post_reset=1) {
			vector<int> scc_number = strongly_connected_components(pre_reset, post_reset);
			int r = 1;
			table.assign(max_node+1, 0);
			for (int i=1; i<=max_node*2; i+=2) {
				if (scc_number[i] == scc_number[i+1]) { r = 0; break; }
				table[(i+1)>>1] = scc_number[i] > scc_number[i+1];
			}
			return r;
		}

};
#endif

#define SAT2_DEFINED
//////// ---- Woosung Song's Source Code ---- ////////


int main() {
	int n; cin >> n;
	sat2 st(n);
	vector<bool> table;
	int m; cin >> m;
	while (m--) {
		int a, b; cin >> a >> b;
		st.add_implication(a<0, abs(a), b>0, abs(b));
	}
	int b = st.is_satisfiable(table);
	cout << b << endl;
	if (b) {
		for (int i=1; i<=n; i++) cout << table[i] << ' ';
		cout << endl;
	}
}
