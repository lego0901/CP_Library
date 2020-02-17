#include <bits/stdc++.h>

using namespace std;

//////// ---- Woosung Song's Source Code ---- ////////
#ifndef SPLAY_TREE_DEFINED
typedef long long ll;

template <class sum_type>
struct __node {
	__node *l, *r, *p;
	int key, cnt;
	sum_type sum, value;

	__node(int k=0, sum_type v=0) : l(0), r(0), p(0), key(k), cnt(1), sum(v), value(v) {}
};

template <class sum_type>
class splay_tree {
	private:
		typedef __node<sum_type> node;
		typedef sum_type (*sum_op) (sum_type, sum_type);
			// left_sum, right_sum -> new_sum
		sum_op DEFAULT_ADD_OP =
			[](sum_type a, sum_type b) { return a + b; };
		sum_type DEFAULT_ADD_ID = 0;

		node *root;
		int __cnt;
		sum_op add_op;
		sum_type add_id;

		void refresh(node *x) {
			x->cnt = 1 + (x->l? x->l->cnt : 0) + (x->r? x->r->cnt : 0);
			x->sum = x->value + add_op(x->l? x->l->sum: add_id, x->r? x->r->sum: add_id);
		}


	public:
		splay_tree(sum_op add_op=0, sum_type add_id=0) : root(NULL), __cnt(0) {
			if (!add_op) add_op = DEFAULT_ADD_OP, add_id = DEFAULT_ADD_ID;
			this->add_op = add_op, this->add_id = add_id;
			insert(numeric_limits<int>::min(), add_id);
			insert(numeric_limits<int>::max(), add_id);
		}

	private:
		void rotate(node *x) {
			node *p = x->p, *b;
			if (x == p->l) p->l = b = x->r, x->r = p;
			else p->r = b = x->l, x->l = p;
			x->p = p->p, p->p = x;
			if (b) b->p = p;
			(x->p? p == x->p->l? x->p->l : x->p->r : root) = x;
			refresh(p);
			refresh(x);
		}
		
		void splay(node *x) {
			while (x->p) {
				node *p = x->p, *g = p->p;
				if (g) rotate((x==p->l) == (p==g->l)? p : x);
				rotate(x);
			}
			__cnt = x->cnt;
		}

		void kth(int k) {
			node *x = root;
			while (1) {
				while (x->l && x->l->cnt > k) x = x->l;
				if (x->l) k -= x->l->cnt;
				if (k-- == 0) break;
				x = x->r;
			}
			splay(x);
		}

		node* interval(int l, int r) {
			// assuming that 2 dummy nodes still exist
			kth(l-1);
			node *x = root; root = x->r; root->p = 0;
			kth(r-l+1);
			x->r = root; root->p = x; root = x;
			return x->r->l;
		}

		void print(node *x) {
			if (x->l) print(x->l);
			cout << x->key << ' ' << x->sum << endl;
			if (x->r) print(x->r);
		}


	public:
		int size() {
			return __cnt;
		}

		void print() {
			if (root) print(root);
		}

		void insert(int key, sum_type value) {
			node *p = root, **pp;
			if (!p) {
				node *x = new node(key, value);
				root = x; return;
			}
			while (1) {
				if (key == p->key) return;
				if (key < p->key) {
					if (!p->l) { pp = &p->l; break; }
					p = p->l;
				} else {
					if (!p->r) { pp = &p->r; break; }
					p = p->r;
				}
			}
			node *x = new node(key, value);
			*pp = x;
			x->p = p;
			splay(x);
		}

		bool find(int key) {
			node *p = root;
			if (!p) return 0;
			while (p) {
				if (key == p->key) break;
				if (key < p->key) {
					if (!p->l) break;
					p = p->l;
				} else {
					if (!p->r) break;
					p = p->r;
				}
			}
			splay(p);
			return key == p->key;
		}

		bool erase(int key) {
			if (!find(key)) return 0;
			node *p = root;
			if (p->l) {
				root = p->l, root->p = 0;
				if (p->r) {
					node *x = root;
					while (x->r) x = x->r;
					x->r = p->r, p->r->p = x;
					refresh(x);
					splay(x);
				}
			} else if (p->r) {
				root = p->r, root->p = 0;
			} else root = 0;
			__cnt = root? root->cnt : 0;
			delete p;
			return 1;
		}

		void refresh(int i, sum_type v) {
			kth(i);
			root->value = v;
			refresh(root);
		}

		sum_type sum(int l, int r) {
			return interval(l, r)->sum;
		}

};
#endif

#define SPLAY_TREE_DEFINED
//////// ---- Woosung Song's Source Code ---- ////////

int main() {
	typedef long long ll;
	splay_tree<ll> sp;

	int n, m, k;
	scanf("%d%d%d", &n, &m, &k);
	for (int i=1; i<=n; i++) {
		ll x; scanf("%lld", &x);
		sp.insert(i, x);
	}
	//sp.print();
	m += k;
	while (m--) {
		int a, b, c;
		scanf("%d%d%d", &a, &b, &c);
		if (a == 1) {
			sp.refresh(b, c);
			//cout << "refreshd " << endl;
			//sp.print();
		} else {
			printf("%lld\n", sp.sum(b, c));
		}
	}
}
