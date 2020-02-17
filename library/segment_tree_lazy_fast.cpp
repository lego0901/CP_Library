#include <bits/stdc++.h>

using namespace std;


//////// ---- Woosung Song's Source Code ---- ////////

// segment tree with lazy propagation
// supports update, update_interval, sum

#define STL_ADD_OP(a, b)                  ((a) + (b))
#define STL_ADD_ID                        0
#define STL_LAZY_MELT(lz, l, r, s)        ((s) + ((r)-(l)+1)*(lz))
#define STL_LAZY_MERGE(lz_p, lz, l, r, t) ((lz_p) + (lz))
#define STL_LAZY_ID                       0

/* helpers for templates */
#define STL_MIN 0x80000000
#define STL_MAX 0x7FFFFFFF
// #define STL_MIN 0x8000000000000000
// #define STL_MAX 0x7FFFFFFFFFFFFFFF


/******* templates *******/
// sum: + elements, lazy: + elements in [l, r] to v value
/*
#define STL_ADD_OP(a, b)                  ((a) + (b))
#define STL_ADD_ID                        0
#define STL_LAZY_MELT(lz, l, r, s)        ((s) + ((r)-(l)+1)*(lz))
#define STL_LAZY_MERGE(lz_p, lz, l, r, t) ((lz_p) + (lz))
#define STL_LAZY_ID                       0
*/

// sum: + elements, lazy: set elements in [l, r] to v value
/*
#define STL_ADD_OP(a, b)                  ((a) + (b))
#define STL_ADD_ID                        0
#define STL_LAZY_MELT(lz, l, r, s)        (((r)-(l)+1)*(lz))
#define STL_LAZY_MERGE(lz_p, lz, l, r, t) (lz_p)
#define STL_LAZY_ID                       0
*/

// sum: + elements, lazy: set elements in [l, r] to v, v+k, v+2k, .. v+(r-l)k values
// lazy_type: pair<int,int> or pair<ll,ll>; stores (v, k).
/*
#define STL_ADD_OP(a, b)                  ((a) + (b))
#define STL_ADD_ID                        0
#define STL_LAZY_MELT(lz, l, r, s)        ((s) + 1ll*(lz).first*((r)-(l)+1) + (lz).second*((r)-(l)+1ll)*((r)-(l))/2)
#define STL_LAZY_MERGE(lz_p, lz, l, r, t) {(lz_p).first + ((t)&1)*((r)-(l)+1ll)*(lz_p).second, (lz_p).second + (lz).second}
#define STL_LAZY_ID                       {0, 0}
*/

// sum: max elements, lazy: maximize elements in [l, r] with v value
/*
#define STL_ADD_OP(a, b)                  (max((a), (b)))
#define STL_ADD_ID                        STL_MIN
#define STL_LAZY_MELT(lz, l, r, s)        (max((lz), (s)))
#define STL_LAZY_MERGE(lz_p, lz, l, r, t) (max((lz_p), (lz)))
#define STL_LAZY_ID                       STL_MIN
*/

// sum: max elements, lazy: set elements in [l, r] to v value
/*
#define STL_ADD_OP(a, b)                  (max((a), (b)))
#define STL_ADD_ID                        STL_MIN
#define STL_LAZY_MELT(lz, l, r, s)        (lz)
#define STL_LAZY_MERGE(lz_p, lz, l, r, t) (lz_p)
#define STL_LAZY_ID                       STL_MIN
*/

// sum: max elements, lazy: set elements in [l, r] to v, v+k, v+2k, .. v+(r-l)k values
// lazy_type: pair<int,int> or pair<ll,ll>; stores (v, k).
#define STL_ADD_OP(a, b)                  (max((a), (b)))
#define STL_ADD_ID                        0
#define STL_LAZY_MELT(lz, l, r, s)        ((lz).second > 0? (
#define STL_LAZY_MERGE(lz_p, lz, l, r, t) {(lz_p).first + ((t)&1)*((r)-(l)+1ll)*(lz_p).second, (lz_p).second + (lz).second}
#define STL_LAZY_ID                       {0, 0}



#ifndef SEGMENT_TREE_LAZY_FAST_DEFINED
template <class sum_type, class lazy_type>
class segment_tree_lazy_fast {
	private:
		int tn = 0;

	public:
		int max_index, min_index;
		int size, limit;
		vector<pair<int, int> > bounds;
		vector<bool> wait_to_propagate;
		vector<sum_type> v;
		vector<lazy_type> lazy;

		segment_tree_lazy_fast<sum_type, lazy_type>() {}

		segment_tree_lazy_fast<sum_type, lazy_type>(int max_index, int min_index=-10) {
			this->max_index = max_index, this->min_index = min_index;/*{{{*/
			assert(min_index <= max_index);

			limit = max_index - min_index + 1;
			for (int b=1; (1<<b)<limit; b++) size = 1<<(b+1);
			limit = size, size = size + size;

			v.assign(size, STL_ADD_ID);
			lazy.assign(size, STL_LAZY_ID);
			wait_to_propagate.assign(size, 0);
			bounds.assign(size, {0, 0});
			__make(1, limit);
		}/*}}}*/

	private:
		inline sum_type __node_value(int t) {/*{{{*/
			int l, r; tie(l, r) = bounds[t];
			return wait_to_propagate[t]? STL_LAZY_MELT(lazy[t], l, r, v[t]) : v[t];
		}/*}}}*/

