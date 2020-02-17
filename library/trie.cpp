#include <bits/stdc++.h>

using namespace std;

//////// ---- Woosung Song's Source Code ---- ////////
#ifndef TRIE_DEFINED

#define ENC(c) ((int) ((c) - 'a'))
#define TRIE_EDGE_SIZE 26
//#define ENC(c) ((int) (('a' <= (c) && (c) <= 'z')? ((c) - 'a') : ((c) - 'A' + 26)))
//#define TRIE_EDGE_SIZE 52
//#define ENC(c) ((int) ((c) - '0'))
//#define TRIE_EDGE_SIZE 10

class trie {
	private:
		int sz = 0, n = 0;
		vector<int> terminal;
		vector<vector<int> > edge;

	public:
		vector<string> added;

		trie() {
			terminal.push_back(-1);
			edge.push_back(vector<int>(TRIE_EDGE_SIZE, 0));
			sz = 1;
		}

		void add(string s) {
			int p = 0, m = s.size();
			for (int i=0; i<m; i++) {
				int c = ENC(s[i]);
				if (!edge[p][c]) {
					terminal.push_back(-1);
					edge.push_back(vector<int>(TRIE_EDGE_SIZE, 0));
					edge[p][c] = sz++;
				}
				p = edge[p][c];
			}
			terminal[p] = n++;
			added.push_back(s);
		}

		bool search(string s) {
			int p = 0, m = s.size();
			for (int i=0; i<m; i++) {
				int c = ENC(s[i]);
				if (!edge[p][c]) return 0;
				p = edge[p][c];
			}
			if (terminal[p] != -1) return 1;
		}

		void print() {
			for (auto &s : added) cout << s << endl;
			cout << "state: " << endl;
			for (int i=0; i<sz; i++) {
				cout.width(2);
				cout << i << ":  ";
				for (int j=0; j<TRIE_EDGE_SIZE; j++) {
					cout.width(2);
					cout << edge[i][j];
					cout.width(0);
					cout << ' ';
				}
				if (terminal[i] != -1)
					cout << "(" << terminal[i] << ")";
				cout << endl;
			}
		}

		bool unique_suffix() {
			for (int i=0; i<sz; i++) {
				if (terminal[i] != -1) {
					for (int c=0; c<TRIE_EDGE_SIZE; c++) {
						if (edge[i][c]) return 0;
					}
				}
			}
			return 1;
		}

};
#endif

#define TRIE_DEFINED
//////// ---- Woosung Song's Source Code ---- ////////

int main() {
	int q;
	//cin >> q;
	q = 1;
	while (q--) {
		int n;
		cin >> n;
		trie t;
		while (n--) {
			string s; cin >> s;
			t.add(s);
		}
		t.print();
		//cout << (t.unique_suffix()? "YES" : "NO") << endl;
	}
}
