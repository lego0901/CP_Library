#include <bits/stdc++.h>

using namespace std;

//////// ---- Woosung Song's Source Code ---- ////////
#ifndef SEGMENT_TREE_LAZY_NODE_DEFINED
template <class dtype, class lztype>
struct segment_lazy {
	int l, r;
	dtype value;
	lztype lazy;

	typedef segment_lazy<dtype, lztype> node;

	node *left, *right, *parent;
	segment_lazy<dtype, lztype>() : left(0), right(0) {}
	//segment_lazy<dtype, lztype>() : left(0), right(0), parent(0) {}
	segment_lazy<dtype, lztype>(int L, int R) : l(L), r(R), left(0), right(0) { make(); }
	//segment_lazy<dtype, lztype>(int L, int R) : l(L), r(R), left(0), right(0), parent(0) { make(); }

	void make() {
		value = 0; // ::fill:: identity value
		lazy = 0;  // ::fill:: identity lazy
		if (l < r) {
			int m = (l + r) / 2;
			left = new node(l, m);
			right = new node(m+1, r);
			//left->parent = this;
			//right->parent = this;
		}
	}

	inline dtype get_value() {
		return value + lazy * (r-l+1);
		// lazy to value operator
	}

	void propagate() {
		if (lazy == 0) return; // ::fill:: identity lazy
		if (left) {
			left->lazy += lazy; 
			// ::fill:: lazy update operator
			right->lazy += lazy;
			// ::fill:: lazy update operator
		}
		value = get_value();
		lazy = 0; // ::fill:: identity lazy
	}

	dtype update(int p, dtype v) {
		if (l <= p && p <= r) {
			propagate();
			if (left) {
				value = (left->update(p, v) + right->update(p, v));
				// ::fill:: sum operator
			} else {
				value = v; // ::fill:: update operator
			}
		}
		return get_value();
	}

	dtype update_interval(int li, int ri, lztype lz) {
		if (li <= l && r <= ri) {
			lazy += lz;
			// ::fill:: lazy update operator
		} else if (r < li || ri < l) {
		} else {
			propagate();
			dtype lv = left->update_interval(li, ri, lz);
			dtype rv = right->update_interval(li, ri, lz);
			value = lv + rv;
			// ::fill:: sum operator
		}
		return get_value();
	}

	dtype sum(int li, int ri) {
		if (li <= l && r <= ri) return get_value();
		else if (r < li || ri < l) return 0; // ::fill:: identity
		else {
			propagate();
			return (left->sum(li, ri) + right->sum(li, ri));
			// ::fill:: sum operator
		}
	}
};
#endif

#define SEGMENT_TREE_LAZY_NODE_DEFINED
//////// ---- Woosung Song's Source Code ---- ////////


typedef long long ll;

int main() {
	int n, m, k;
	scanf("%d%d%d", &n, &m, &k);
	m += k;

	segment_tree_lazy<ll, ll> seg(1, n);
	for (int i=1; i<=n; i++) {
		ll a; scanf("%lld",&a);
		seg.update(i, a);
	}

	while (m--) {
		ll a, b, c; scanf("%lld%lld%lld", &a, &b, &c);
		if (a == 1) {
			ll d; scanf("%lld", &d);
			seg.update_interval(b, c, d);
		} else {
			printf("%lld\n", seg.sum(b, c));
		}
	}
}
