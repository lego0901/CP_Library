#include <bits/stdc++.h>

using namespace std;

//////// ---- Woosung Song's Source Code ---- ////////
#ifndef HASH_SUBSTRING_SHUFFLE_DEFINED
// All strings have to be 1-based.
class hash_substring_shuffle {
	typedef long long ll;

	const static int HASH_MAX = 10; // to defense from hack
	const ll HASH_MODS[HASH_MAX] = {1000180033, 1031825407, 1037281769, 1043566169, 1070996533, 1101601643, 1106614007, 1177477843, 1190965717, 1196697457};
	const ll HASH_PRIMES[HASH_MAX] = {100403, 142433, 143779, 183203, 185099, 189583, 220537, 289039, 291869, 295033};

		ll hash_mod_pow(ll a, ll p) {
			ll x = a;
			while (p) {
				if (p&1) x = (a * x) % hash_mod;
				a = (a * a) % hash_mod;
				p >>= 1;
			}
			return x;
		}

		void make_hash_char(int n='z') {
			hash_char.assign(n+1, 1);
			for (int i=1; i<=n; i++) {
				hash_char[i] = (hash_char[i-1] * hash_prime) % hash_mod;
			}
		}

		inline ll hash_value(int c) {
			if (c < (int) hash_char.size()) return hash_char[c];
			else return hash_mod_pow(hash_prime, c);
		}

	public:
		ll hash_mod, hash_prime;
		int size = 0;
		string str;
		vector<ll> arr;
		vector<ll> sum;
		vector<ll> hash_char;

		hash_substring_shuffle() {
			srand(time(0));
			hash_mod = HASH_MODS[rand() % HASH_MAX];
			hash_prime = HASH_PRIMES[rand() % HASH_MAX];
			make_hash_char('z');
		}

		hash_substring_shuffle(hash_substring_shuffle &hs) {
			hash_mod = hs.hash_mod;
			hash_prime = hs.hash_prime;
			make_hash_char('z');
		}

		void set_string(string &s) {
			size = s.size()-1;
			sum.assign(size+1, 0);
			for (int i=1; i<=size; i++) {
				sum[i] = (sum[i-1] + hash_value(s[i])) % hash_mod;
			}
		}

		void set_array(vector<ll> &v) {
			size = v.size() - 1;
			sum.assign(size+1, 0);
			for (int i=1; i<=size; i++) {
				ll elt = (v[i] % hash_mod + hash_mod) % hash_mod;
				sum[i] = (sum[i-1] + hash_value(elt)) % hash_mod;
			}
		}

		ll hash_value(int l, int r) {
			// from s[l:r] with 1-base
			return (sum[r] - sum[l-1] + hash_mod) % hash_mod;
		}
};
#endif

#define HASH_SUBSTRING_SHUFFLE_DEFINED
//////// ---- Woosung Song's Source Code ---- ////////

int main() {

}
