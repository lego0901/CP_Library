#include <bits/stdc++.h>

using namespace std;

//////// ---- Woosung Song's Source Code ---- ////////

#ifndef GEOMETRY_DEFINED
namespace geometry {
	typedef long long ll;

	const double GEOMETRY_PI = 2.0 * acos(0.0);

	int sign(ll x) { return (x>0) - (x<0); }

	class point {
		public:
			ll x, y;
			point(ll x_=0, ll y_=0) : x(x_), y(y_) {}
			bool operator==(const point &o) const { return x==o.x && y==o.y; }
			bool operator<(const point &o) const { return x==o.x? y<o.y : x<o.x; }
			bool operator<=(const point &o) const { return *this<o || *this==o; }
			bool operator>(const point &o) const { return o<*this; }
			bool operator>=(const point &o) const { return o<=*this; }
			point operator+(const point &o) const { return point(x+o.x, y+o.y); }
			point operator-(const point &o) const { return point(x-o.x, y-o.y); }
			point operator*(const int k) const { return point(x*k, y*k); }
			point operator/(const int k) const { return point(x/k, y/k); }
			ll dist2() const { return 1ll*x*x + 1ll*y*y; }
			ll dist2(const point &o) const { return (*this-o).dist2(); }
			double dist() const { return sqrt(dist2()); }
			double dist(const point &o) const { return (*this-o).dist(); }
			ll dot(const point &o) const { return 1ll*x*o.x + 1ll*y*o.y; }
			ll dot(const point &a, const point &b) const { return (a-*this).dot(b-*this); }
			ll cross(const point &o) const { return 1ll*x*o.y - 1ll*y*o.x; }
			ll cross(const point &a, const point &b) const { return (a-*this).cross(b-*this); }
			int ccw(const point &o) const { return sign(cross(o)); }
			int ccw(const point &a, const point &b) const { return sign(cross(a, b)); }
			bool in_between(const point &a, const point &b) const {
				if (ccw(a, b)) return 0;
				return min(a,b) <= *this && *this <= max(a,b);
			}
	};

	class line {
		public:
			point a, b;
			line() {}
			line(point a_, point b_) : a(a_), b(b_) {}
			ll length2() { return a.dist2(b); }
			double length() { return a.dist(b); }
			bool contains(const point &o) const { return o.ccw(a, b)? (min(a, b) <= o && o <= max(a, b)) : 0; }
			ll det(const line &o) const { return (b-a).cross(o.b-o.a); }
			bool parallel(const line &o) const { return det(o) == 0; }
			bool intersects(const line &o) const {
				int ab = a.ccw(b, o.a) * a.ccw(b, o.b);
				int oab = o.a.ccw(o.b, a) * o.a.ccw(o.b, b);
				if (ab == 0 && oab == 0) return !(max(a,b)<min(o.a,o.b) || max(o.a,o.b)<min(a,b));
				return ab <= 0 && oab <= 0;
			}
			int num_intersection(const line &o) const {
				if (parallel(o)) {
					if (a.ccw(b, o.a)!=0 || max(a,b)<min(o.a,o.b) || max(o.a,o.b)<min(a,b)) return 0;
					else if (max(a,b) == min(o.a,o.b) || max(o.a,o.b) == min(a,b)) return 1;
					else return 2;
				} else return intersects(o);
			}
	};


	vector<point> convexhull(vector<point> &points) {
		int n = (int) points.size();
		vector<point> *pcvh = new vector<point>(), &cvh = *pcvh;
		sort(points.begin(), points.end());
		cvh.push_back(points[0]);

		if (n == 1) return cvh;

		point cur, prv1, prv2;
		int sz = 1, iinc = 1;
		for (int i=1; i>=0; i+=iinc) {
			cur = points[i];
			while (sz >= 2) {
				prv1 = cvh[sz-1], prv2 = cvh[sz-2];
				if (prv2.ccw(prv1, cur) >= 0 && !cur.in_between(prv1, prv2))
					cvh.pop_back(), sz--;
				else break;
			}
			cvh.push_back(cur), sz++;
			if (i == n-1) iinc = -1;
		}
		cvh.pop_back(), sz--;
		if (sz == 1) cvh.push_back(points[n-1]);
		return cvh;
	}

