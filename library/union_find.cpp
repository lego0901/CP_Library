#include <bits/stdc++.h>

using namespace std;

//////// ---- Woosung Song's Source Code ---- ////////

// an efficient data structure that implements union-find
// supports unio(returns 1 when separated components are merged) and find

#ifndef UNION_FIND_DEFINED
class union_find {
	public:
		int max_node;
		vector<int> parents;

		union_find(int max_node) {/*{{{*/
			this->max_node = max_node;
			parents.assign(max_node+1, 0);
			for (int i=0; i<=max_node; i++)
				parents[i] = i;
		}/*}}}*/

		void init() {/*{{{*/
			for (int i=0; i<=max_node; i++)
				parents[i] = i;
		}/*}}}*/

		int find(int a) {/*{{{*/
			return parents[a] == a? a : parents[a] = find(parents[a]);
		}/*}}}*/

		bool unio(int a, int b) {/*{{{*/
			parents[a = find(a)] = b = find(b);
			return a != b;
		}/*}}}*/

		void print() {/*{{{*/
			for (int i=1; i<=max_node; i++)
				cout << find(i) << ' ';
			cout << endl;
		}/*}}}*/
};
#endif

#define UNION_FIND_DEFINED

//////// ---- Woosung Song's Source Code ---- ////////


int main() {
	union_find uf(10);
	int num_components = 10;
	while (1) {
		int a, b; cin >> a >> b;
		if (uf.unio(a, b)) {
			cout << "united!" << endl;
			cout << "number of components: " << (--num_components) << endl;
		} else {
			cout << "not united" << endl;
			cout << "number of components: " << num_components << endl;
		}
	}
	return 0;
}
