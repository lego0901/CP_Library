#include <bits/stdc++.h>

using namespace std;


//////// ---- Woosung Song's Source Code ---- ////////

#ifndef NUMBER_THEORY_DEFINED
namespace number_theory {
	typedef long long ll;
	typedef vector<pair<ll,int> > factor;

	ll MOD = 1e9 + 7;

	ll __gcd_two(ll a, ll b) {/*{{{*/
		return a<0ll? __gcd_two(-a, b) :
			   b<0ll? __gcd_two(a, -b) :
			   a<b? __gcd_two(b, a) : b? __gcd_two(b, a%b) : a;
	}/*}}}*/
	
	ll gcd(ll a) { return a<0ll? -a : a; }
	template<typename ... rests> ll gcd(ll a, const rests&... rest) {/*{{{*/
		ll b = gcd(rest...);
		return __gcd_two(a, b);
	}/*}}}*/

	void extended_euclid(ll a, ll b, ll &x, ll &y) {/*{{{*/
		if (a<0) { extended_euclid(-a, b, x, y); x = -x; }
		else if (b<0) { extended_euclid(a, -b, x, y); y = -y; }
		else if (a<b) { extended_euclid(b, a, y, x); }
		else if (b>0) {
			extended_euclid(b, a%b, y, x);
			y -= a/b*x;
		} else { x = 1, y = 0; }
	}/*}}}*/

	ll mod_clip(ll a, ll m) { return (a % m + m) % m; }/*{{{*/
	ll mod_clip(ll a) { return mod_clip(a, MOD); }/*}}}*/

	ll mod_inverse(ll a, ll m) {/*{{{*/
		// returns b s.t. a * b mod m = 1
		assert(gcd(a, m) == 1);
		ll x, y;
		extended_euclid(a, m, x, y);
		return mod_clip(x, m);
	}
	ll mod_inverse(ll a) { return mod_inverse(a, MOD); }/*}}}*/

	ll mod_mult(ll a, ll b, ll m) {/*{{{*/
		if (b<0) return mod_clip(-mod_mult(a, -b, m), m);
		ll r = 0, x = a;
		while (b) {
			if (b & 1) r = (r + x) % m;
			x = (x + x) % m; b >>= 1;
		}
		return mod_clip(r, m);
	}
	ll mod_mult(ll a, ll b) { return mod_mult(a, b, MOD); }/*}}}*/

	ll mod_mults(ll a) { return mod_clip(a); }
	template<typename ... rests> ll mod_mults(ll a, const rests&... rest) {/*{{{*/
		ll b = mod_mults(rest...);
		return mod_mult(a, b);
	}/*}}}*/

	ll mod_div(ll a, ll b, ll m) {/*{{{*/
		assert(b != 0);
		return mod_mult(a, mod_inverse(b, m), m);
	}
	ll mod_div(ll a, ll b) { return mod_div(a, b, MOD); }/*}}}*/

	ll mod_pow(ll a, ll p, ll m) {/*{{{*/
		if (p<0) return mod_inverse(mod_pow(a, -p, m), m);
		ll r = 1, x = a;
		while (p) {
			if (p & 1) r = mod_clip(mod_mult(r, x, m), m);
			x = mod_mult(x, x, m); p >>= 1;
		}
		return r;
	}
	ll mod_pow(ll a, ll p) { return mod_pow(a, p, MOD); }/*}}}*/


	vector<ll> primes, phi, mob;
	vector<bool> sieve;

	void make_sieve(int n) {/*{{{*/
		sieve.assign(n+1, 1);
		sieve[0] = sieve[1] = 0;
		for (int i=2; i<=n; i++) {
			if (!sieve[i]) continue;
			for (int j=i+i; j<=n; j+=i) {
				sieve[j] = 0;
			}
		}
	}/*}}}*/

	void make_primes(int n, bool naive_way=0) {/*{{{*/
		primes.resize(0);
		if (naive_way) {
			if (n >= 2) primes.push_back(2);
			for (int i=3; i<=n; i+=2) {
				int flag = 0;
				for (auto &p : primes) {
					if (i%p == 0) { flag = 1; break; }
				}
				if (!flag) primes.push_back(i);
			}
		} else {
			if (((int) sieve.size()) <= n) make_sieve(n);
			for (int i=2; i<=n; i++)
				if (sieve[i]) primes.push_back(i);
		}
	}/*}}}*/

