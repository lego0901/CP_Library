#include <bits/stdc++.h>

using namespace std;

//////// ---- Woosung Song's Source Code ---- ////////
#ifndef SEGMENT_TREE_MIN_DEFINED
template <class dtype>
class segment_tree_min {
	private:
		dtype SGID = numeric_limits<dtype>::max();

		dtype update(int tn, int l, int r, int p, dtype v) {
			if (l <= p && p <= r) {
				if (l < r) {
					int m = (l + r) / 2;
					tree[tn] = min(update(tn*2, l, m, p, v),update(tn*2+1, m+1, r, p, v));
				} else tree[tn] = v;
			}
			return tree[tn];
		}

		dtype sum(int tn, int l, int r, int ql, int qr) {
			if (ql <= l && r <= qr) return tree[tn];
			else if (r < ql || qr < l) return SGID;
			else {
				int m = (l + r) / 2;
				return min(sum(tn*2, l, m, ql, qr), sum(tn*2+1, m+1, r, ql, qr));
			}
		}

	public:
		int min_index, max_index;
		int size;
		vector<dtype> tree;

		segment_tree_min<dtype>() {}
		segment_tree_min<dtype>(int min_index, int max_index) {
			assert(min_index <= max_index);
			this->min_index = min_index, this->max_index = max_index;
			for (size=max_index-min_index+1; size&(size-1); size+=size&(-size));
			size <<= 1;
			tree.assign(size, SGID);
		}

		void update(int p, dtype v) {
			assert(min_index <= p && p <= max_index);
			update(1, 0, max_index-min_index, p-min_index, v);
		}

		dtype sum(int l, int r) {
			assert(min_index <= l && l <= r && r <= max_index);
			return sum(1, 0, max_index-min_index, l-min_index, r-min_index);
		}
};
#endif

#define SEGMENT_TREE_MIN_DEFINED
//////// ---- Woosung Song's Source Code ---- ////////

typedef long long ll;

int main() {
	int n, m, k;
	scanf("%d%d%d", &n,&m,&k);
	m += k;
	segment_tree_min<ll> seg(1, n);
	for (int i=1; i<=n; i++) {
		ll a; scanf("%lld", &a);
		seg.update(i, a);
	}

	while (m--) {
		ll a, b, c; scanf("%lld%lld%lld", &a, &b, &c);
		if (a == 1) seg.update(b, c);
		else printf("%lld\n", seg.sum(b, c));
	}
}