		void __make(int l, int r) {/*{{{*/
			int t = ++tn;
			bounds[t] = {l, r};
			for (tn=2; ; tn++) {
				int p = tn >> 1;
				int l, r; tie(l, r) = bounds[p];
				if (l == r) break;
				int m = (l + r) >> 1;
				if (tn & 1) bounds[tn] = {m+1, r};
				else bounds[tn] = {l, m};
			}
		}/*}}}*/

		void __propagate(int t) {/*{{{*/
			int l, r; tie(l, r) = bounds[t];
			if (!wait_to_propagate[t]) return;
			if (l < r) {
				int left = 2*t, right = left+1;
				lazy[left] = wait_to_propagate[left]?
					STL_LAZY_MERGE(lazy[left], lazy[t]) : lazy[t];
				lazy[right] = wait_to_propagate[right]?
					STL_LAZY_MERGE(lazy[right], lazy[t]) : lazy[t];
				wait_to_propagate[left] = 1;
				wait_to_propagate[right] = 1;
				v[t] = __node_value(t);
				lazy[t] = STL_LAZY_ID; wait_to_propagate[t] = 0;
			} else {
				v[t] = __node_value(t);
				lazy[t] = STL_LAZY_ID; wait_to_propagate[t] = 0;
			}
		}/*}}}*/

		void __propagate_all(int t=1) {/*{{{*/
			int l, r; tie(l, r) = bounds[t];
			__propagate(t);
			if (l < r) {
				__propagate_all(t*2);
				__propagate_all(t*2+1);
			}
		}/*}}}*/

		sum_type __update_interval(int pl, int pr, lazy_type x, int t=1) {/*{{{*/
			int l, r; tie(l, r) = bounds[t];
			if (pl <= l && r <= pr) {
				__propagate(t);
				lazy[t] = x; wait_to_propagate[t] = 1;
				return __node_value(t);
			} else if (pl > r || l > pr) {
				return __node_value(t);
			} else {
				__propagate(t);
				return v[t] = STL_ADD_OP(__update_interval(pl, pr, x, t*2),
						__update_interval(pl, pr, x, t*2+1));
			}
		}/*}}}*/

		sum_type __sum(int pl, int pr, int t=1) {/*{{{*/
			int l, r; tie(l, r) = bounds[t];
			if (pl <= l && r <= pr) {
				return __node_value(t);
			} else if (pl > r || l > pr) {
				return STL_ADD_ID;
			} else {
				__propagate(t);
				return STL_ADD_OP(__sum(pl, pr, t*2),
						__sum(pl, pr, t*2+1));
			}
		}/*}}}*/

		sum_type __set_element(int p, sum_type x, int t=1, bool init=0) {/*{{{*/
			int l, r; tie(l, r) = bounds[t];
			if (p == l && p == r) {
				if (!init) wait_to_propagate[t] = 0, lazy[t] = STL_LAZY_ID;
				return v[t] = x;
			} else if (l > p || r < p) {
				return v[t];
			} else {
				if (!init && wait_to_propagate[t]) __propagate(t);
				return v[t] = STL_ADD_OP(__set_element(p, x, t*2, init),
						__set_element(p, x, t*2+1, init));
			}
		}/*}}}*/

	public:
		sum_type get(int p) {/*{{{*/
			return sum(p, p);
		}/*}}}*/

		void set_element(int p, sum_type x, bool init=0) {/*{{{*/
			p = p - min_index + 1;
			__set_element(p, x, 1, init);
		}/*}}}*/

		void update_interval(int l, int r, lazy_type x) {/*{{{*/
			assert(min_index <= l && l <= r && r <= max_index);
			l = l - min_index + 1, r = r - min_index + 1;
			__update_interval(l, r, x);
		}/*}}}*/

		sum_type sum(int l, int r) {/*{{{*/
			assert(min_index <= l && l <= r && r <= max_index);
			l = l - min_index + 1, r = r - min_index + 1;
			return __sum(l, r);
		}/*}}}*/

		void print(int t=1) {/*{{{*/
			int l, r; tie(l, r) = bounds[t];
			cout << "(l, r): (" << l << ", " << r << "),  "
				 << "(sum): (" << v[t] << "),  "
				 << "(wlz, lz): (" << wait_to_propagate[t] << ", " << lazy[t] << ")" << endl;
			if (l<r) {
				print(t*2);
				print(t*2+1);
			}
		}/*}}}*/

};
#endif

#define SEGMENT_TREE_LAZY_FAST_DEFINED

//////// ---- Woosung Song's Source Code ---- ////////


int main() {
	int lazy_id = numeric_limits<int>::min();

	segment_tree_lazy_fast<int,int> seg(64, 1);

	seg.update_interval(1, 50, 2);
	cout << seg.sum(1, 50) << endl;
	seg.update_interval(3, 7, 3);
	seg.update_interval(4, 6, -1);
	seg.print();
	// 2 2 3 -1 -1 -1 3 2 2 2
	cout << seg.sum(2, 7) << endl; // 5
	cout << seg.sum(1, 10) << endl; // 13
	for (int i=1; i<=11; i++) {
		cout << i << ' ' << seg.sum(i, i) << endl;
	}
}