	pair<point,point> __brute_force_farthest_pair(vector<point> &cvh) {
		int c = (int) cvh.size();
		ll record_dist2 = 0;
		int record_i = 0, record_j = 0;
		for (int i=0; i<c; i++)
			for (int j=i+1; j<c; j++) {
				ll d2 = cvh[i].dist2(cvh[j]);
				if (d2 > record_dist2)
					record_dist2 = d2, record_i = i, record_j = j;
			}
		return make_pair(cvh[record_i], cvh[record_j]);
	}

	pair<point,point> farthest_pair(vector<point> &points, bool brute_force=0) {
		vector<point> cvh = convexhull(points);
		int n = (int) cvh.size();
		if (brute_force || n <= 100) return __brute_force_farthest_pair(cvh);
		
		ll record_dist2 = 0;
		int record_i = 0, record_j = 0;
		for (int i=0, j=1; i<n; i++) {
			int ni = (i+1) % n, nj = (j+1) % n;
			while (1) {
				point a(0, 0), b = cvh[ni] - cvh[i], c = cvh[nj] - cvh[j];
				ll d2 = cvh[i].dist2(cvh[j]);
				if (d2 > record_dist2)
					record_dist2 = d2, record_i = i, record_j = j;
				if (a.ccw(b, c) >= 0) break;
				j = nj, nj = (nj + 1) % n;
			}
		}
		return make_pair(cvh[record_i], cvh[record_j]);
	}

	pair<point,point> __brute_force_closest_pair(vector<point> &points) {
		int n = (int) points.size();
		ll record_dist2 = numeric_limits<ll>::max();
		int record_i = 0, record_j = 0;
		for (int i=0; i<n; i++)
			for (int j=i+1; j<n; j++) {
				ll d2 = points[i].dist2(points[j]);
				if (d2 < record_dist2)
					record_dist2 = d2, record_i = i, record_j = j;
			}
		return make_pair(points[record_i], points[record_j]);
	}

	pair<point,point> closest_pair(vector<point> &points, bool brute_force=0) {
		int n = (int) points.size();
		if (brute_force || n <= 100) return __brute_force_closest_pair(points);
		sort(points.begin(), points.end());
		vector<point> left, right;
		for (int i=0; i<n; i++) (i+i<n? left : right).push_back(points[i]);

		point l1, l2, r1, r2, p1, p2; ll record_dist2 = 0;
		tie(l1, l2) = closest_pair(left); ll ld2 = l1.dist2(l2);
		tie(r1, r2) = closest_pair(right); ll rd2 = r1.dist2(r2);
		if (ld2 < rd2) record_dist2 = ld2, p1 = l1, p2 = l2;
		else record_dist2 = rd2, p1 = r1, p2 = r2;

		int x_mid = (left[(int) left.size()-1].x + right[0].x) >> 1;
		int s2 = sqrt(record_dist2);
		vector<point> mid;
		for (int i=(int) left.size()-1; i>=0; i--) {
			if (x_mid - left[i].x > s2) break;
			mid.push_back(left[i]);
		}
		for (int i=0; i<(int) right.size(); i++) {
			if (right[i].x - x_mid > s2) break;
			mid.push_back(right[i]);
		}
		sort(mid.begin(), mid.end(), [](const point &a, const point &b) {
				return a.y == b.y? a.x < b.x : a.y < b.y; });
		for (int i=0; i<(int) mid.size(); i++) {
			for (int j=i+1; j<min((int) mid.size(), i+8); j++) {
				ll d2 = mid[i].dist2(mid[j]);
				if (d2 < record_dist2)
					record_dist2 = d2, p1 = mid[i], p2 = mid[j];
			}
		}
		return make_pair(p1, p2);
	}
};
#endif

#define GEOMETRY_DEFINED

//////// ---- Woosung Song's Source Code ---- ////////


int main() {
	using namespace geometry;
	int n;
	cin >> n;
	vector<point> points;
	for (int i=0; i<n; i++) {
		int a, b;
		cin >> a >> b;
		points.emplace_back(a, b);
	}
	point a, b; tie(a, b) = closest_pair(points);
	cout << a.dist2(b) << endl;
}
