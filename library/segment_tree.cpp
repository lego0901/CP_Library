#include <bits/stdc++.h>

using namespace std;

//////// ---- Woosung Song's Source Code ---- ////////
#ifndef SEGMENT_TREE_DEFINED
template <class dtype>
struct segment_tree {
	int l, r;
	dtype value;
	segment_tree<dtype> *left, *right; //, *parent;
	segment_tree<dtype>() : left(0), right(0) {}
	segment_tree<dtype>(int L, int R) : l(L), r(R), left(0), right(0) { make(); }

	void make() {
		value = 0; // ::fill:: identity value
		if (l < r) {
			int m = (l + r) / 2;
			left = new segment_tree<dtype>(l, m);
			right = new segment_tree<dtype>(m+1, r);
		}
	}

	dtype update(int p, dtype v) {
		if (l <= p && p <= r) {
			if (left) {
				value = (left->update(p, v) + right->update(p, v));
				// ::fill:: sum operator
			} else {
				value = v; // ::fill:: update operator
			}
		}
		return value;
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

#define SEGMENT_TREE_DEFINED
//////// ---- Woosung Song's Source Code ---- ////////


typedef long long ll;

int main() {
	int n, m, k;
	scanf("%d%d%d", &n, &m, &k);
	m += k;

	segment_tree<ll> seg(1, n);
	for (int i=1; i<=n; i++) {
		ll a; scanf("%lld",&a);
		seg.update(i, a);
	}

	while (m--) {
		ll a, b, c; scanf("%lld%lld%lld", &a, &b, &c);
		if (a == 1) {
			seg.update(b, c);
		} else {
			printf("%lld\n", seg.sum(b, c));
		}
	}
}
