#include"global.hpp"

int start_vld_id = 20, end_vld_id = 97;
int first_contact_ut = inf, last_contact_ut = 0;

bool isVldId(int node) {
  if (node < start_vld_id || node > end_vld_id) return 0;
  return 1;
}

// get n_machine, T
void getRealTrace(vector<vector<pii> >& contact_nodes) {
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
    int T = last_contact_ut - first_contact_ut + 2;

    contact_nodes.resize(T);
    rep(i, sz(v)) {
        int node1 = v[i][0], node2 = v[i][1], contact_ut = v[i][2];
        contact_ut -= first_contact_ut;
        contact_ut += 2; // 時刻0にコンタクトが起きてほしくない グラフ的に
        contact_nodes[contact_ut].eb(node1, node2);
    }
    cerr << "n_machine T " << n_machine << " " << T << endl;
}
