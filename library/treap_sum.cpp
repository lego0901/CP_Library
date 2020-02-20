#include <bits/stdc++.h>

using namespace std;


//////// ---- Woosung Song's Source Code ---- ////////

// an efficient data structure that implements binary tree
// supports kth, count_greator, sum_greater ..., and e.t.c.
// `sum' operator is enabled

#ifndef TREAP_SUM_DEFINED
template <class element_type, class sum_type>
class __treap_sum_node {/*{{{*/
	public:
		element_type key;
		sum_type sum;
		int priority, num_nodes, count;
		__treap_sum_node *left, *right;

		__treap_sum_node(element_type k, int cnt=1) : key(k), sum(cnt * ((sum_type) k)), priority(rand()), num_nodes(cnt), count(cnt), left(0), right(0) {}
		void calc_num_node() { num_nodes = count + (left? left->num_nodes : 0) + (right? right->num_nodes : 0); }
		void calc_sum() { sum = count * key + (left? left->sum : 0) + (right? right->sum : 0); }
		void set_left(__treap_sum_node *t) { left = t; calc_num_node(); calc_sum(); }
		void set_right(__treap_sum_node *t) { right = t; calc_num_node(); calc_sum(); }
};/*}}}*/

template <class element_type, class sum_type>
using __treap_sum_pair = pair<__treap_sum_node<element_type, sum_type>*, __treap_sum_node<element_type, sum_type>* >;

template <class element_type, class sum_type>
struct treap_sum {
	private:
		typedef __treap_sum_node<element_type, sum_type> node;/*{{{*/
		typedef __treap_sum_pair<element_type, sum_type> node_pair;

		node *root;
		int num_nodes;/*}}}*/

	public:
		const static element_type ELEMENT_MAX = numeric_limits<element_type>::max();
		const static element_type ELEMENT_MIN = numeric_limits<element_type>::min();
		sum_type sum;
		bool enable_duplicates;
		treap_sum(bool _enable_duplicates=1) : root(0), num_nodes(0), sum(0), enable_duplicates(_enable_duplicates) {}

	private:
		node_pair* __splited(node *r, element_type key) {/*{{{*/
			if (!r) return new node_pair(0, 0);
			if (r->key < key) {
				node_pair *p = __splited(r->right, key);
				r->set_right(p->first);
				return new node_pair(r, p->second);
			} else {
				node_pair *p = __splited(r->left, key);
				r->set_left(p->second);
				return new node_pair(p->first, r);
			}
		}

		node* __find_node(node *r, element_type key, int inc=0) {
			if (!r) return 0;
			if (r->key == key) {
				if (inc) r->count += inc, r->calc_num_node(), r->calc_sum();
				return r;
			}
			if (r->key < key) {
				if (inc) r->num_nodes += inc, r->sum += inc * key;
				return __find_node(r->right, key, inc);
			}
			if (inc) r->num_nodes += inc, r->sum += inc * key;
			return __find_node(r->left, key, inc);
		}

		node* __insert_node(node *r, node *n) {
			if (!r) return n;
			if (r->priority < n->priority) {
				node_pair *p = __splited(r, n->key);
				n->set_left(p->first);
				n->set_right(p->second);
				return n;
			}
			if (n->key < r->key) r->set_left(__insert_node(r->left, n));
			else r->set_right(__insert_node(r->right, n));
			return r;
		}

		node* __merge_node(node *n1, node *n2) {
			if (!n1) return n2;
			else if (!n2) return n1;
			else if (n1->priority < n2->priority) {
				n2->set_left(__merge_node(n1, n2->left));
				return n2;
			} else {
				n1->set_right(__merge_node(n1->right, n2));
				return n1;
			}
		}

		node* __erase_node(node *n, element_type key) {
			if (!n) return n;
			if (n->key == key) {
				node* ret = __merge_node(n->left, n->right);
				delete n;
				return ret;
			}
			if (key < n->key) n->set_left(__erase_node(n->left, key));
			else n->set_right(__erase_node(n->right, key));
			return n;
		}

		node* __kth_node(node* n, int k) {
			int left = 0;
			if (n->left) left = n->left->num_nodes;
			if (k <= left) return __kth_node(n->left, k);
			if (left+1 <= k && k <= left+n->count) return n;
			return __kth_node(n->right, k-left-n->count);
		}

		element_type __kth_smallest_sum(node* n, int k) {
			int left = 0;
			element_type left_sum = 0;
			if (n->left) left = n->left->num_nodes, left_sum = n->left->sum;
			if (k <= left) return __kth_smallest_sum(n->left, k);
			if (left+1 <= k && k <= left+n->count) return (k-left)*(n->key) + left_sum;
			return left_sum + (n->count)*(n->key) + __kth_smallest_sum(n->right, k-left-n->count);
		}

		int __count_greater(node* n, element_type key) {
			if (!n) return 0;
			if (n->key <= key) return __count_greater(n->right, key);
			return n->count + (n->right? n->right->num_nodes : 0) + __count_greater(n->left, key);
		}

