#include"global.hpp"
#include"mab.hpp"
#include"egreedy.hpp"
#include"ucb.hpp"
#include"gossip.hpp"
#include"getRealTrace.hpp"

int n_arm = 10, n_machine = 78, MAX_STEPS = 20000, N_ROUND = 10, round_id;
double init_val = 0; // 78, 1000

void round(vector<Agent> &agt, vector<vector<pii> > &contact_nodes) {
    vector<Lever> lv(n_arm);
    {
        vector<double> v;
        rep(k, n_arm) v.eb(lv[k].getMu());
        sort(all(v));
        // cerr << "lv    " << v << endl;
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

    // バイアスの影響を調べる
    double sum = 0;
    rep(i, n_machine) {
        vector<pdi> v, vb; // バイアスなし，あり
        rep(k, n_arm) {
            double mu_d = env[i].getMu(k);
            v.eb(mu_d, k);
            mu_d += agt[i].bias[k];
            vb.eb(mu_d, k);
        }
        sort(all(v)); sort(all(vb));
        sum += vb[sz(vb) - 1].S - v[sz(v) - 1].S;
    }

    cerr << "ucb" << endl; ucb(agt, env);
    // cerr << "ucb2_0" << endl; ucb2(agt, contact_nodes, env, 0, 0);
    // cerr << "ucb2_1" << endl; ucb2(agt, contact_nodes, env, 1, 0);
    // cerr << "ucb3" << endl; ucb2(agt, contact_nodes, env, 1, 1);
    cerr << "gossip" << endl; gossip(agt, contact_nodes, env);
}

int main() {
    // 各手法で共通のエージェント
    vector<Agent> agt(n_machine); // global.hpp, bias
    rep(i, n_machine) agt[i].initFixed(i); // id, d[id], hop[id]

    vector<vector<pii> > contact_nodes;
    getRealTrace(contact_nodes, agt); // agt.e, lambda. d, hop, parent

    for (round_id = 0; round_id < N_ROUND; ++round_id) {
        cerr << "round_id " << round_id << endl;
        rep(i, n_machine) {
            agt[i].setBias();
            agt[i].setPullableLever();
        }
        round(agt, contact_nodes);
    }
}
