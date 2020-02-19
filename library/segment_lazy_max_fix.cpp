#include <bits/stdc++.h>

using namespace std;

//////// ---- Woosung Song's Source Code ---- ////////
#ifndef SEGMENT_LAZY_MAX_FIX_DEFINED
template <class dtype, class ltype>
class segment_lazy_max_fix {
	private:
		dtype SGID = numeric_limits<dtype>::min(); // MAX
		ltype LZID = 0; // FIX, No use 

		inline dtype value(int tn, int l, int r) {
			(void) l; (void) r;
			return prop[tn]? (dtype) lazy[tn] : tree[tn];
		}

		void propagate(int tn, int l, int r) {
			if (!prop[tn]) return;
			tree[tn] = value(tn, l, r);
			if (l < r) {
				lazy[tn*2] = lazy[tn];
				lazy[tn*2+1] = lazy[tn];
				prop[tn*2] = prop[tn*2+1] = 1;
			}
			lazy[tn] = LZID;
			prop[tn] = 0;
		}

		dtype update_interval(int tn, int l, int r, int ql, int qr, ltype lz) {
			if (ql <= l && r <= qr) {
				lazy[tn] = lz;
				prop[tn] = 1;
			} else if (r < ql || qr < l) {
			} else {
				propagate(tn, l, r);
				int m = (l + r) / 2;
				tree[tn] = max(update_interval(tn*2, l, m, ql, qr, lz),
					update_interval(tn*2+1, m+1, r, ql, qr, lz));
			}
			return value(tn, l, r);
		}

		dtype sum(int tn, int l, int r, int ql, int qr) {
			if (ql <= l && r <= qr) return value(tn, l, r);
			else if (r < ql || qr < l) return SGID;
			else {
				propagate(tn, l, r);
				int m = (l + r) / 2;
				return max(sum(tn*2, l, m, ql, qr), sum(tn*2+1, m+1, r, ql, qr));
			}
		}

	public:
		int min_index, max_index;
		int size;
		vector<dtype> tree;
		vector<ltype> lazy;
		vector<bool> prop;

		segment_lazy_max_fix<dtype, ltype>() {}
		segment_lazy_max_fix<dtype, ltype>(int min_index, int max_index) {
			assert(min_index <= max_index);
			this->min_index = min_index, this->max_index = max_index;
			for (size=max_index-min_index+1; size&(size-1); size+=size&(-size));
			size <<= 1;
			tree.assign(size, SGID);
			lazy.assign(size, LZID);
			prop.assign(size, 0);
		}

		void update_interval(int l, int r, dtype v) {
			assert(min_index <= l && l <= r && r <= max_index);
			update_interval(1, 0, max_index-min_index, l-min_index, r-min_index, v);
		}

		dtype sum(int l, int r) {
			assert(min_index <= l && l <= r && r <= max_index);
			return sum(1, 0, max_index-min_index, l-min_index, r-min_index);
		}
};
#endif

#define SEGMENT_LAZY_MAX_FIX_DEFINED
//////// ---- Woosung Song's Source Code ---- ////////

typedef long long ll;

int main() {
	int n, m, k;
	scanf("%d%d%d", &n,&m,&k);
	m += k;
	segment_lazy_max_fix<ll,ll> seg(1, n);
	for (int i=1; i<=n; i++) {
		ll a; scanf("%lld", &a);
		seg.update_interval(i, i, a);
	}

	while (m--) {
		ll a, b, c, d; scanf("%lld%lld%lld", &a, &b, &c);
		if (a == 1) {
			scanf("%lld", &d);
			seg.update_interval(b, c, d);
		}
		else printf("%lld\n", seg.sum(b, c));
	}
}
