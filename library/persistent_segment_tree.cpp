#include <bits/stdc++.h>

using namespace std;

//////// ---- Woosung Song's Source Code ---- ////////
#ifndef SEGMENT_TREE_NODE_DEFINED
template <class dtype>
class persistent_segment_tree {
	typedef persistent_segment_tree<dtype> node;
	int l, r;
	dtype value;
	node *left, *right; //, *parent;

	private:
		void make() {
			value = 0; // ::fill:: identity value
			if (l < r) {
				int m = (l + r) / 2;
				left = new node(l, m);
				right = new node(m+1, r);
			}
		}

	public:
		persistent_segment_tree<dtype>() : left(0), right(0) {}
		persistent_segment_tree<dtype>(int L, int R) : l(L), r(R), left(0), right(0) { make(); }
		persistent_segment_tree<dtype>(int L, int R, bool NOTMAKE)
			: l(L), r(R), left(0), right(0) { (void) NOTMAKE; }

		dtype update(int p, dtype v) {
			if (l <= p && p <= r) {
				if (left) {
					value = left->update(p, v) + right->update(p, v);
					// ::fill:: sum operator
				} else {
					value = v;
					// ::fill:: sum operator
				}
			}
			return value;
		}

		node* update_nodes(int p, dtype v) {
			if (l <= p && p <= r) {
				node* n = new node(l, r, 1);
				if (left) {
					n->left = left->update_nodes(p, v);
					n->right = right->update_nodes(p, v);
					n->value = n->left->value + n->right->value;
					// ::fill:: sum operator
				} else {
					n->value = v;
					// ::fill:: sum operator
				}
				return n;
			}
			return this;
		}

		dtype sum(int li, int ri) {
			if (li <= l && r <= ri) return value;
			else if (r < li || ri < l) return 0; // ::fill:: identity
			else {
				return (left->sum(li, ri) + right->sum(li, ri));
				// ::fill:: sum operator
			}
		}
};
#endif

#define SEGMENT_TREE_NODE_DEFINED
//////// ---- Woosung Song's Source Code ---- ////////


typedef long long ll;
typedef persistent_segment_tree<ll> pst_ll;

int main() {
	int n, m, k;
	scanf("%d%d%d", &n, &m, &k);
	m += k;

	vector<pst_ll*> pst;
	pst.push_back(new pst_ll(1, n));
	for (int i=1; i<=n; i++) {
		ll a; scanf("%lld",&a);
		pst.back()->update(i, a);
	}

	while (m--) {
		ll a, b, c; scanf("%lld%lld%lld", &a, &b, &c);
		if (a == 1) {
			pst.push_back(pst.back()->update_nodes(b, c));
		} else {
			ll d; scanf("%lld", &d);
			printf("%lld\n", pst[b]->sum(c, d));
		}
	}
}
