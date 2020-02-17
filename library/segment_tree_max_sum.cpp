#include <bits/stdc++.h>

using namespace std;

//////// ---- Woosung Song's Source Code ---- ////////
#ifndef SEGMENT_TREE_MAX_SUM_DEFINED
struct segment_tree_max_sum {
	typedef long long ll;

	int l, r;
	ll sum;
	ll lmxsum, rmxsum, mxsum;
	const ll SEG_MIN = -(0x3f2f1f0f3f2f1f0f);

	segment_tree_max_sum *left, *right; //, *parent;
	segment_tree_max_sum() : left(0), right(0) {}
	segment_tree_max_sum(int L, int R) : l(L), r(R), left(0), right(0) { make(); }

	void make() {
		sum = 0; // ::fill:: identity sum 
		if (l < r) {
			int m = (l + r) / 2;
			left = new segment_tree_max_sum(l, m);
			right = new segment_tree_max_sum(m+1, r);
		}
	}

	void update(int p, ll v) {
		if (l <= p && p <= r) {
			if (left) {
				left->update(p, v);
				right->update(p, v);
				sum = left->sum + right->sum;
				rmxsum = max(right->rmxsum, left->rmxsum + right->sum);
				lmxsum = max(left->lmxsum, right->lmxsum + left->sum);
				mxsum = max(left->rmxsum + right->lmxsum,
							max(left->mxsum, right->mxsum));
				// ::fill:: sum operator
			} else {
				sum = lmxsum = rmxsum = mxsum = v;
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

	ll left_max_sum(int li, int ri) {
		if (li <= l && r <= ri) {
			return lmxsum;
		} else if (li > r || l > ri) {
			return SEG_MIN;
		} else {
			return max(left->left_max_sum(li, ri),
						left->naive_sum(li, ri) + right->left_max_sum(li, ri));
		}
	}

	ll right_max_sum(int li, int ri) {
		if (li <= l && r <= ri) {
			return lmxsum;
		} else if (li > r || l > ri) {
			return SEG_MIN;
		} else {
			return max(right->right_max_sum(li, ri),
						right->naive_sum(li, ri) + left->right_max_sum(li, ri));
		}
	}

	ll _rmx(int li) {
		if (li <= l) return rmxsum;
		else if (li > r) return SEG_MIN;
		else return max(left->_rmx(li) + right->sum, right->_rmx(li));
	}

	ll _lmx(int ri) {
		if (r <= ri) return lmxsum;
		else if (l > ri) return SEG_MIN;
		else return max(right->_lmx(ri) + left->sum, left->_lmx(ri));
	}

	ll max_sum(int li, int ri) {
		if (li <= l && r <= ri) return mxsum;
		else if (li > r || l > ri) return SEG_MIN;
		else {
			return max(max(left->max_sum(li, ri), right->max_sum(li, ri)),
					left->_rmx(li) + right->_lmx(ri));
		}
	}
};
#endif

#define SEGMENT_TREE_MAX_SUM_DEFINED
//////// ---- Woosung Song's Source Code ---- ////////


typedef long long ll;

int main() {
	int n, m;
	scanf("%d%d", &n, &m);

	segment_tree_max_sum seg(1, n);
	for (int i=1; i<=n; i++) {
		ll a; scanf("%lld",&a);
		seg.update(i, a);
	}

	while (m--) {
		ll a, b, c; scanf("%lld%lld%lld", &a, &b, &c);
		if (a == 1) {
			seg.update(b, c);
		} else {
			printf("%lld\n", seg.max_sum(b, c));
		}
	}
}