	void make_primes_added(int n) {/*{{{*/
		if (((int) primes.size()) > 0 && primes.back() >= n) return;
		int start = ((int) primes.size()) == 0? 2 : primes.back()+1;
		for (int i=start; i<=n; i++) {
			int flag = 0;
			for (auto &p : primes) {
				if (i%p == 0) { flag = 1; break; }
			}
			if (!flag) primes.push_back(i);
		}
	}/*}}}*/

	void make_phi(int n) {/*{{{*/
		phi.assign(n+1, 0);
		for (int i=0; i<=n; i++) phi[i] = i;
		for (int i=1; i<=n; i++) {
			for (int j=i+i; j<=n; j+=i)
				phi[j] -= phi[i];
		}
	}/*}}}*/

	void make_mob(int n) {/*{{{*/
		mob.assign(n+1, 0);
		mob[1] = 1;
		for (int i=1; i<=n; i++) {
			for (int j=i+i; j<=n; j+=i)
				mob[j] -= mob[i];
		}
	}/*}}}*/


	long long __AUTO_POLLARD_RHO_BOUND = 1e14;/*{{{*/
	long long __POLLARD_RHO_NAIVE_BOUND = 5e4;
	int __MAX_POLLARD_RHO_ITERATION = 1e5;

	int __MAX_MILLER_RABIN_TEST = 37;
	bool __MILLER_RABIN_BEFORE_POLLARD_RHO = 1;/*}}}*/

	bool miller_rabin(ll n) {/*{{{*/
		// probable primality test
		if (n <= 1) return 0;
		if (((int) primes.size()) == 0 || primes.back() < __MAX_MILLER_RABIN_TEST)
			make_primes_added(__MAX_MILLER_RABIN_TEST);
		for (int i=0; i<((int) primes.size()) && primes[i]<=__MAX_MILLER_RABIN_TEST; i++) {
			if (n == primes[i]) return 1;
			if (n % primes[i] == 0) return 0;
		}

		int s = 0;
		ll m = n-1;
		while ((m&1) == 0) m >>= 1, s++;

		for (int i=0; i<((int) primes.size()) && primes[i] <= __MAX_MILLER_RABIN_TEST; i++) {
			ll p = primes[i];
			ll q = mod_pow(p, m, n);
			if (q == 1) continue;
			bool flag = 1;
			for (int j=0; j<s; j++) {
				if (q == n-1) { flag = 0; break; }
				q = mod_mult(q, q, n);
			}
			if (flag) return 0;
		}
		return 1;
	}/*}}}*/

	ll pollard_rho(ll n) {/*{{{*/
		// stochastic divisor breaker
		if (__MILLER_RABIN_BEFORE_POLLARD_RHO)
			if (miller_rabin(n)) return n;
		for (ll p=2; p<__POLLARD_RHO_NAIVE_BOUND; p++)
			if (n % p == 0) return p;
		int it = 0;
		ll d = 1;
		while (it < __MAX_POLLARD_RHO_ITERATION) {
			srand(time(0));
			if (n == 1) return n;
			if (n%2 == 0) return 2;
			ll x = (rand() % (n-2)) + 2, y = x;
			ll c = (rand() % (n-1)) + 1;

			while (it < __MAX_POLLARD_RHO_ITERATION && d == 1) {
				++it;
				x = (mod_mult(x, x, n) + c + n) % n;
				y = (mod_mult(y, y, n) + c + n) % n;
				y = (mod_mult(y, y, n) + c + n) % n;
				d = gcd(x-y, n);
				if (d == n) { d = 1; break; }
				else if (d > 1) return d;
			}
		}
		return n;
	}/*}}}*/

	factor __merge_factors(factor a, factor b) {/*{{{*/
		map<ll,int> m;
		for (auto &p : a) m[p.first] += p.second;
		for (auto &p : b) m[p.first] += p.second;
		factor v;
		for (auto &p : m) v.push_back(p);
		return v;
	}/*}}}*/

	factor factorization(ll n, int calc_mode=0) {/*{{{*/
		// calc_mode: 0 is auto, 1 is naive (O(n^1/2)), 2 is pollard-rho
		int orig_calc_mode = calc_mode;
		if (calc_mode == 0) calc_mode = (n > __AUTO_POLLARD_RHO_BOUND)? 2 : 1;
		if (calc_mode == 1) {
			if (((int) primes.size()) > 0 && primes.back() > n / primes.back()) {
				factor v;
				for (auto &p : primes) {
					if (p * p > n) break;
					if (n % p == 0) {
						v.emplace_back(p, 0);
						while (n % p == 0) { v.back().second++; n /= p; }
					}
				}
				if (n > 1) v.emplace_back(n, 1);
				return v;
			} else {
				factor v;
				for (ll p=2; p*p<=n; p++) {
					if (n % p == 0) {
						v.emplace_back(p, 0);
						while (n % p == 0) { v.back().second++; n /= p; }
					}
				}
				if (n > 1) v.emplace_back(n, 1);
				return v;
			}
		} else {
			ll d = pollard_rho(n);
			if (d == n || d == 1) {
				factor v;
				v.emplace_back(n, 1);
				return v;
			} else {
				return __merge_factors(factorization(d, orig_calc_mode), factorization(n/d, orig_calc_mode));
			}
		}
	}/*}}}*/


