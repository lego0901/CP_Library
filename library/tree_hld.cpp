#include <bits/stdc++.h>

using namespace std;


//////// ---- Woosung Song's Source Code ---- ////////

// tree class with efficient algorithms
// NOT IMPLEMENTED!!!

// library 'binary_indexed_tree.cpp'{{{
//
//////// ---- Woosung Song's Source Code ---- ////////

// binary indexed tree with operators.

#ifndef BINARY_INDEXED_TREE_DEFINED
template <class sum_type>
class binary_indexed_tree {
	private:
		typedef sum_type (*sum_op) (sum_type, sum_type);
		sum_op DEFAULT_ADD_OP = [](sum_type a, sum_type b) { return a + b; };
		sum_type DEFAULT_ADD_ID = 0;
	
	public:
		int max_index, min_index;
		int size;
		vector<sum_type> v;

		sum_op add_op = DEFAULT_ADD_OP;
		sum_type add_id = DEFAULT_ADD_ID;

		binary_indexed_tree<sum_type>() {}

		binary_indexed_tree<sum_type>(int max_index, int min_index=1,
				sum_op add_op=0, sum_type add_id=0) {
			this->max_index = max_index, this->min_index = min_index;
			if (add_op) this->add_op = add_op, this->add_id = add_id;
			assert(min_index <= max_index);
			for (size=1; size<max_index-min_index+1; size<<=1);
			v.assign(size * 2, add_id);
		}

		void update(int p, sum_type x) {
			assert(min_index <= p && p <= max_index);
			v[p=p-min_index+size] = x;
			for (p>>=1; p; p>>=1) {
				v[p] = add_op(v[p+p], v[p+p+1]);
			}
		}

		sum_type sum(int l, int r) {
			assert(min_index <= l && l <= r && r <= max_index);
			sum_type s = add_id;
			l = l - min_index + size, r = r - min_index + size;
			for (; l<=r; l>>=1, r>>=1) {
				if (l & 1) s = add_op(s, v[l++]);
				if ((r+1)&1) s = add_op(s, v[r--]);
			}
			return s;
		}
};
#endif

#define BINARY_INDEXED_TREE_DEFINED

//////// ---- Woosung Song's Source Code ---- ////////}}}

#ifndef TREE_HLD_DEFINED
template <class weight_type>
class tree_hld {
	private:
		typedef pair<int,weight_type> intw;
		typedef vector<vector<intw> > edge_type;
		typedef vector<pair<int,int> > edges_type;

		typedef weight_type (*weight_op) (weight_type, weight_type);
		weight_op DEFAULT_ADD_OP = [](weight_type a, weight_type b) { return a + b; };
		weight_type DEFAULT_ADD_ID = 0;
	
	public:
		int max_node, root, log_node;
		vector<int> parents, weights, hldnums, hldroots;
		edge_type edge;
		edges_type edges;
		binary_indexed_tree<weight_type> bit; // 구간 쿼리면 segment_tree_lazy or splay_lazy

		weight_op add_op = DEFAULT_ADD_OP;
		weight_type add_id = DEFAULT_ADD_ID;

		tree_hld(int max_node, int root=1, weight_op add_op=0, weight_type add_id=0) {/*{{{*/
			this->max_node = max_node;
			this->root = root;
			if (add_op) this->add_op = add_op, this->add_id = add_id;
			for (log_node=1; (1<<(log_node-1))<=max_node; log_node++);
			edge.assign(max_node+1, vector<intw>());
			parents.assign(max_node+1, 0);
			weights.assign(max_node+1, 0);
			hldnums.assign(max_node+1, 0);
			hldroots.assign(max_node+1, 0);
			edges.resize(0);
			bit = binary_indexed_tree<weight_type>(max_node, 1, add_op, add_id);
		}/*}}}*/

		void add_edge(int a, int b, weight_type w) {/*{{{*/
			edge[a].emplace_back(b, w); edge[b].emplace_back(a, w);
			edges.emplace_back(a, b);
		}/*}}}*/
	
	private:
		vector<int> vst_dfs;
		vector<int> num_sib;
		vector<int> lv_dfs;
		vector<vector<int> > anc;
		vector<bool> dfs_banned_vertex;
		int vst_dfs_make_done = 0;
		int hld_make_done = 0;
		stack<int> clear_dfs;
		stack<int> stack_dfs;

		void make_vst_dfs(bool pre_reset=0) {
			if (!vst_dfs_make_done || pre_reset) {
				if ((int) vst_dfs.size() == max_node+1) {
					fill(vst_dfs.begin(), vst_dfs.end(), 0);
					fill(num_sib.begin(), num_sib.end(), 0);
					fill(lv_dfs.begin(), lv_dfs.end(), 0);
					fill(dfs_banned_vertex.begin(), dfs_banned_vertex.end(), 0);
					for (auto &v : anc) fill(v.begin(), v.end(), 0);
				} else {
					vst_dfs.assign(max_node+1, 0);
					num_sib.assign(max_node+1, 0);
					lv_dfs.assign(max_node+1, 0);
					dfs_banned_vertex.assign(max_node+1, 0);
					anc.assign(max_node+1, vector<int>(log_node, 0));
				}
				vst_dfs_make_done = -1;
			}
		}

		void clear_vst_dfs(bool post_reset=1) {
			if (post_reset) {
				while (!clear_dfs.empty()) {
					vst_dfs[clear_dfs.top()] = 0;
					clear_dfs.pop();
				}
			}
		}

