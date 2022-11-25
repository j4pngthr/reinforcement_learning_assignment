#include"global.hpp"

int start_vld_id = 20, end_vld_id = 97;
int first_contact_ut = inf, pre_end_ut = inf, last_contact_ut = 0;

bool isVldId(int node) {
    if (node < start_vld_id || node > end_vld_id) return 0;
    return 1;
}

void dijkstra(vector<Agent> &agt, const int s) {
    priority_queue<pdi, vector<pdi>, greater<pdi> > q; // 距離, 頂点

    for (int nxt : agt[s].e) { // 下でelse if通る
        q.emplace(agt[s].d[nxt], nxt);
        agt[s].hop[nxt] = 1;
        agt[s].parent[nxt] = s;
    }

    while (!q.empty()) {
        pdi p = q.top();
        int now = p.S;
        q.pop();
        for (int nxt : agt[now].e) {
            // if (s == 0) cerr << now << " " << agt[s].d[now] << " " << agt[now].d[nxt] << endl;
            if (chmin(agt[s].d[nxt], agt[s].d[now] + agt[now].d[nxt])) {
                q.emplace(agt[s].d[nxt], nxt);
                agt[s].hop[nxt] = agt[s].hop[now] + 1;
                agt[s].parent[nxt] = now;
            }
        }
    }

    // if (s != 0) return;
    // rep(i, n_machine) {
    //     cerr << "i " << i << endl;
    //     int cnt = 0;
    //     for (int cur = i; cur != -1; cur = agt[s].parent[cur]) {
    //         ++cnt;
    //         if (cnt == 10) break;
    //         cerr << cur << " ";
    //     }
    //     cerr << endl;
    // }
}

// get n_machine, T
void getRealTrace(vector<vector<pii> >& contact_nodes, vector<Agent> &agt) {
    // n_machine = end_vld_id - start_vld_id + 1;

    // 入力ファイル
    string str = "contacts.Exp";
    str += "6";
    str += ".txt";
    ifstream ifs(str);

    vector<vector<int> > v; // node i, j, contact unit time
    string line;

    // コンタクトを2周する
    // まずは最初と最後のコンタクトの時間を得る
    while (getline(ifs, line)) {
        istringstream stream(line);
        vector<int> v2;
        while (getline(stream, str, ' ')) {
            if (str[0] >= '0' && str[0] <= '9') {
                v2.eb(stoi(str));
            }
        }

        --v2[0]; --v2[1];
        int node1 = v2[0], node2 = v2[1], contact_ut = v2[2];
        if (node1 == node2 || !isVldId(node1) || !isVldId(node2)) continue;

        chmax(last_contact_ut, contact_ut);
        chmin(first_contact_ut, contact_ut);

        v2[0] -= start_vld_id; v2[1] -= start_vld_id;
        // agt[v2[0]].e.insert(v2[1]); agt[v2[1]].e.insert(v2[0]);
        v.eb(v2);
    }
    cerr << "fi_co_ut la_co_ut " << first_contact_ut << " " << last_contact_ut << endl;
    // int T = last_contact_ut - first_contact_ut + 2;
    int pre_end_ut = first_contact_ut + (last_contact_ut - first_contact_ut) / 2;
    cerr << "pre_end_ut " << pre_end_ut << endl;
    // contact_nodes.resize(T);

    contact_nodes.resize(last_contact_ut - pre_end_ut);
    rep(k, sz(v)) {
        int i = v[k][0], j = v[k][1], contact_ut = v[k][2];
        if (contact_ut < pre_end_ut) { // 前半, グラフを作る
            rep(_, 2) {
                agt[i].e.insert(j);
                ++agt[i].lambda[j];
            }
        } else { // 後半, simulationに使う
            int ut = contact_ut - pre_end_ut;
            assert(ut >= 0);

            contact_nodes[ut].eb(i, j);
        }
    }

    // rep(i, sz(v)) {
    //     int node1 = v[i][0], node2 = v[i][1], contact_ut = v[i][2];
    //     contact_ut -= first_contact_ut;
    //     contact_ut += 2; // 時刻0にコンタクトが起きてほしくない グラフ的に
    //     contact_nodes[contact_ut].eb(node1, node2);
    // }
    // cerr << "n_machine T " << n_machine << " " << T << endl;

    rep(i, n_machine) rep(j, n_machine) {
        agt[i].lambda[j] /= pre_end_ut - first_contact_ut;
    }

    rep(i, n_machine) rep3(j, i + 1, n_machine) { // i != j
        if (agt[i].lambda[j] > eps) {
            agt[i].d[j] = 1.0 / agt[i].lambda[j];
        } else {
            agt[i].d[j] = inf;
        }
        agt[j].d[i] = agt[i].d[j];
    }

    rep(s, n_machine) {
        dijkstra(agt, s);
    }
}