	bool is_prime(ll n, int calc_mode=0) {/*{{{*/
		// calc_mode: 0 is auto, 1 is naive (O(n^1/2)), 2 is miller-rabin.
		if (n <= 1) return 0;
		if (((int) sieve.size()) > n) return sieve[n];
		if (calc_mode == 0) calc_mode = (n > __AUTO_POLLARD_RHO_BOUND)? 2 : 1;
		if (calc_mode == 1) {
			if (((int) primes.size()) > 0 && primes.back() >= n) {
				auto p = lower_bound(primes.begin(), primes.end(), n);
				return (*p == n);
			} else if (((int) primes.size()) > 0 && primes.back() * primes.back() >= n) {
				for (auto &p : primes) {
					if (n % p == 0) return 0;
					if (p * p >= n) return 1;
				}
				return 1;
			} else {
				for (ll p=2; p*p<=n; p++) {
					if (n % p == 0) return 0;
				}
				return 1;
			}
		} else {
			return miller_rabin(n);
		}
		return 0;
	}/*}}}*/

	ll euler_phi(ll n, int calc_mode=0) {/*{{{*/
		// calc_mode: 0 is auto, 1 is naive (O(n^1/2)), 2 is pollard-rho
		if (n <= 1) return max(0ll, n);
		if (((int) phi.size()) > n) return phi[n];
		if (calc_mode == 0) calc_mode = (n > __AUTO_POLLARD_RHO_BOUND)? 2 : 1;
		if (calc_mode == 1) {
			if (((int) primes.size()) > 0 && primes.back() * primes.back() >= n) {
				ll r = n;
				for (auto &p : primes) {
					if (n % p == 0) { r -= r/p; while (n % p == 0) n /= p; }
					if (p * p > n) break;
				}
				if (n > 1) r -= r/n;
				return r;
			} else {
				ll r = n;
				for (ll p=2; p*p<=n; p++) {
					if (n % p == 0) { r -= r/p; while (n % p == 0) n /= p; }
				}
				if (n > 1) r -= r/n;
				return r;
			}
		} else {
			factor f = factorization(n, 2);
			ll r = n;
			for (auto &pe : f) {
				r -= r/pe.first;
			}
			return r;
		}
	}/*}}}*/

	ll mobius(ll n, int calc_mode=0) {/*{{{*/
		// calc_mode: 0 is auto, 1 is naive (O(n^1/2)), 2 is pollard-rho
		assert(n > 0);
		if (((int) mob.size()) > n) return mob[n];
		if (calc_mode == 0) calc_mode = (n > __AUTO_POLLARD_RHO_BOUND)? 2 : 1;
		if (calc_mode == 1) {
			if (((int) primes.size()) > 0 && primes.back() * primes.back() >= n) {
				ll r = 1;
				for (auto &p : primes) {
					if (n % (p*p) == 0) return 0;
					if (n % p == 0) r = -r, n /= p;
					if (p * p > n) break;
				}
				if (n > 1) r = -r;
				return r;
			} else {
				ll r = 1;
				for (ll p=2; p*p<=n; p++) {
					if (n % (p*p) == 0) return 0;
					if (n % p == 0) r = -r, n /= p;
				}
				if (n > 1) r = -r;
				return r;
			}
		} else {
			factor f = factorization(n, 2);
			ll r = 1;
			for (auto &pe : f) {
				if (pe.second >= 2) return 0;
				r = -r;
			}
			return r;
		}
	}/*}}}*/


	ll to_number(factor f) {/*{{{*/
		ll r = 1;
		for (auto &pe : f)
			for (int i=0; i<pe.second; i++) r *= pe.first;
		return r;
	}/*}}}*/

	struct divisor_iterator {/*{{{*/
		ll n;
		factor f;

