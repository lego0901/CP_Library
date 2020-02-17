#include <bits/stdc++.h>

using namespace std;

//////// ---- Woosung Song's Source Code ---- ////////
#ifndef SPARSE_TABLE_2D_MAX
// should be 1-base
class sparse_table_2d_max {
	private:
		static const int MAX_N = 510, LOG_N = 10; // should be modified!!
		typedef long long dtype;
		const dtype STID = numeric_limits<dtype>::min();

		inline int coverage(int bit) {
			return (1<<bit) - 1;
		}
		inline int coverage(int s, int bit) {
			return s + coverage(bit);
		}

	public:
		int n, m, lgn;
		dtype table[MAX_N][MAX_N][LOG_N][LOG_N];
		dtype data[MAX_N][MAX_N];

		sparse_table_2d_max(int n, int m) {
			this->n = n, this->m = m;
			lgn = 1;
			while (coverage(1, lgn) <= max(n, m)) lgn++;
			assert(n <= MAX_N && m <= MAX_N && lgn <= LOG_N);
			for (int i=1; i<=n; i++) {
				for (int j=1; j<=m; j++) {
					data[i][j] = STID;
					for (int bi=0; bi<LOG_N; bi++) {
						for (int bj=0; bj<LOG_N; bj++) {
							table[i][j][bi][bj] = STID;
						}
					}
				}
			}
		}

		void set_element(int r, int c, dtype v) {
			data[r][c] = v;
		}

		void print() {
			for (int i=1; i<=n; i++) {
				for (int j=1; j<=m; j++) {
					cout << data[i][j] << ' ';
				}
				cout << endl;
			}
		}

		void make() {
			for (int r=n; r>=1; r--) {
				for (int c=m; c>=1; c--) {
					table[r][c][0][0] = data[r][c];
					for (int br=0; br<lgn; br++) {
						if (coverage(r, br) > n) break;
						int nr = br? r + (1<<(br-1)) : r;
						for (int bc=0; bc<lgn; bc++) {
							if (coverage(c, bc) > m) break;
							int nc = bc? c + (1<<(bc-1)) : c;
							int pbr = br? br-1 : br, pbc = bc? bc-1 : bc;

							table[r][c][br][bc] =
								max(table[r][c][pbr][pbc],
								  max(table[r][nc][pbr][pbc],
								    max(table[nr][c][pbr][pbc],
										table[nr][nc][pbr][pbc])));
						}
					}
				}
			}
		}

		dtype max_value(int rs, int cs, int rf, int cf) {
			// max value in [rs, rf] x [cs, cf]
			int br = 0, bc = 0;
			while (coverage(rs, br+1) <= rf) br++;
			while (coverage(cs, bc+1) <= cf) bc++;

			int rcv = coverage(br), ccv = coverage(bc);
			return
				max(table[rs][cs][br][bc],
				  max(table[rf-rcv][cs][br][bc],
				    max(table[rs][cf-ccv][br][bc],
						table[rf-rcv][cf-ccv][br][bc])));
		}
};
#endif

#define SPARSE_TABLE_2D_MAX
//////// ---- Woosung Song's Source Code ---- ////////

sparse_table_2d_max spt(10, 10);

int main() {
	for (int i=1; i<=10; i++) {
		for (int j=1; j<=10; j++) {
			spt.set_element(i, j, (i+j)%10);
		}
	}
	spt.make();
	while (1) {
		spt.print();
		int rs, cs, rf, cf;
		cin >> rs >> cs >> rf >> cf;
		cout << spt.max_value(rs, cs, rf, cf) << endl;
	}
}
