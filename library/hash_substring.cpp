#include <bits/stdc++.h>

using namespace std;

//////// ---- Woosung Song's Source Code ---- ////////
#ifndef HASH_SUBSTRING_DEFINED
// All strings have to be 1-based.
class hash_substring {
	typedef long long ll;

	const static int HASH_MAX = 10; // to defense from hack
	const ll HASH_MODS[HASH_MAX] = {1000180033, 1031825407, 1037281769, 1043566169, 1070996533, 1101601643, 1106614007, 1177477843, 1190965717, 1196697457};
	const ll HASH_PRIMES[HASH_MAX] = {100403, 142433, 143779, 183203, 185099, 189583, 220537, 289039, 291869, 295033};

		void make_hash_inv() {
			ll p = hash_mod - 2, x = hash_prime;
			hash_inv = 1;
			while (p) {
				if (p&1) hash_inv = (hash_inv * x) % hash_mod;
				x = (x * x) % hash_mod;
				p >>= 1;
			}
			assert(hash_inv * hash_prime % hash_mod == 1);
		}

		void make_hash_pows(int n) {
			hash_pows.assign(n+1, 1);
			hash_inv_pows.assign(n+1, 1);

			for (int i=1; i<=n; i++) {
				hash_pows[i] = (hash_pows[i-1] * hash_prime) % hash_mod;
				hash_inv_pows[i] = (hash_inv_pows[i-1] * hash_inv) % hash_mod;
			}
		}

	public:
		ll hash_mod, hash_prime;
		ll hash_inv;
		int size = 0;
		string str;
		vector<ll> sum;
		vector<ll> hash_pows, hash_inv_pows;

		hash_substring() {
			srand(time(0));
			hash_mod = HASH_MODS[rand() % HASH_MAX];
			hash_prime = HASH_PRIMES[rand() % HASH_MAX];
			make_hash_inv();
		}

		hash_substring(hash_substring &hs) {
			hash_mod = hs.hash_mod;
			hash_prime = hs.hash_prime;
			hash_inv = hs.hash_inv;
			make_hash_inv();
			set_string(s);
		}

		void set_string(string &s) {
			int prev_size = size;
			size = s.size()-1;
			sum.assign(size+1, 0);
			if (prev_size < size)
				make_hash_pows(size);
			for (int i=1; i<=size; i++) {
				sum[i] = (sum[i-1] + hash_pows[i] * s[i]) % hash_mod;
			}
		}

		ll hash_value(int l, int r) {
			// from s[l:r] with 1-base
			ll partial = (sum[r] - sum[l-1] + hash_mod) % hash_mod;
			return partial * hash_inv_pows[l] % hash_mod;
		}
};
#endif

#define HASH_SUBSTRING_DEFINED
//////// ---- Woosung Song's Source Code ---- ////////

int main() {

}
