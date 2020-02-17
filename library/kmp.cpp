#include <bits/stdc++.h>

using namespace std;


//////// ---- Woosung Song's Source Code ---- ////////

// a data structure that finds substring faster
// supports find, exists, find_all

#ifndef KMP_DEFINED
class kmp {
	public:
		string str;
		vector<int> pi;
	private:
		bool __make_done;

	public:
		kmp() {}
		kmp(string _str) : str(_str), __make_done(0) {}

		int size() {/*{{{*/
			return (int) str.size();
		}/*}}}*/

		void set_str(string _str) {/*{{{*/
			// when `str' is changed
			str = _str;
			__make_done = 0 ;
		}/*}}}*/
	
	public:
		void make() {
			// preprocessing
			int n= size();
			pi.assign(n+1, -1);
			for (int j=0, k=-1; j<n; ) {
				if (k == -1 || str[j] == str[k])
					pi[++j] = ++k;
				else
					k = pi[k];
			}
			__make_done = 1;
		}

		vector<int> search_from(string &a) {
			// find all occurances of `a' from `str'
			if (!__make_done) make();
			int n = a.size(), m = size();
			vector<int> o;
			for (int i=0, j=0; i<n; ) {
				if (j == -1 || a[i] == str[j])
					i++, j++;
				else
					j = pi[j];
				if (j == m) o.push_back(i - m);
			}
			return o;
		}

		int search_from_first(string &a) {/*{{{*/
			// find `a' from `str' and returns the first position
			if (!__make_done) make();
			int m = size(), n = a.size();
			for (int i=0, j=0; i<n; ) {
				if (j == -1 || a[i] == str[j])
					i++, j++;
				else
					j = pi[j];
				if (j == m) return i - m;
			}
			return -1;
		}/*}}}*/

		bool exists_from(string &a) {/*{{{*/
			// find `a' from `str'
			return search_from_first(a) != -1;
		}/*}}}*/

};
#endif

#define KMP_DEFINED

//////// ---- Woosung Song's Source Code ---- ////////



int main() {
	string s, t;
	cout << "search keyword: ";
	cin >> t;
	cout << "target keyword(will be scanned by prev): ";
	cin >> s;

	kmp tt(t);
	tt.make();
	cout << "kmp pi function for target keyword: " << endl;
	for (int i=0; i<=(int)t.size(); i++) {
		cout << i << ' ' << tt.pi[i] << endl;
	}


	cout << endl << endl;
	vector<int> v = tt.search_from(s);
	for (auto &i : v) {
		cout << i << ' ';
	}
	cout << endl << endl;
	
	cout << s << endl;
	for (auto &i : v) {
		for (int j=0; j<i; j++) cout << ' ';
		cout << t << endl;
	}

	return 0;
}
