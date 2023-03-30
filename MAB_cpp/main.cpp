#include"global.hpp"
#include"mab.hpp"
#include"egreedy.hpp"
#include"ucb.hpp"
#include"gossip.hpp"
#include"getRealTrace.hpp"
#include"iucb.hpp"

int n_arm = 10, n_machine = 78, MAX_STEPS = 2000, N_ROUND = 100, round_id = 0, opt_lv_id = -1;
double init_val = 0, opt_val = 0;
set<string> st;

void method(vector<Agent> &agt, vector<vector<pii> > &contact_nodes, vector<MAB> &env, vector<Lever> &lv, int key) {
    string filename = "data/"; // error "/data/"

    if (key == 0) {
        ucb(agt, env, contact_nodes);
        filename += "gucb";
    } else if (key == 1) {
        ucb2(agt, contact_nodes, env, 0, 0, 0); // exc_info, exc_Q, weighted
        filename += "ucb2_0";
    } else if (key == 2) {
        ucb2(agt, contact_nodes, env, 1, 0, 0);
        filename += "ucb";
    } else if (key == 3) {
        ucb2(agt, contact_nodes, env, 1, 1, 1);
        filename += "proposed";
    } else if (key == 4) {
        gossip(agt, contact_nodes, env);
        filename += "gossip";
    } else if (key == 5) {
        iucb(agt, contact_nodes, env, 0);
        filename += "iucb";
    } else if (key == 6) {
        iucb(agt, contact_nodes, env, 1); // gIUCB
        filename += "giucb";
    } else exit(1);

    string str = outputRegret(filename, agt, lv);
    if (round_id == 0) st.insert(str);
    str = outputReward(filename, agt);
    if (round_id == 0) st.insert(str);
}

void round(vector<Agent> &agt, vector<vector<pii> > &contact_nodes) {
    vector<Lever> lv(n_arm);
    { // レバーの中身表示
        vector<pdi> v;
        rep(k, n_arm) v.eb(lv[k].getMu(), k);
        sort(all(v), greater<>());
        // cerr << "lv    ";
        // rep(k, n_arm) cerr << v[k].F << " " << v[k].S << "    "; cerr << endl;

        // v.clear();
        // // バイアス込みで
        // rep(k, n_arm) v.eb(lv[k].getMu() + agt[0].bias[k], k);
        // sort(all(v), greater<>());
        // cerr << "lv+bias    ";
        // rep(k, n_arm) cerr << v[k].F << " " << v[k].S << "    "; cerr << endl;

        // 最適なレバーを求める
        double ma = 0;
        // これはchmax使っていい 同じ値は出ない
        rep(k, n_arm) {
            if (chmax(ma, lv[k].getMu())) {
                opt_lv_id = k; // コメントアウトしてはいけない 使う
            }
        }
        opt_val += lv[opt_lv_id].getMu() / N_ROUND;
        // cerr << "opt_lv_id " << opt_lv_id << " " << lv[opt_lv_id].getMu() << endl;
    }

    vector<MAB> env(n_machine); // mab.hpp
    // envの初期化
    rep(machine_id, n_machine) {
        vector<double> _mu, _sig;
        rep(k, n_arm) {
            _mu.eb(lv[k].getMu()); // 正規分布を持つレバーから出た値を期待値とする正規分布を持つレバーを
            // 各エージェントがもつ
            _sig.eb(1);
        }
        env[machine_id].init(_mu, _sig);
    }

    // // バイアスの影響を調べる
    // double sum = 0;
    // rep(i, n_machine) {
    //     vector<pdi> v, vb; // バイアスなし，あり
    //     rep(k, n_arm) {
    //         double mu_d = env[i].getMu(k);
    //         v.eb(mu_d, k);
    //         mu_d += agt[i].bias[k];
    //         vb.eb(mu_d, k);
    //     }
    //     sort(all(v)); sort(all(vb));
    //     sum += vb[sz(vb) - 1].S - v[sz(v) - 1].S;
    // }

    // for (int key : {5}) {
    for (int key : {3, 5, 0, 4}) {
        method(agt, contact_nodes, env, lv, key);
    }
}

int main() {
    // 各手法で共通のエージェント
    vector<Agent> agt(n_machine); // global.hpp, bias
    rep(i, n_machine) agt[i].initFixed(i); // id, d[id], hop[id]

    vector<vector<pii> > contact_nodes;
    getRealTrace(contact_nodes, agt); // agt.e, lambda. d, hop, parent

    for (; round_id < N_ROUND; ++round_id) { // global変数
        cerr << "round_id " << round_id << endl;
        rep(i, n_machine) {
            agt[i].setBias();
            agt[i].setPullableLever();
        }
        round(agt, contact_nodes);
    }
    cerr << "opt_val " << opt_val << endl;

    // txtをtに関してitvおきに変える
    for (string filename : st) {
    // for (string filename : {"data/gossip_regret.txt", "data/gossip_reward.txt", "data/gucb_regret.txt", "data/gucb_reward.txt", "data/iucb_regret.txt", "data/iucb_reward.txt", "data/proposed_regret.txt", "data/proposed_reward.txt"}) {
        cerr << filename << endl;
        changeTimeInterval(filename);
    }
}