		int __count_less(node* n, element_type key) {
			if (!n) return 0;
			if (n->key >= key) return __count_less(n->left, key);
			return n->count + (n->left? n->left->num_nodes : 0) + __count_less(n->right, key);
		}

		element_type __sum_greater(node* n, element_type key) {
			if (!n) return 0;
			if (n->key <= key) return __sum_greater(n->right, key);
			return (n->right? n->right->sum : 0) + (n->count * n->key) + __sum_greater(n->left, key);
		}

		element_type __sum_less(node* n, element_type key) {
			if (!n) return 0;
			if (n->key >= key) return __sum_less(n->left, key);
			return (n->left? n->left->sum : 0) + (n->count * n->key) + __sum_less(n->right, key);
		}

		void __print_node(node *n) {
			if (n) {
				__print_node(n->left);
				//cout << (n->key) << '(' << (n->count) << ')' << ' ';
				for (int i=0; i<n->count; i++) cout << n->key << ' ';
				__print_node(n->right);
			}
		}

		node* __biggest(node *n) {
			if (n->right) return __biggest(n->right);
			else return n;
		}

		node* __smallest(node *n) {
			if (n->left) return __smallest(n->left);
			else return n;
		}

		element_type __supremum(node *n, element_type key) {
			if (!n) return ELEMENT_MAX;
			if (n->key == key) return key;
			else if (n->key < key) return __supremum(n->right, key);
			else return min(__supremum(n->left, key), n->key);
		}

		element_type __infimum(node *n, element_type key) {
			if (!n) return ELEMENT_MIN;
			if (n->key == key) return key;
			else if (n->key > key) return __infimum(n->left, key);
			else return max(__infimum(n->right, key), n->key);
		}

		void __disable_duplicates(node *n) {
			n->count = 1;
			if (n->left) __disable_duplicates(n->left);
			if (n->right) __disable_duplicates(n->right);
			n->calc_num_node();
			n->calc_sum();
		}

		void __vectorize(node *n, vector<element_type> &v) {
			if (n->left) __vectorize(n->left, v);
			for (int i=0; i<n->count; i++) v.push_back(n->key);
			if (n->right) __vectorize(n->right, v);
		}/*}}}*/
	
	public:
		int size() {/*{{{*/
			return num_nodes;
		}/*}}}*/

		bool empty() {/*{{{*/
			return num_nodes == 0;
		}/*}}}*/

		void print() {/*{{{*/
			// print the treap_sum tree in sorted order
			__print_node(root);
			cout << "(num_nodes: " << num_nodes << ", sum: " << sum << ")" << endl;
		}/*}}}*/

		void insert(element_type key, int cnt=1) {/*{{{*/
			// insert an element of the given key
			node *f = __find_node(root, key);
			if (!enable_duplicates) cnt = 1;
			if (f) {
				__find_node(root, key, enable_duplicates? cnt : 0);
			} else {
				node *n = new node(key, cnt);
				root = __insert_node(root, n);
			}
			num_nodes = root->num_nodes;
			sum = root->sum;
		}/*}}}*/

		bool erase(element_type key, int cnt=1) {/*{{{*/
			// erase an element with the given key
			// returns 1 iff tree has a node of the key
			node *f = __find_node(root, key);
			if (f) {
				if (f->count <= cnt) root = __erase_node(root, key);
				else __find_node(root, key, -cnt);
				num_nodes = root? root->num_nodes : 0;
				sum = root? root->sum : 0;
				return 1;
			} else return 0;
		}/*}}}*/

		bool erase_key(element_type key) {/*{{{*/
			// erase a tree node with the given key (for every count)
			// returns 1 iff tree has a node of the key
			node *f = __find_node(root, key);
			if (f) {
				root = __erase_node(root, key);
				num_nodes = root? root->num_nodes : 0;
				sum = root? root->sum : 0;
				return 1;
			} else return 0;
		}/*}}}*/

		element_type kth(int k) {/*{{{*/
			// the kth smallest element
			assert(1 <= k && k <= num_nodes);
			return __kth_node(root, k)->key;
		}/*}}}*/

		element_type kth_biggest(int k) {/*{{{*/
			return kth(num_nodes + 1 - k);
		}/*}}}*/

		element_type kth_smallest_sum(int k) {/*{{{*/
			// the sum of 1st to kth smallest elements
			assert(1 <= k && k <= num_nodes);
			return __kth_smallest_sum(root, k);
		}/*}}}*/

		element_type kth_biggest_sum(int k) {/*{{{*/
			// the sum of 1st to kth biggest elements;
			assert(1 <= k && k <= num_nodes);
			return sum - __kth_smallest_sum(root, num_nodes-k);
		}/*}}}*/

		int count(element_type key) {/*{{{*/
			// count the duplicates of the key
			node *f = __find_node(root, key);
			return f? f->count : 0;
		}/*}}}*/

