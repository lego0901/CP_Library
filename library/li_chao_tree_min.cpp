#include <bits/stdc++.h>

using namespace std;

//////// ---- Woosung Song's Source Code ---- ////////
#ifndef LI_CHAO_TREE_MIN_DEFINED
typedef long long ll;
const ll LCIF = (0x3f2f1f0f3f2f1f0f);

struct li_chao_line_min {
	ll a, b;
	li_chao_line_min(ll A=0, ll B=LCIF) : a(A), b(B) {}
	ll operator()(const ll x) {
		return a*x + b;
	}
};

struct li_chao_node_min {
	int left, right; // child
	ll l, r; // range
	li_chao_line_min f; // dominant line at x = (s+e)/2
};

class li_chao_tree_min {
	private:
		void update(li_chao_line_min f, int tn=0) {
			ll l = tree[tn].l, r = tree[tn].r, m = (l + r) / 2;

			li_chao_line_min low = tree[tn].f, high = f;
			if (low(l) < high(l)) swap(low, high);
			if (low(r) >= high(r)) {
				tree[tn].f = high;
				return;
			}

			if (low(m) > high(m)) {
				tree[tn].f = high;
				if (tree[tn].right == -1) {
					tree[tn].right = tree.size();
					tree.push_back({-1, -1, m+1, r, {0, LCIF}});
				}
				update(low, tree[tn].right);
			} else {
				tree[tn].f = low;
				if (tree[tn].left == -1) {
					tree[tn].left = tree.size();
					tree.push_back({-1, -1, l, m, {0, LCIF}});
				}
				update(high, tree[tn].left);
			}
		}
	
	public:
		vector<li_chao_node_min> tree;
		ll l, r;

		li_chao_tree_min(ll l, ll r) {
			this->l = l, this->r = r;
			tree.push_back({-1, -1, l, r, {0, LCIF}});
		}

		void init() {
			tree.clear();
			tree.push_back({-1, -1, l, r, {0, LCIF}});
		}

		void insert(ll a, ll b) {
			update(li_chao_line_min(a, b));
		}

		ll min_value(ll x, int tn=0) {
			if (tn == -1) return LCIF; // id
			ll l = tree[tn].l, r = tree[tn].r, m = (l + r) / 2;
			if (x <= m) return min(tree[tn].f(x), min_value(x, tree[tn].left));
			else return min(tree[tn].f(x), min_value(x, tree[tn].right));
		}
};
#endif

#define LI_CHAO_TREE_MIN_DEFINED
//////// ---- Woosung Song's Source Code ---- ////////

const int MAX_N = 1e5 + 20;

int N;
ll A[MAX_N], B[MAX_N], D[MAX_N];

int main() {
	scanf("%d", &N);
	for (int i=1; i<=N; i++) scanf("%lld", A+i);
	for (int i=1; i<=N; i++) scanf("%lld", B+i);

	li_chao_tree_min lct(0, 1e9+20);
	lct.insert(A[N], 0);

	for (int i=N-1; i>=1; i--) {
		D[i] = lct.min_value(B[i]);
		lct.insert(A[i], D[i]);
	}

	printf("%lld\n", D[1]);
}
