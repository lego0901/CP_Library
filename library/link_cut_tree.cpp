#include <bits/stdc++.h>

using namespace std;

//////// ---- Woosung Song's Source Code ---- ////////
#ifndef LINK_CUT_TREE_DEFINED
struct __lct_node {
	__lct_node *l, *r, *p, *pp;
	int idx;
	bool inv;
	int val, min_val;

	__lct_node(int idx, int val) : l(0), r(0), p(0), pp(0), inv(0) {
		this->idx = idx;
		this->val = val;
		min_val = val;
	}
};

class link_cut_tree {
	private:
		typedef __lct_node node;

		void refresh(node *x) {
			x->min_val = x->val;
			if (x->l && x->min_val > x->l->min_val)
				x->min_val = x->l->min_val;
			if (x->r && x->min_val > x->r->min_val)
				x->min_val = x->r->min_val;
		}

		void propagate(node *x) {
			if (!x->inv) return;
			swap(x->l, x->r);
			x->inv = 0;
			if (x->l) x->l->inv = !x->l->inv;
			if (x->r) x->r->inv = !x->r->inv;
		}
	
	public:
		int max_node;
		vector<node*> nodes;

		link_cut_tree(int max_node) {
			this->max_node = max_node;
			nodes.resize(max_node+1);
			for (int i=1; i<=max_node; i++) {
				nodes[i] = new node(i, 0x3f2f1f0f);
			}
		}

		link_cut_tree(int max_node, vector<int> &vals) {
			this->max_node = max_node;
			nodes.resize(max_node+1);
			for (int i=1; i<=max_node; i++) {
				nodes[i] = new node(i, vals[i]);
			}
		}
	
	private:
		void rotate(node *x) {
			node *p = x->p, *b;
			propagate(p);
			propagate(x);
			if (x == p->l) p->l = b = x->r, x->r = p;
			else p->r = b = x->l, x->l = p;
			x->p = p->p, p->p = x;
			if (b) b->p = p;
			if (x->p) (p == x->p->l? x->p->l : x->p->r) = x;
			else x->pp = p->pp, p->pp = 0;
			refresh(p);
			refresh(x);
		}

		void splay(node *x) {
			while (x->p) {
				node *p = x->p, *g = p->p;
				if (g) rotate((x==p->l) == (p==g->l)? p : x);
				rotate(x);
			}
			propagate(x);
		}

		void access(node *x) {
			splay(x);
			if (x->r) x->r->pp = x, x->r->p = 0, x->r = 0;
			while (x->pp) {
				node *pp = x->pp;
				splay(pp);
				if (pp->r) pp->r->pp = pp, pp->r->p = 0;
				pp->r = x;
				x->p = pp;
				x->pp = 0;
				splay(x);
			}
		}

		node *find_root(node *x) {
			access(x);
			while (x->l) x = x->l;
			access(x);
			return x;
		}

		node *find_parent(node *x) {
			access(x);
			if (!x->l) return 0;
			x = x->l;
			while (1) {
				propagate(x);
				if (x->r) x = x->r;
				else break;
			}
			return x;
		}
		
		void cut(node *x) {
			access(x);
			if (x->l) x->l->p = 0, x->l = 0, refresh(x);
		}

		void link_root(node *x, node *y) {
			//x and y should be roots
			access(x);
			access(y);
			x->l = y;
			y->p = x;
			refresh(x);
		}

		void make_root(node *x) {
			access(x);
			x->inv = !x->inv;
		}

		void link(node *x, node *y) {
			make_root(x);
			link_root(x, y);
		}

		node *lca(node *x, node *y) {
			access(x);
			access(y);
			splay(x);
			return x->pp? x->pp : x;
		}

		void update(node *x, int val) {
			access(x);
			x->val = val;
			refresh(x);
		}

		int path_vertices_up_query(node *p, node *y) {
			if (p == y) return p->min_val;

			access(p);
			access(y);
			splay(p);
			p->r->p = 0;
			splay(y);
			y->p = p;
			// p->r = y; ??

			int res = min(p->val, y->val);
			return y->l && res > y->l->min_val? y->l->min_val : res;
		}

		void print(node *x) {
			if (!x) return;
			printf("node %d: l-%d, r-%d, p-%d, pp-%d, inv-%d\n",
					x->idx, (x->l? x->l->idx : 0),
					(x->r? x->r->idx : 0),
					(x->p? x->p->idx : 0),
					(x->pp? x->pp->idx : 0),
					x->inv);
			//print(x->l);
			//print(x->r);
		}
	
	
	public:
		int root_index(int a) {
			return find_root(nodes[a])->idx;
		}

		bool is_same_tree(int a, int b) {
			return find_root(nodes[a]) == find_root(nodes[b]);
		}

		bool link(int a, int b) {
			if (is_same_tree(a, b)) return 0;
			link(nodes[a], nodes[b]);
			return 1;
		}

		int parent(int a) {
			node *x = find_parent(nodes[a]);
			return x? x->idx : 0;
		}

		bool cut(int a, int b) {
			if (find_parent(nodes[a]) == nodes[b]) swap(a, b);
			if (find_parent(nodes[b]) != nodes[a]) return 0;
			cut(nodes[b]);
			return 1;
		}

		int lca(int a, int b) {
			if (!is_same_tree(a, b)) return 0;
			return lca(nodes[a], nodes[b])->idx;
		}

		void update(int a, int val) {
			update(nodes[a], val);
		}

		int path_vertices_query(int a, int b) {
			node *x = nodes[a], *y = nodes[b], *p = lca(x, y);
			
			int px = path_vertices_up_query(p, x);
			int py = path_vertices_up_query(p, y);
			return min(px, py);
		}

		void print(int a) {
			print(nodes[a]);
		}

		void print() {
			for (int a=1; a<=max_node; a++)
				print(a);
		}
};
#endif

#define LINK_CUT_TREE_DEFINED
//////// ---- Woosung Song's Source Code ---- ////////


int main() {
	int n, q;
	scanf("%d%d", &n, &q);

	link_cut_tree lct(n);

	vector<int> parents(n+1, 0);

	while (q--) {
		int t, a, b; scanf("%d%d", &t, &a);
		if (t == 1) {
			scanf("%d", &b);
			lct.link(a, b);
			parents[a] = b;
		} else if (t == 2) {
			lct.cut(a, parents[a]);
		} else {
			scanf("%d", &b);
			printf("%d\n", lct.lca(a, b));
		}
	}
}
