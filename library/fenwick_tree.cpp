#include <bits/stdc++.h>

using namespace std;


//////// ---- Woosung Song's Source Code ---- ////////

// fenwick tree data structure with operators.
// supports add, sum, ...

#ifndef FENWICK_TREE_DEFINED
template <class sum_type>
class fenwick_tree {
	public:
		vector<sum_type> v;
		int max_index, min_index;
		int size;
	
		fenwick_tree<sum_type>(int min_index, int max_index) {
			this->max_index = max_index;
			this->min_index = min_index;
			size = max_index - min_index + 3;
			v.assign(size, 0); // ::fill:: identity
		}

		void init() {
			fill(v.begin(), v.end(), 0); // ::fill:: identity
		}

		void add(int p, sum_type x) {
			// add pth element to x.
			assert(p >= min_index-1  && p <= max_index);
			for (p=p-min_index+2; p<size; p+=p&-p) {
				v[p] += x;
				// ::fill:: sum operator
			}
		}

		sum_type sum(int p) {
			// sum of the first element to pth.
			assert(p >= min_index-1  && p <= max_index);
			sum_type s = 0;
			for (p=p-min_index+2; p; p&=p-1) {
				s += v[p];
				// ::fill:: sum operator
			}
			return s;
		}

		sum_type sum(int a, int b) {
			sum_type s = sum(b) - sum(a-1);
			// ::fill:: sum operator
			return s;
		}
};
#endif

#define FENWICK_TREE_DEFINED

//////// ---- Woosung Song's Source Code ---- ////////


int main() {
	fenwick_tree<long long> fw(1, 1e6);
	fw.add(5, 10);
	fw.add(8, 12);
	fw.add(10, 13);
	fw.add(11, -2);
	cout << fw.sum(2, 9) << endl;
	cout << fw.sum(5, 11) << endl;
	cout << fw.sum(5, 10) << endl;
}
