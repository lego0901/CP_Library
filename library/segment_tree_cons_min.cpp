#include <bits/stdc++.h>

using namespace std;

//////// ---- Woosung Song's Source Code ---- ////////
#ifndef SEGMENT_TREE_CONS_MIN_DEFINED
template <class dtype>
class segment_tree_cons_min {
	private:
		typedef tuple<dtype,dtype,dtype,dtype> sgtype;
			// sum, left cons sum, right cons sum, max cons sum
		dtype DID = numeric_limits<dtype>::max()/2;
		dtype DINIT = 0; // SEGMENT TREE Initial value
		sgtype SGINIT = {DINIT, DINIT, DINIT, DINIT};

		inline void update_from_sib(int tn) {/*{{{*/
			get<0>(tree[tn]) = get<0>(tree[tn*2]) + get<0>(tree[tn*2+1]);
			get<1>(tree[tn]) = min(get<1>(tree[tn*2]), get<0>(tree[tn*2]) + get<1>(tree[tn*2+1]));
			get<2>(tree[tn]) = min(get<2>(tree[tn*2+1]), get<2>(tree[tn*2]) + get<0>(tree[tn*2+1]));
			get<3>(tree[tn]) = min(min(get<3>(tree[tn*2]), get<3>(tree[tn*2+1])),
								   get<2>(tree[tn*2]) + get<1>(tree[tn*2+1]));
		}

		void update(int tn, int l, int r, int p, dtype v) {
			if (l <= p && p <= r) {
				if (l < r) {
					int m = (l + r) / 2;
					update(tn*2, l, m, p, v);
					update(tn*2+1, m+1, r, p, v);
					update_from_sib(tn);
				} else {
					//tree[tn] = {v, v, v, v};
					get<0>(tree[tn]) += v;
					get<1>(tree[tn]) += v;
					get<2>(tree[tn]) += v;
					get<3>(tree[tn]) += v;
				}
			}
		}

		dtype naive_sum(int tn, int l, int r, int ql, int qr) {
			if (ql <= l && r <= qr) return get<0>(tree[tn]);
			else if (r < ql || qr < l) return 0;
			else {
				int m = (l + r) / 2;
				return naive_sum(tn*2, l, m, ql, qr) + naive_sum(tn*2+1, m+1, r, ql, qr);
			}
		}

		dtype left_min_sum(int tn, int l, int r, int ql, int qr) {
			if (ql <= l && r <= qr) return get<1>(tree[tn]);
			else if (r < ql || qr < l) return DID;
			else {
				int m = (l + r) / 2;
				return min(left_min_sum(tn*2, l, m, ql, qr),
						   naive_sum(tn*2, l, m, ql, qr) + left_min_sum(tn*2+1, m+1, r, ql, qr));
			}
		}

		dtype right_min_sum(int tn, int l, int r, int ql, int qr) {
			if (ql <= l && r <= qr) return get<2>(tree[tn]);
			else if (r < ql || qr < l) return DID;
			else {
				int m = (l + r) / 2;
				return min(right_min_sum(tn*2+1, m, r, ql, qr),
						   right_min_sum(tn*2, l, m, ql, qr) + naive_sum(tn*2+1, m+1, r, ql, qr));
			}
		}

		dtype _lmx(int tn, int l, int r, int qr) {
			if (r <= qr) return get<1>(tree[tn]);
			else if (l > qr) return DID;
			else {
				int m = (l + r) / 2;
				return min(_lmx(tn*2, l, m, qr), get<0>(tree[tn*2]) + _lmx(tn*2+1, m+1, r, qr));
			}
		}

		dtype _rmx(int tn, int l, int r, int ql) {
			if (ql <= l) return get<2>(tree[tn]);
			else if (ql > r) return DID;
			else {
				int m = (l + r) / 2;
				return min(_rmx(tn*2+1, m+1, r, ql), get<0>(tree[tn*2+1]) + _rmx(tn*2, l, m, ql));
			}
		}

