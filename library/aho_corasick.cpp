#include <bits/stdc++.h>

using namespace std;

//////// ---- Woosung Song's Source Code ---- ////////
#ifndef AHO_CORASICK_DEFINED

#define ENC(c) ((int) ((c) - 'a'))
#define TRIE_EDGE_SIZE 26
//#define ENC(c) ((int) (('a' <= (c) && (c) <= 'z')? ((c) - 'a') : ((c) - 'A' + 26)))
//#define TRIE_EDGE_SIZE 52
//#define ENC(c) ((int) ((c) - '0'))
//#define TRIE_EDGE_SIZE 10

class aho_corasick {
	private:
		int sz = 0, n = 0;
		vector<int> terminal;
		vector<vector<int> > edge;
		vector<int> failure;
		vector<int> output;

	public:
		vector<string> added;
		bool __make_failure = 0;

		aho_corasick() {
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
			__make_failure = 0;
		}

		void make_failure() {
			failure.assign(sz, 0);
			output.assign(sz, -1);
			queue<int> q;
			q.push(0);
			while (!q.empty()) {
				int here = q.front(); q.pop();
				for (int i=0; i<TRIE_EDGE_SIZE; i++) {
					int child = edge[here][i];
					if (!child) continue;
					if (here) {
						int j = failure[here];
						while (j && !edge[j][i]) j = failure[j];
						if (edge[j][i]) j = edge[j][i];
						failure[child] = j;
					}
					int f_child = failure[child];
					output[child] = -1;
					if (f_child != -1) output[child] = terminal[f_child] != -1? f_child : output[f_child];
					q.push(child);
				}
			}
			__make_failure = 1;
		}

		vector<pair<int,int> > search_from(string s) {
			if (!__make_failure) make_failure();
			int p = 0, m = s.size();
			vector<pair<int,int> > keyword_pos;
			for (int i=0; i<m; i++) {
				int c = ENC(s[i]);
				while (p && !edge[p][c]) p = failure[p];
				p = edge[p][c];
				if (terminal[p] != -1 || output[p] != -1) {
					int q = p;
					while (q != -1 && (terminal[q] != -1 || output[q] != -1)) {
						if (terminal[q] != -1)
							keyword_pos.emplace_back(terminal[q], i - (int) added[terminal[q]].size() + 1);
						q = output[q];
					}
				}
			}
			return keyword_pos;
		}

		bool exists_from(string s) {
			if (!__make_failure) make_failure();
			int p = 0, m = s.size();
			for (int i=0; i<m; i++) {
				int c = ENC(s[i]);
				while (p && !edge[p][c]) p = failure[p];
				p = edge[p][c];
				if (terminal[p] != -1 || output[p] != -1) return 1;
			}
			return 0;
		}

		void print() {
			if (!__make_failure) make_failure();
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
				if (terminal[i] != -1) {
					cout << "(" << added[terminal[i]] << ")  ";
				}
				cout << "failure: " << failure[i] << "  ";
				cout << "output: " << output[i];
			}
		}

		bool unique_suffix() {
			for (int i=0; i<sz; i++) {
				if (terminal[i] != -1) {
					for (int j=0; j<TRIE_EDGE_SIZE; j++) 
						if (edge[i][j] != -1)
							return 0;
				}
			}
			return 1;
		}

};
#endif

#define AHO_CORASICK_DEFINED
//////// ---- Woosung Song's Source Code ---- ////////

int main() {
	int n;
	cin >> n;
	aho_corasick aho;
	while (n--) {
		string s; cin >> s;
		aho.add(s);
	}
	int m;
	//cin >> m;
	m = 1;
	while (m--) {
		string s; cin >> s;
		vector<pair<int,int> > keyword_pos = aho.search_from(s);
		for (auto kp : keyword_pos) {
			cout << aho.added[kp.first] << ' ' << kp.second << endl;
		}
		if (aho.exists_from(s)) cout << "YES" << endl;
		else cout << "NO" << endl;
	}
	//aho.print();
}