		void init() { f = factorization(n); }
		divisor_iterator(ll n_) : n(n_) { init(); }
		void set_number(ll n_) { n = n_; init(); }
		ll next_divisor(ll d) {
			assert(n % d == 0);
			factor iter = f;
			for (auto &pe : iter) pe.second = 0;
			for (auto &pe : iter) { while (d % pe.first == 0) pe.second++, d /= pe.first; }
			for (int i=0; i<((int) iter.size()); i++) {
				if (iter[i].second == f[i].second) iter[i].second = 0;
				else { iter[i].second++; break; }
			}
			return to_number(iter);
		}
	};/*}}}*/

	vector<ll> divisors(ll n) {/*{{{*/
		vector<ll> r;
		divisor_iterator iter(n);
		ll d = 1;
		while (d != n) {
			r.push_back(d);
			d = iter.next_divisor(d);
		}
		r.push_back(n);
		return r;
	}/*}}}*/

	vector<ll> prime_divisors(ll n) {/*{{{*/
		factor f = factorization(n);
		vector<ll> r;
		for (auto &pe : f) r.push_back(pe.first);
		return r;
	}/*}}}*/
	 

	
	vector<ll> facts;
	vector<vector<ll> > binoms;

	bool __FACTS_ENABLE_MOD = 1;/*{{{*/
	ll __FACTS_MOD = MOD;
	bool __BINOMS_ENABLE_MOD = 1;
	ll __BINOMS_MOD = MOD;/*}}}*/
	
	void make_facts(int n, bool enable_mod=1, ll mod=0) {/*{{{*/
		// O(N)
		if (enable_mod && mod == 0) mod = MOD;
		__FACTS_ENABLE_MOD = enable_mod;
		__FACTS_MOD = enable_mod? mod : 0;

		facts.assign(n+1, 1);
		for (int i=1; i<=n; i++) {
			facts[i] = enable_mod? mod_mult(facts[i-1], i, mod) : facts[i-1] * i;
		}
	}/*}}}*/

	void make_binoms(int n, int m, bool enable_mod=1, ll mod=0) {/*{{{*/
		// O(NM)
		if (enable_mod && mod == 0) mod = MOD;
		__BINOMS_ENABLE_MOD = enable_mod;
		__BINOMS_MOD = enable_mod? mod : 0;

		binoms.assign(n, vector<ll>(m, 0));
		binoms[0][0] = 1;
		for (int i=1; i<=n; i++) {
			for (int j=1; j<=i; j++) {
				binoms[i][j] = binoms[i-1][j] + (j>0? binoms[i-1][j-1] : 0);
				if (enable_mod) binoms[i][j] %= mod;
			}
		}
	}/*}}}*/

	ll factorial(ll n, bool enable_mod=1, ll mod=0) {/*{{{*/
		if (enable_mod && mod == 0) mod = MOD;
		if (((int) facts.size()) > n && enable_mod == __FACTS_ENABLE_MOD && mod == __FACTS_MOD)
			return facts[n];
		ll r = 1;
		for (int i=1; i<=n; i++) {
			r = enable_mod? mod_mult(r, i, mod) : r * i;
		}
		return r;
	}/*}}}*/

	ll binomial(ll n, ll m, bool enable_mod=1, ll mod=0) {/*{{{*/
		if (enable_mod && mod == 0) mod = MOD;
		if (((int) binoms.size()) > n && ((int) binoms[0].size()) > m &&
				enable_mod == __BINOMS_ENABLE_MOD && mod == __BINOMS_MOD)
			return binoms[n][m];
		else {
			if (enable_mod) return mod_div(mod_div(factorial(n, 1, mod), factorial(n-m, 1, mod)), factorial(m, 1, mod));
			else {
				ll a = m, b = n-m; if (a<b) swap(a, b);
				ll r = 1;
				for (ll c=a+1; c<=n; c++) r *= c;
				return r / factorial(b, 0);
			}
		}
	}/*}}}*/
	
};
#endif

#define NUMBER_THEORY_DEFINED

//////// ---- Woosung Song's Source Code ---- ////////



int main() {
	using namespace number_theory;
	vector<ll> divs = divisors(120);
	for (auto &d : divs) cout << d << endl;
	// cout << mod_mults(20, 25, 30, 35, 45, 55, 56, 765, 91) << endl;
	/*
	cout << mod_clip(mod_mult(n, mod_inverse(n))) << endl;
	cout << is_prime(n) << endl;

	factor f = factorization(n);
	for (auto &pe : f) {
		cout << pe.first << '^' << pe.second << ' ';
	}
	cout << endl;

	vector<ll> divs = divisors(n);
	for (auto l : divs) {
		cout << l << " | " << n << endl;
	}

	for (int i=1; i<=n; i++)
		cout << "phi" << i << ": " << euler_phi(i) << endl;
	*/
	return 0;
}
