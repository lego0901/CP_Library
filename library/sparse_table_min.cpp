#include <bits/stdc++.h>

using namespace std;

//////// ---- Woosung Song's Source Code ---- ////////
#ifndef SPARSE_TABLE_MIN
// should be 1-base
class sparse_table_min {
	private:
		static const int MAX_N = 1e5+10, LOG_N = 17; // should be modified!!
		typedef long long dtype;
		const dtype STID = numeric_limits<dtype>::max();
		
		inline int coverage(int bit) {
			return (1<<bit) - 1;
		}
		inline int coverage(int s, int bit) {
			return s + coverage(bit);
		}

	public:
		int n, lgn;
		dtype table[MAX_N][LOG_N];
		dtype data[MAX_N];

		sparse_table_min(int n) {
			this->n = n;
			lgn = 1;
			while (coverage(1, lgn) <= n) lgn++;
			assert(n <= MAX_N && lgn <= LOG_N);
			for (int i=1; i<=n; i++) {
				data[i] = STID;
				for (int bi=0; bi<LOG_N; bi++) {
					table[i][bi] = STID;
				}
			}
		}

		void set_element(int i, dtype v) {
			data[i] = v;
		}

		void make() {
			for (int i=n; i>=1; i--) {
				table[i][0] = data[i];
				for (int b=0; b<lgn; b++) {
					if (coverage(i, b) > n) break;
					int ni = b? i + (1<<(b-1)) : i;
					int pb = b? b-1 : b;
					table[i][b] =
						min(table[i][pb], table[ni][pb]);
				}
			}
		}

		dtype min_value(int l, int r) {
			int b = 0;
			while (coverage(l, b+1) <= r) b++;
			return min(table[l][b], table[r-coverage(b)][b]);
		}

};
#endif

#define SPARSE_TABLE_MIN
//////// ---- Woosung Song's Source Code ---- ////////

sparse_table_min spt(100);

int main() {
	for (int i=1; i<=10; i++) {
		cout << i << "th element? ";
		int a;
		cin >> a;
		spt.set_element(i, a);
	}
	spt.make();
	while (1) {
		int l, r;
		cin >> l >> r;
		cout << spt.min_value(l, r) << endl;
	}
}
