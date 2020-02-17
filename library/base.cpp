#include <bits/stdc++.h>

using namespace std;

typedef long long ll;/*{{{*/
typedef long double ld;
typedef pair<int,int> pii;
typedef tuple<int,int,int> piii;
typedef pair<int,ll> pil;
typedef pair<ll,ll> pll;
typedef tuple<ll,ll,ll> plll;
typedef vector<int> vi;
typedef vector<ll> vl;
typedef vector<pii> vii;
typedef vector<pil> vil;
typedef vector<pll> vll;
typedef vector<piii> viii;
typedef vector<vi> vvi;
typedef vector<vii> vvii;
typedef vector<vl> vvl;
typedef vector<vll> vvll;
typedef vector<char> vc;
typedef vector<bool> vb;
typedef vector<vb> vvb;
typedef int (*itoi) (int);
typedef int (*iitoi) (int, int);
typedef int (*iiitoi) (int, int, int);
typedef ll (*ltol) (ll);
typedef ll (*lltol) (ll, ll);
typedef ll (*llltol) (ll, ll, ll);

const ld PI = acos(0)*2;
const int INF = 0x3f2f1f0f;
const ll LINF = 1ll*INF*INF;
int X4[] = {-1, 0, 1, 0}, Y4[] = {0, -1, 0, 1};
int X8[] = {-1, -1, -1, 0, 1, 1, 1, 0}, Y8[] = {-1, 0, 1, 1, 1, 0, -1, -1};

#define ALL(x) (x).begin(), (x).end()
#define RALL(x) (x).rbegin(), (x).rend()
#define SZ(x) ((int) (x).size())
#define HASV(v, x) ((lower_bound(ALL(v), (x)) != (v).end()) && (*lower_bound(ALL(v), (x)) == (x)))
#define HASS(v, x) ((v).find(x) != (v).end())
#define NTH(v, x) ((int) (lower_bound(ALL(v), (x)) - (v).begin()))
#define SAME(v, x) ((int) (upper_bound(ALL(v), (x)) - lower_bound(ALL(v), (x))))
#define UNIQUE(v) v.erase(unique(ALL(v)), (v).end())
#define WER << ' ' <<

#define pb push_back
#define eb emplace_back
#define INP(t, x) t x; cin >> x

#define CPPIO ios::sync_with_stdio(0); cin.tie(0); cout << std::setprecision(10); cout << fixed

#define forn(x, n) for (int x=0; x<(n); x++)
#define for1(x, n) for (int x=1; x<=(n); x++)
#define ford(x, n) for (int x=(n)-1; x>=0; x--)
#define ford1(x, n) for (int x=(n); x>=1; x--)
#define forr(x, n, m) for (int x=(n); x<=(m); x++)
#define forrd(x, n, m) for (int x=(m); x>=(n); x--)

template<typename T> inline bool uax(T &a, T b) { return a<b? (a=b, 1) : 0; }
template<typename T> inline bool uin(T &a, T b) { return a>b? (a=b, 1) : 0; }
template<typename T1, typename T2> istream& operator>>(istream& is, pair<T1,T2>& p) {
	return is >> p.first >> p.second;
}
template<typename T1, typename T2> ostream& operator<<(ostream& os, const pair<T1,T2>& p) {
	return os << '(' << p.first << ", " << p.second << ")";
}
template<typename T> ostream& operator<<(ostream& os, const vector<T>& v) {
	for (int i=0; i<(int)v.size(); i++) { os << v[i]; if (i+1<(int)v.size()) os << ' '; } return os;
}/*}}}*/


//////// ---- Woosung Song's Source Code ---- ////////




int main() {
	CPPIO;
	
	//////// ---- Woosung Song's Source Code ---- ////////
	
	
	
	
#ifdef LOCAL_DEFINE/*{{{*/
	cerr << "Time elapsed: " << 1.0 * clock() / CLOCKS_PER_SEC << "s.\n";
#endif
	return 0;/*}}}*/
}
