#include <bits/stdc++.h>

using namespace std;

//////// ---- Woosung Song's Source Code ---- ////////
#ifndef SEGMENT_TREE_MIN_SUM_DEFINED
struct segment_tree_min_sum {
	typedef long long ll;

	int l, r;
	ll sum;
	ll lmnsum, rmnsum, mnsum;
	const ll SEG_MAX = 0x3f2f1f0f3f2f1f0f;

	segment_tree_min_sum *left, *right; //, *parent;
	segment_tree_min_sum() : left(0), right(0) {}
	segment_tree_min_sum(int L, int R) : l(L), r(R), left(0), right(0) { make(); }

	void make() {
		sum = 0; // ::fill:: identity sum 
		if (l < r) {
			int m = (l + r) / 2;
			left = new segment_tree_min_sum(l, m);
			right = new segment_tree_min_sum(m+1, r);
		}
	}

	void update(int p, ll v) {
		if (l <= p && p <= r) {
			if (left) {
				left->update(p, v);
				right->update(p, v);
				sum = left->sum + right->sum;
				rmnsum = min(right->rmnsum, left->rmnsum + right->sum);
				lmnsum = min(left->lmnsum, right->lmnsum + left->sum);
				mnsum = min(left->rmnsum + right->lmnsum,
							min(left->mnsum, right->mnsum));
				// ::fill:: sum operator
			} else {
				sum = lmnsum = rmnsum = mnsum = v;
			}
		}
	}

	ll naive_sum(int li, int ri) {
		if (li <= l && r <= ri) return sum;
		else if (r < li || ri < l) return 0; // ::fill:: identity
		else {
			return (left->naive_sum(li, ri) + right->naive_sum(li, ri));
		}
	}

	ll left_min_sum(int li, int ri) {
		if (li <= l && r <= ri) {
			return lmnsum;
		} else if (li > r || l > ri) {
			return SEG_MAX;
		} else {
			return min(left->left_min_sum(li, ri),
						left->naive_sum(li, ri) + right->left_min_sum(li, ri));
		}
	}

	ll right_min_sum(int li, int ri) {
		if (li <= l && r <= ri) {
			return lmnsum;
		} else if (li > r || l > ri) {
			return SEG_MAX;
		} else {
			return min(right->right_min_sum(li, ri),
						right->naive_sum(li, ri) + left->right_min_sum(li, ri));
		}
	}

	ll _rmn(int li) {
		if (li <= l) return rmnsum;
		else if (li > r) return SEG_MAX;
		else return min(left->_rmn(li) + right->sum, right->_rmn(li));
	}

	ll _lmn(int ri) {
		if (r <= ri) return lmnsum;
		else if (l > ri) return SEG_MAX;
		else return min(right->_lmn(ri) + left->sum, left->_lmn(ri));
	}

	ll min_sum(int li, int ri) {
		if (li <= l && r <= ri) return mnsum;
		else if (li > r || l > ri) return SEG_MAX;
		else {
			return min(min(left->min_sum(li, ri), right->min_sum(li, ri)),
					left->_rmn(li) + right->_lmn(ri));
		}
	}
};
#endif

#define SEGMENT_TREE_MIN_SUM_DEFINED
//////// ---- Woosung Song's Source Code ---- ////////


typedef long long ll;

int main() {
	int n, m;
	scanf("%d%d", &n, &m);

	segment_tree_min_sum seg(1, n);
	for (int i=1; i<=n; i++) {
		ll a; scanf("%lld",&a);
		seg.update(i, a);
	}

	while (m--) {
		ll a, b, c; scanf("%lld%lld%lld", &a, &b, &c);
		if (a == 1) {
			seg.update(b, c);
		} else {
			printf("%lld\n", seg.min_sum(b, c));
		}
	}
}
