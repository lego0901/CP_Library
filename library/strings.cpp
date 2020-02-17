#include <bits/stdc++.h>

using namespace std;

//////// ---- Woosung Song's Source Code ---- ////////
#ifndef STRINGS_DEFINED
// all strings should be encoded in 1-base indices.
namespace strings {
	vector<int>& suffix_array(string &s) {
		int n = s.size() - 1;
		int i, m = 26;
		vector<int> cnt(max(n,m)+1, 0), x(n+1, 0), y(n+1, 0);
		vector<int> *psa = new vector<int>(n+1, 0), &sa = *psa;
		for (i=1;i<=n;i++) cnt[x[i] = s[i]-'a'+1]++;
		for (i=1;i<=m;i++) cnt[i] += cnt[i-1];
		for (i=n;i;i--) sa[cnt[x[i]]--] = i;
		for (int len=1,p=1;p<n;len<<=1,m=p) {
			for (p=0,i=n-len;++i<=n;) y[++p] = i;
			for (i=1;i<=n;i++) if (sa[i] > len) y[++p] = sa[i]-len;
			for (i=0;i<=m;i++) cnt[i] = 0;
			for (i=1;i<=n;i++) cnt[x[y[i]]]++;
			for (i=1;i<=m;i++) cnt[i] += cnt[i-1];
			for (i=n;i;i--) sa[cnt[x[y[i]]]--] = y[i];
			swap(x,y); p = 1; x[sa[1]] = 1;
			for (i=1;i<n;i++)
				x[sa[i+1]] = sa[i]+len <= n && sa[i+1]+len <= n && y[sa[i]] == y[sa[i+1]] && y[sa[i]+len] == y[sa[i+1]+len] ? p : ++p;
		}
		return sa;
	}

	vector<int>& longest_common_prefix(string &s, vector<int> &sa) {
		int n = s.size() - 1;
		int i,j,k=0;
		vector<int> *plcp = new vector<int>(n+1, 0), &lcp = *plcp;
		vector<int> rank(n+1,0);
		for (i=1;i<=n;i++) rank[sa[i]] = i;
		for (i=1;i<=n;lcp[rank[i++]]=k)
			for (k?k--:0,j=sa[rank[i]-1];s[i+k]==s[j+k];k++);
		return lcp;
	}

	vector<int>& longest_common_prefix(string &s) {
		vector<int> &sa = suffix_array(s);
		return longest_common_prefix(s, sa);
	}
};
#endif

#define STRINGS_DEFINED
//////// ---- Woosung Song's Source Code ---- ////////


int main() {
	string s;
	cin >> s; // assume s = bananaban
	int n = s.size();
	s = "_" + s; // 1-base index
	vector<int> &sa = strings::suffix_array(s);
	vector<int> &lcp = strings::longest_common_prefix(s, sa);
	for (int i=1; i<=n; i++) {
		cout.width(2);
		cout << i << ' ';
		cout.width(2);
		cout << sa[i] << ' ';
		cout.width(2);
		cout << lcp[i] << ' '
			 << s.substr(sa[i]) << endl;
		cout.width(0);
	}
	/* example for s = bananaban
 1  6  0 aban
 2  8  1 an
 3  4  2 anaban
 4  2  3 ananaban
 5  7  0 ban
 6  1  3 bananaban
 7  9  0 n
 8  5  1 naban
 9  3  2 nanaban
 	*/
}