		int count_greater(element_type key) {/*{{{*/
			// count the number of elements whose key is greater than key
			return __count_greater(root, key);
		}/*}}}*/

		int count_less(element_type key) {/*{{{*/
			// count the number of elements whose key is less than key
			return __count_less(root, key);
		}/*}}}*/

		element_type sum_same(element_type key) {/*{{{*/
			// sum of the elements whose keys are same to the given key
			node *f = __find_node(root, key);
			return f? (f->count * f->key) : 0;
		}/*}}}*/

		element_type sum_greater(element_type key) {/*{{{*/
			// sum of the elements whose keys are greater than the given key
			return __sum_greater(root, key);
		}/*}}}*/

		element_type sum_less(element_type key) {/*{{{*/
			// sum of the elements whose keys are less than the given key
			return __sum_less(root, key);
		}/*}}}*/

		element_type biggest() {/*{{{*/
			// find the biggest key
			assert(root);
			return __biggest(root)->key;
		}/*}}}*/

		element_type smallest() {/*{{{*/
			// find the smallest key
			assert(root);
			return __smallest(root)->key;
		}/*}}}*/

		element_type supremum(element_type key) {/*{{{*/
			// find the min_{k>=key} k
			return __supremum(root, key);
		}/*}}}*/

		element_type infimum(element_type key) {/*{{{*/
			// find the max_{k<=key} k
			return __infimum(root, key);
		}/*}}}*/

		void set_duplicates(bool duplicates) {/*{{{*/
			// allow duplicates or not
			if (enable_duplicates == duplicates) return;
			enable_duplicates = duplicates;
			if (!duplicates) {
				if (root) __disable_duplicates(root);
				num_nodes = root? root->num_nodes : 0;
				sum = root? root->sum : 0;
			}
		}/*}}}*/

	vector<element_type>& vectorize() {/*{{{*/
		vector<element_type> *pv = new vector<element_type>(), &v = *pv;
		if (root) __vectorize(root, v);
		return v;
	}/*}}}*/
};
#endif

#define TREAP_SUM_DEFINED

//////// ---- Woosung Song's Source Code ---- ////////




int main() {
	treap_sum<int, long long> t;

	cout << "Do you want to add preloaded data? (0/1)" << endl;

	int preload; cin >> preload;

	if (preload) {
		int aa[] = {0, 1, 1, 2, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 11, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 21, 21, 21, 21, 21 };
		for (int i=0; i<31; i++) {
			t.insert(aa[i]);
		}
	}

	while(1) {
		t.print();
		cout << "-1 to exit" << endl
			 << "0 to print" << endl
			 << "1 to insert" << endl
			 << "2 to erase" << endl
			 << "3 to erase key" << endl
			 << "4 to kth" << endl
			 << "5 to sum of kth smallest" << endl
			 << "6 to sum of kth biggest" << endl
			 << "7 to count" << endl
			 << "8 to count greater" << endl
			 << "9 to count less" << endl
			 << "10 to sum same" << endl
			 << "11 to sum greater" << endl
			 << "12 to sum less" << endl
			 << "13 to biggest" << endl
			 << "14 to smallest" << endl
			 << "15 to supremum" << endl
			 << "16 to infimum" << endl
			 << "17 to toggle duplicates" << endl;
		int o, a, b; cin >> o;
		if (o==-1) break;
		if (o==0) {
		} else if (o==1) {
			cin >> a;
			t.insert(a);
		} else if (o==2) {
			cin >> a >> b;
			cout << t.erase(a, b) << endl;
		} else if (o==3) {
			cin >> a;
			cout << t.erase_key(a) << endl;
		} else if (o==4) {
			cin >> a;
			cout << t.kth(a) << endl;
		} else if (o==5) {
			cin >> a;
			cout << t.kth_smallest_sum(a) << endl;
		} else if (o==6) {
			cin >> a;
			cout << t.kth_biggest_sum(a) << endl;
		} else if (o==7) {
			cin >> a;
			cout << t.count(a) << endl;
		} else if (o==8) {
			cin >> a;
			cout << t.count_greater(a) << endl;
		} else if (o==9) {
			cin >> a;
			cout << t.count_less(a) << endl;
		} else if (o==10) {
			cin >> a;
			cout << t.sum_same(a) << endl;
		} else if (o==11) {
			cin >> a;
			cout << t.sum_greater(a) << endl;
		} else if (o==12) {
			cin >> a;
			cout << t.sum_less(a) << endl;
		} else if (o==13) {
			cout << t.biggest() << endl;
		} else if (o==14) {
			cout << t.smallest() << endl;
		} else if (o==15) {
			cin >> a;
			cout << t.supremum(a) << endl;
		} else if (o==16) {
			cin >> a;
			cout << t.infimum(a) << endl;
		} else if (o==17) {
			t.set_duplicates(!t.enable_duplicates);
		}
	}
	return 0;
}
