#include <bits/stdc++.h>

using namespace std;

//////// ---- Woosung Song's Source Code ---- ////////
#ifndef LI_CHAO_TREE_DEFINED
typedef long long ll;
const ll LCID = -(0x3f2f1f0f3f2f1f0f);

struct li_chao_line {
	ll a, b;
	li_chao_line(ll A=0, ll B=LCID) : a(A), b(B) {}
	ll operator()(const ll x) {
		return a*x + b;
	}
};

struct li_chao_node {
	int left, right; // child
	ll l, r; // range
	li_chao_line f; // dominant line at x = (s+e)/2
};

class li_chao_tree {
	private:
		void update(li_chao_line f, int tn=0) {
			ll l = tree[tn].l, r = tree[tn].r, m = (l + r) / 2;

			li_chao_line low = tree[tn].f, high = f;
			if (low(l) > high(l)) swap(low, high);
			if (low(r) <= high(r)) {
				tree[tn].f = high;
				return;
			}

			if (low(m) < high(m)) {
				tree[tn].f = high;
				if (tree[tn].right == -1) {
					tree[tn].right = tree.size();
					tree.push_back({-1, -1, m+1, r, {0, LCID}});
				}
				update(low, tree[tn].right);
			} else {
				tree[tn].f = low;
				if (tree[tn].left == -1) {
					tree[tn].left = tree.size();
					tree.push_back({-1, -1, l, m, {0, LCID}});
				}
				update(high, tree[tn].left);
			}
		}
	
	public:
		vector<li_chao_node> tree;
		ll l, r;

		li_chao_tree(ll l, ll r) {
			this->l = l, this->r = r;
			tree.push_back({-1, -1, l, r, {0, LCID}});
		}

		void init() {
			tree.clear();
			tree.push_back({-1, -1, l, r, {0, LCID}});
		}

		void insert(ll a, ll b) {
			update(li_chao_line(a, b));
		}

		ll max_value(ll x, int tn=0) {
			if (tn == -1) return LCID; // id
			ll l = tree[tn].l, r = tree[tn].r, m = (l + r) / 2;
			if (x <= m) return max(tree[tn].f(x), max_value(x, tree[tn].left));
			else return max(tree[tn].f(x), max_value(x, tree[tn].right));
		}
};
#endif

#define LI_CHAO_TREE_DEFINED
//////// ---- Woosung Song's Source Code ---- ////////

int main() {

}
