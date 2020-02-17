#include <bits/stdc++.h>

using namespace std;

//////// ---- Woosung Song's Source Code ---- ////////
#ifndef LINKED_NODE
template <class dtype>
struct linked_node {
	dtype val;
	linked_node *l, *r;
	linked_node() : l(0), r(0) {}
	linked_node(dtype v) : l(0), r(0) {
		val = v;
	}

	void erase() {
		if (l) l->r = r;
		if (r) r->l = l;
	}

	linked_node* insert(dtype v) {
		linked_node *x = new linked_node(v);
		if (r) r->l = x;
		x->r = r;
		x->l = this;
		r = x;
		return x;
	}

	linked_node* insert_left(dtype v) {{{{
		linked_node *x = new linked_node(v);
		if (l) l->r = x;
		x->l = l;
		x->r = this;
		l = x;
		return x;
	}/*}}}*/

	linked_node* move_right(int k) {{{{
		if (k < 0) return move_left(-k);
		linked_node* x = this;
		while (k--) x = x->r;
		return x;
	}/*}}}*/

	linked_node* move_left(int k) {{{{
		if (k < 0) return move_right(-k);
		linked_node* x = this;
		while (k--) x = x->l;
		return x;
	}/*}}}*/
};
#endif

#define LINKED_NODE
//////// ---- Woosung Song's Source Code ---- ////////

int main() {
	linked_node<int> *head, *tail;
	head = tail = 0;

	int q;
	cin >> q;
	while (q--) {
		string s; int v;
		cin >> s;
		if (s == "push_back") {
			cin >> v;
			if (tail) tail = tail->insert(v);
			else head = tail = new linked_node<int>(v);
		} else if (s == "push_front") {
			cin >> v;
			if (head) head = head->insert_left(v);
			else head = tail = new linked_node<int>(v);
		} else if (s == "pop_back") {
			if (tail) {
				if (head == tail) {
					delete tail;
					head = tail = 0;
				} else {
					linked_node<int> *x = tail->l;
					tail->erase();
					delete tail;
					tail = x;
				}
			} else cout << "error" << endl;
		} else if (s == "pop_front") {
			if (head) {
				if (head == tail) {
					delete head;
					head = tail = 0;
				} else {
					linked_node<int> *x = head->r;
					head->erase();
					delete head;
					head = x;
				}
			} else cout << "error" << endl;
		} else if (s == "back") {
			if (tail) cout << tail->val << endl;
			else cout << "error" << endl;
		} else if (s == "front") {
			if (head) cout << head->val << endl;
			else cout << "error" << endl;
		}
	}
}