		dtype min_sum(int tn, int l, int r, int ql, int qr) {
			if (ql <= l && r <= qr) return get<3>(tree[tn]);
			else if (r < ql || qr < l) return DID;
			else {
				int m = (l + r) / 2;
				return min(min(min_sum(tn*2, l, m, ql, qr), min_sum(tn*2+1, m+1, r, ql, qr)),
						   _rmx(tn*2, l, m, ql) + _lmx(tn*2+1, m+1, r, qr));
			}
		}/*}}}*/

	public:
		int min_index, max_index;
		int size;
		vector<sgtype> tree;

		segment_tree_cons_min<dtype>() {}
		segment_tree_cons_min<dtype>(int min_index, int max_index) {
			assert(min_index <= max_index);
			this->min_index = min_index, this->max_index = max_index;
			for (size=max_index-min_index+1; size&(size-1); size+=size&(-size));
			size <<= 1;
			tree.assign(size, SGINIT);
		}

		void init() {
			for (int i=0; i<size; i++) {
				tree[i] = SGINIT;
			}
		}

		void update(int p, dtype v) {
			// A[p] := v
			assert(min_index <= p && p <= max_index);
			update(1, 0, max_index-min_index, p-min_index, v);
		}

		dtype naive_sum(int l, int r) {
			// A[l] + A[l+1] + A[l+2] + ... + A[r]
			assert(min_index <= l && l <= r && r <= max_index);
			return naive_sum(1, 0, max_index-min_index, l-min_index, r-min_index);
		}

		dtype left_min_sum(int l, int r) {
			// min_{l<=t<=r} (A[l] + A[l+1] + ... + A[t])
			assert(min_index <= l && l <= r && r <= max_index);
			return left_min_sum(1, 0, max_index-min_index, l-min_index, r-min_index);
		}

		dtype right_min_sum(int l, int r) {
			// min_{l<=t<=r} (A[t] + A[t+1] + ... + A[r])
			assert(min_index <= l && l <= r && r <= max_index);
			return right_min_sum(1, 0, max_index-min_index, l-min_index, r-min_index);
		}

		dtype min_sum(int l, int r) {
			// min_{l<=k<=t<=r} (A[k] + A[k+1] + ... + A[t])
			assert(min_index <= l && l <= r && r <= max_index);
			return min_sum(1, 0, max_index-min_index, l-min_index, r-min_index);
		}
};
#endif

#define SEGMENT_TREE_CONS_MIN_DEFINED
//////// ---- Woosung Song's Source Code ---- ////////

const int MAX_N = 3024;
typedef long long ll;

int N;
int X[MAX_N], Y[MAX_N];
ll W[MAX_N];
vector<int> YtoI[MAX_N];

int main() {
	scanf("%d", &N);

	vector<int> cx, cy;
	for (int i=1; i<=N; i++) {
		scanf("%d%d%lld", X+i, Y+i, W+i);
		cx.push_back(X[i]);
		cy.push_back(Y[i]);
	}
	sort(cx.begin(), cx.end());
	sort(cy.begin(), cy.end());
	cx.erase(unique(cx.begin(), cx.end()), cx.end());
	cy.erase(unique(cy.begin(), cy.end()), cy.end());

	for (int i=1; i<=N; i++) {
		X[i] = 1 + (lower_bound(cx.begin(), cx.end(), X[i]) - cx.begin());
		Y[i] = 1 + (lower_bound(cy.begin(), cy.end(), Y[i]) - cy.begin());
		YtoI[Y[i]].push_back(i);
	}

	int xsz = cx.size(), ysz = cy.size();

	segment_tree_cons_min<ll> seg(1, xsz);

	ll ans = 0;

	for (int y1=1; y1<=ysz; y1++) {
		for (int y2=y1; y2<=ysz; y2++) {
			for (int i : YtoI[y2]) {
				seg.update(X[i], W[i]);
			}
			ans = min(ans, seg.min_sum(1, xsz));
		}
		seg.init();
	}
	printf("%lld\n", ans);
}
