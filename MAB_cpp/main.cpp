#include"global.hpp"
#include"mab.hpp"
#include"egreedy.hpp"
#include"ucb.hpp"
#include"gossip.hpp"
#include"proposed.hpp"
#include"getRealTrace.hpp"

int n_arm = 10, n_machine = 78, MAX_STEPS = 1500;
double init_val = 0; // 78, 1000

int main() {
    vector<Lever> lv(n_arm);
    {
        vector<double> v;
        rep(k, n_arm) v.eb(lv[k].getMu());
        sort(all(v));
        cerr << v << endl;
    }

    // 各手法で共通のエージェント
    vector<Agent> agt(n_machine); // global.hpp, bias
    vector<vector<pii> > contact_nodes;
    getRealTrace(contact_nodes, agt);

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
    rep(machine_id, n_machine) {
        vector<double> v, vb;
        rep(k, n_arm) {
            double mu_d = env[machine_id].getMu(k);
            v.eb(mu_d);
            mu_d += agt[machine_id].bias[k];
            vb.eb(mu_d);
        }
        sort(all(v)); sort(all(vb));
        cerr << v[0] << " " << v[sz(v) - 1] << "    " << vb[0] << " " << vb[sz(vb) - 1] << endl;
        sum += vb[sz(vb) - 1] - v[sz(v) - 1];
    }
    cerr << "diff " << sum / n_machine << endl;

    cerr << "ucb" << endl; ucb(agt, env);
    cerr << "ucb2_0" << endl; ucb2(agt, contact_nodes, env, 0);
    cerr << "ucb2_1" << endl; ucb2(agt, contact_nodes, env, 1);
}