		void pre_dfs(int a=-1, int p=-1) {
			if (a == -1) a = root;
			if (p == -1) vst_dfs_make_done = a, lv_dfs[a] = 1; // saves the root number
			else {
				lv_dfs[a] = lv_dfs[p] + 1;
				parents[a] = p;
				anc[a][0] = p;
				for (int i=1; i<log_node; i++)
					anc[a][i] = anc[anc[a][i-1]][i-1];
			}
			vst_dfs[a] = 1;
			clear_dfs.push(a);
			num_sib[a] = 1;
			for (intw &bw : edge[a]) {
				int b; weight_type w; tie(b, w) = bw;
				if (dfs_banned_vertex[b] || b==p) continue;
				weights[b] = w;
				pre_dfs(b, a);
				num_sib[a] += num_sib[b];
			}
		}

		int hld_cnt = 0;
		void heavy_light_decomposition(int a=-1, int p=-1, int r=-1) {
			if (a == -1) a = root;
			if (p == -1) hld_make_done = a;
			if (r == -1) r = a;
			hldroots[a] = r;
			hldnums[a] = max_node - (hld_cnt++);
			bit.update(hldnums[a], weights[a]);
			int next = -1;
			for (intw &bw : edge[a]) {
				int b = bw.first;
				if (dfs_banned_vertex[b] || b == p) continue;
				if (num_sib[b] * 2 >= num_sib[a]) { next = b; break; }
			}
			if (next != -1) heavy_light_decomposition(next, a, r);
			for (intw &bw : edge[a]) {
				int b = bw.first;
				if (dfs_banned_vertex[b] || b == p || b == next) continue;
				heavy_light_decomposition(b, a, -1);
			}
		}

		weight_type sum_to_top(int a, int top) {
			weight_type s = add_id;
			while (lv_dfs[a] >= lv_dfs[top]) {
				int r = hldroots[a];
				if (lv_dfs[r] < lv_dfs[top]) r = top;
				s = add_op(s, bit.sum(hldnums[a], hldnums[r]));
				a = parents[r];
			}
			return s;
		}
	

	public:
		void make() {
			make_vst_dfs(0);
			pre_dfs();
			heavy_light_decomposition();
			clear_vst_dfs(1);
		}

		int get_level(int a) {
			if (!vst_dfs_make_done) make();
			return lv_dfs[a];
		}

		int lca(int a, int b) {
			if (!vst_dfs_make_done) make();
			if (lv_dfs[a] > lv_dfs[b]) swap(a, b);
			for (int i=log_node-1; i>=0; i--)
				if (lv_dfs[a] <= lv_dfs[anc[b][i]]) b = anc[b][i];
			if (a == b) return a;
			for (int i=log_node-1; i>=0; i--)
				if (anc[a][i] != anc[b][i]) a = anc[a][i], b = anc[b][i];
			return anc[a][0];
		}

		void update_weight(int a, int b, weight_type w) {
			if (!vst_dfs_make_done) make();
			// should be modified with bit
			if (parents[b] == a) swap(a, b);
			assert(parents[a] == b);
			weights[a] = w;
			bit.update(hldnums[a], w);
		}

		weight_type sum(int a, int b) {
			if (!vst_dfs_make_done) make();
			if (lv_dfs[a] > lv_dfs[b]) swap(a, b);
			int la = a, lb = b;
			for (int i=log_node-1; i>=0; i--)
				if (lv_dfs[la] < lv_dfs[anc[lb][i]]) lb = anc[lb][i];
			if (parents[lb] == la) return sum_to_top(b, lb);

			if (lv_dfs[la] < lv_dfs[lb]) lb = parents[lb];
			for (int i=log_node-1; i>=0; i--)
				if (anc[la][i] != anc[lb][i]) la = anc[la][i], lb = anc[lb][i];
			return add_op(sum_to_top(a, la), sum_to_top(b, lb));
		}
};
#endif

#define TREE_HLD_DEFINED

//////// ---- Woosung Song's Source Code ---- ////////


// library 'union_find.cpp'{{{
//
//////// ---- Woosung Song's Source Code ---- ////////

// an efficient data structure that implements union-find
// supports unio(returns 1 when separated components are merged) and find

#ifndef UNION_FIND_DEFINED
class union_find {
	public:
		int max_node;
		vector<int> parents;

		union_find(int max_node) {/*{{{*/
			this->max_node = max_node;
			parents.assign(max_node+1, 0);
			for (int i=0; i<=max_node; i++)
				parents[i] = i;
		}/*}}}*/

		int find(int a) {/*{{{*/
			return parents[a] == a? a : parents[a] = find(parents[a]);
		}/*}}}*/

		bool unio(int a, int b) {/*{{{*/
			parents[a = find(a)] = b = find(b);/*}}}*/
			return a != b;
		}/*}}}*/

		void print() {/*{{{*/
			for (int i=1; i<=max_node; i++)
				cout << find(i) << ' ';
			cout << endl;
		}/*}}}*/
};
#endif

#define UNION_FIND_DEFINED

//////// ---- Woosung Song's Source Code ---- ////////

auto add_op = [](int a, int b) { return max(a, b); };
auto add_id = numeric_limits<int>::min();

int main() {
	int n;
	scanf("%d", &n);
	tree_hld<int> tr(n, 1, add_op, add_id);
	for (int i=1; i<n; i++) {
		int a, b, c;
		scanf("%d%d%d", &a, &b, &c);
		tr.add_edge(a, b, c);
	}
	int m;
	scanf("%d", &m);
	while (m--) {
		int a, b, c;
		scanf("%d%d%d", &a, &b, &c);
		if (a == 1) {
			int u, v; tie(u, v) = tr.edges[b-1];
			tr.update_weight(u, v, c);
		} else {
			printf("%d\n", tr.sum(b, c));
		}
	}
}
