#include <bits/stdc++.h>

using namespace std;


//////// ---- Woosung Song's Source Code ---- ////////

// binary indexed tree with operators.

#ifndef BINARY_INDEXED_TREE_DEFINED
template <class sum_type>
class binary_indexed_tree {
	private:
		typedef sum_type (*sum_op) (sum_type, sum_type);
		sum_op DEFAULT_ADD_OP = [](sum_type a, sum_type b) { return a + b; };
		sum_type DEFAULT_ADD_ID = 0;
	
	public:
		int max_index, min_index;
		int size;
		vector<sum_type> v;

		sum_op add_op = DEFAULT_ADD_OP;
		sum_type add_id = DEFAULT_ADD_ID;

		binary_indexed_tree<sum_type>() {}

		binary_indexed_tree<sum_type>(int min_index, int max_index,
				sum_op add_op=0, sum_type add_id=0) {
			this->max_index = max_index, this->min_index = min_index;
			if (add_op) this->add_op = add_op, this->add_id = add_id;
			assert(min_index <= max_index);
			for (size=1; size<max_index-min_index+1; size<<=1);
			v.assign(size * 2, add_id);
		}

		void update(int p, sum_type x) {
			assert(min_index <= p && p <= max_index);
			v[p=p-min_index+size] = x;
			for (p>>=1; p; p>>=1) {
				v[p] = add_op(v[p+p], v[p+p+1]);
			}
		}

		sum_type sum(int l, int r) {
			assert(min_index <= l && l <= r && r <= max_index);
			sum_type s = add_id;
			l = l - min_index + size, r = r - min_index + size;
			for (; l<=r; l>>=1, r>>=1) {
				if (l & 1) s = add_op(s, v[l++]);
				if ((r+1)&1) s = add_op(s, v[r--]);
			}
			return s;
		}
};
#endif

#define BINARY_INDEXED_TREE_DEFINED

//////// ---- Woosung Song's Source Code ---- ////////


//// test code for binary indexed tree

//auto add_op = [](int a, int b) { return a+b; };
auto add_op = [](int a, int b) { return min(a, b); };
//int add_id = 0;
int add_id = numeric_limits<int>::max();

vector<int> brute_force_v;

int brute_force(int l, int r) {
	int s = add_id;
	for (int i=l; i<=r; i++)
		s = add_op(s, brute_force_v[i]);
	return s;
}

int generate_index(int min_index, int max_index) {
	return rand() % (max_index - min_index + 1) + min_index;
}

int main() {
	int min_index, max_index; cin >> min_index >> max_index;
	int min_v, max_v; cin >> min_v >> max_v;
	int iteration; cin >> iteration;

	brute_force_v.assign(max_index+1, add_id);
	binary_indexed_tree<int> bit(min_index, max_index, add_op, add_id);

	srand(time(0));

	for (int it=0; it<iteration; it++) {
		int q = rand() % 2;
		if (q) {
			int x = generate_index(min_index, max_index);
			int y = generate_index(min_v, max_v);
			brute_force_v[x] = y;
			bit.update(x, y);
		} else {
			int x = generate_index(min_index, max_index);
			int y = generate_index(min_index, max_index);
			if (x>y) swap(x, y);
			if (brute_force(x, y) != bit.sum(x, y)) {
				cout << "!!! " << x << ' ' << y << endl;
				for (int i=min_index; i<=max_index; i++) {
					if (brute_force_v[i] == add_id)
						cout << "id ";
					else
						cout << brute_force_v[i] << ' ';
				}
				cout << endl;
				cout << "bf: " << brute_force(x, y) << endl;
				cout << "bit: " << bit.sum(x, y) << endl;
				assert(0);
			}
		}
	}
	cout << "pretest passed" << endl;
}
