#include <bits/stdc++.h>

using namespace std;

//////// ---- Woosung Song's Source Code ---- ////////

#ifndef MCMF_DEFINED
// wtype: 가중치, ctype: 간선 capacity
template <class wtype, class ctype>
class mcmf {
	private:
		typedef tuple<int,int,wtype,ctype> etype;

		vector<wtype> dist;
		vector<pair<int,int> > previdx;
		vector<bool> inQ;
		const wtype WINF = numeric_limits<wtype>::max();
		const wtype CINF = numeric_limits<ctype>::max();
	
	public:
		int max_node;
		vector<vector<etype> > edge;

		mcmf() {}
		mcmf(int max_node) {
			this->max_node = max_node;
			edge.assign(max_node+1, vector<etype>());
			dist.assign(max_node+1, WINF);
			previdx.assign(max_node+1, {-1, -1});
			inQ.assign(max_node+1, 0);
		}

		void add_edge(int a, int b, wtype w, ctype c) {
			assert(a <= max_node && b <= max_node && a >= 0 && b >= 0);
			edge[a].emplace_back(b, (int) edge[b].size(), w, c);
			edge[b].emplace_back(a, (int) edge[a].size()-1, -w, 0);
			//edge[b].emplace_back(a, (int) edge[a].size()-1, -w, c);
		}


		pair<wtype,ctype> get_mcmf(int src, int dst) {
			wtype cost = 0;
			ctype flow = 0;
			queue<int> q;
			queue<int> corr;

			while (1) {
				while (!corr.empty()) {
					int a = corr.front(); corr.pop();
					dist[a] = WINF, previdx[a] = {-1, -1}, inQ[a] = 0;
				}

				dist[src] = 0;
				q.push(src);
				inQ[src] = 1;
				corr.push(src);

				while (!q.empty()) {
					int a = q.front(); q.pop(); inQ[a] = 0;
					for (int i=0; i<(int) edge[a].size(); i++) {
						int b, r; wtype w; ctype c; tie(b, r, w, c) = edge[a][i];
						if (c > 0 && dist[b] > dist[a] + w) {
							dist[b] = dist[a] + w;
							previdx[b] = make_pair(i, r);
							if (!inQ[b]) {
								q.push(b);
								corr.push(b);
								inQ[b] = 1;
							}
						}
					}
				}
				if (previdx[dst].first == -1) break;

				ctype f = CINF;
				for (int a=dst; a!=src; ) {
					int i, r, b; tie(i, r) = previdx[a]; b = get<0>(edge[a][r]);
					f = min(f, get<3>(edge[b][i]));
					a = b;
				}

				for (int a=dst; a!=src; ) {
					int i, r, b; tie(i, r) = previdx[a]; b = get<0>(edge[a][r]);
					ctype &c = get<3>(edge[b][i]), &cr = get<3>(edge[a][r]);
					wtype w = get<2>(edge[b][i]);

					cost += f * w;
					c -= f;
					cr += f;

					a = b;
				}
				flow += f;
			}
			return make_pair(cost, flow);
		}
};
#endif

#define MCMF_DEFINED

//////// ---- Woosung Song's Source Code ---- ////////

typedef long long ll;

int main() {
	int n, m;
	//cin >> n >> m;
	scanf("%d%d", &n, &m);

	mcmf<ll,ll> gr(n+m+2);

	int src = n+m+1, dst = n+m+2;

	for (int a=1; a<=n; a++) {
		gr.add_edge(src, a, 0, 1);
		int k;
		//cin >> k;
		scanf("%d", &k);
		while (k--) {
			int x, y;
			//cin >> x >> y;
			scanf("%d%d", &x, &y);
			gr.add_edge(a, n+x, y, 1);
		}
	}

	for (int b=1; b<=m; b++) {
		gr.add_edge(n+b, dst, 0, 1);
	}

	ll cost, flow;
	tie(cost, flow) = gr.get_mcmf(src, dst);
	//cout << flow << endl << cost << endl;
	printf("%lld\n%lld\n", flow, cost);
}
