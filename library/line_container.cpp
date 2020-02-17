#include <bits/stdc++.h>

using namespace std;

//////// ---- Woosung Song's Source Code ---- ////////
#ifndef LINE_CONTAINER_DEFINED
typedef long long ll;

int sgn(ll x) { return (x>0) - (x<0); }

ll gcd(ll x, ll y) { return x<y? gcd(y, x) : y? gcd(y, x%y) : x; }

struct frac_ll {
	ll p, q;
	frac_ll(ll p_=0, ll q_=1) {
		p = p_, q = q_;
		normalize();
	}
	void normalize() {
		if (q < 0) p = -p, q = -q;
		ll g = gcd(abs(p), q);
		p /= g, q /= g;
	}
	double eval() const { return (double) p / q; }
	/* numerically unsafe (but faster) mode */
	/*
	bool operator==(const frac_ll &o) const { return p*o.q == q*o.p; }
	bool operator<(const frac_ll &o) const { return p*o.q < q*o.p; }
	bool operator<=(const frac_ll &o) const { return *this < o || *this == o; }
	bool operator>=(const frac_ll &o) const { return o <= *this; }
	*/
	/* numerically safe (but slower) mode */
	bool operator==(const frac_ll &o) const { return eval() == o.eval(); }
	bool operator<(const frac_ll &o) const { return eval() < o.eval(); }
	bool operator<=(const frac_ll &o) const { return eval() <= o.eval(); }
	bool operator>=(const frac_ll &o) const { return o <= *this; }

	double operator()() { return (double) p / q; }
};

struct line_ll {
	ll a, b;
	line_ll(ll a_=0, ll b_=0) : a(a_), b(b_) {}
	inline ll operator()(ll x) { return a*x + b; }
	bool operator<(const line_ll &o) const { return a < o.a; } // min 이면 부호 반대
	bool operator<=(const line_ll &o) const { return a <= o.a; } // min 이면 부호 반대
	frac_ll intersection(const line_ll &o) const {
		return frac_ll(o.b-b, a-o.a);
	}
};

class line_container {
	private:
		vector<frac_ll> intersections;

		int line_index(ll x) {
			if (!intersections.size()) return 0;
			frac_ll f(x, 1);
			if (f <= intersections[0]) return 0;
			if (f >= intersections.back()) return (int) intersections.size();
			int l = 1, r = (int) intersections.size() - 1, m;
			while (l < r) {
				m = (l + r) >> 1;
				if (f <= intersections[m]) r = m;
				else l = m+1;
			}
			return l;
		}

	public:
		vector<line_ll> container;

		line_container() {}

		void insert(ll a, ll b) {
			line_ll l(a, b); // a: slope, b: y-intersect
			if ((int) container.size() == 0) { container.push_back(l); return; }
			assert(container.back() <= l);

			if (a == container.back().a) {
				if (b <= container.back().b) // min 이면 부호 반대
					return;
				else {
					container.pop_back();
					if (intersections.size()) intersections.pop_back();
				}
			}

			int p = container.size() - 1;
			while (p > 0) {
				frac_ll f = l.intersection(container[p-1]);
				if (f <= intersections.back()) {
					intersections.pop_back();
					container.pop_back();
					p--;
				} else break;
			}

			if (container.size()) intersections.push_back(l.intersection(container.back()));
			container.push_back(l);

			/*
			for (int i=0; i<(int) container.size(); i++) {
				cout << container[i].a << ' ' << container[i].b << ' ';
				if (i != (int) container.size() - 1)
					cout << '(' << intersections[i]() << ')';
				cout << endl;
			}
			*/
		}

		ll max_value(ll x) {
			assert(container.size());
			int idx = line_index(x);
			return container[idx](x);
		}
};
#endif

#define LINE_CONTAINER_DEFINED
//////// ---- Woosung Song's Source Code ---- ////////


int main() {
	int m; cin >> m;
	line_container lc;
	while (m--) {
		ll t, a, b;
		cin >> t;
		if (t == 1) {
			cin >> a >> b;
			lc.insert(a, b);
		} else {
			cin >> a;
			cout << lc.max_value(a) << endl;
		}
	}
}
