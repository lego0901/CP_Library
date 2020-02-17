#include <bits/stdc++.h>

using namespace std;

//////// ---- Woosung Song's Source Code ---- ////////

// an efficient data structure that implements union-find
// supports unio(returns 1 when separated components are merged) and find

#ifndef UNION_FIND_SIZE_DEFINED
class union_find_size {
	public:
		int max_node;
		vector<int> parents;
		vector<int> sizes;

		union_find_size(int max_node) {/*{{{*/
			this->max_node = max_node;
			parents.assign(max_node+1, 0);
			sizes.assign(max_node+1, 1);
			for (int i=0; i<=max_node; i++)
				parents[i] = i;
		}/*}}}*/

		int find(int a) {/*{{{*/
			return parents[a] == a? a : parents[a] = find(parents[a]);
		}/*}}}*/

		bool unio(int a, int b) {/*{{{*/
			if ((a = find(a)) != (b = find(b))) {
				sizes[b] += sizes[a];
				parents[a] = b;
			}
			return a != b;
		}/*}}}*/

		int size_of(int a) {/*{{{*/
			return sizes[find(a)];
		}/*}}}*/

		void print() {/*{{{*/
			for (int i=1; i<=max_node; i++)
				cout << find(i) << ' ';
			cout << endl;
		}/*}}}*/
};
#endif

#define UNION_FIND_SIZE_DEFINED

//////// ---- Woosung Song's Source Code ---- ////////


int main() {
	union_find_size uf(10);
	int num_components = 10;
	return 0;
}
