#include"global.hpp"
#include"mab.hpp"
#include"ucb.hpp"
#include"gossip.hpp"
#include"getRealTrace.hpp"

void gossip(vector<Agent> &agt, vector<vector<pii> > &contact_nodes, vector<MAB> &env) {
    rep(machine_id, n_machine) agt[machine_id].init();

    vector<Gossip> a(n_machine);
    // aの初期化
    rep(machine_id, n_machine) {
        a[machine_id].init(env[machine_id]);
    }

    { // t = 0
        rep(machine_id, n_machine) {
            rep(k, n_arm) {
                if (!agt[machine_id].pullable[k]) continue;

                double X = env[machine_id].bandit(k, machine_id, 0, agt[machine_id]); // mab.hpp, arm_id, machine_id, t, agt
                agt[machine_id].X[k][0] = X; // action/banditがないから
                agt[machine_id].n[k][0] = 1;
                agt[machine_id].X_tilde[k][0] = agt[machine_id].theta[k][0] = agt[machine_id].X[k][0];
            }
        }
    }

    rep3(t, 1, MAX_STEPS) {
        if (t % 1000 == 0) cerr << "t " << t << endl;

        // レバーを引く
        rep(i, n_machine) {
            int arm_id = a[i].action(i, t, agt); // mab.hpp/banditでXに代入
            calcXTilde(i, arm_id, t, agt[i]); // SIGMETRIC2021 line 15, global.cpp
            ++agt[i].n[arm_id][t];
        }

        // 情報交換
        vector<int> used(n_machine);
        for (pii p : contact_nodes[t]) {
            int i = p.F, j = p.S;
            used[i] = 1; used[j] = 1;
            assert(i >= 0 && i < n_machine && j >= 0 && j < n_machine);

            rep(k, n_arm) {
                if (!agt[i].pullable[k] && !agt[j].pullable[k]) continue;
                else if (agt[i].pullable[k] && agt[j].pullable[k]) {
                    rep(_, 2) {
                        agt[i].theta[k][t] = (agt[i].theta[k][t - 1] + agt[j].theta[k][t - 1]) / 2.0 + agt[i].X_tilde[k][t] - agt[i].X_tilde[k][t - 1];
                        swap(i, j);
                    }
                } else if (agt[i].pullable[k]) {
                    agt[j].pullable[k] = 1;
                } else if (agt[j].pullable[k]) {
                    agt[i].pullable[k] = 1;
                }
            }
        }
        // tに関して更新されていないものを更新
        rep(i, n_machine) {
            if (used[i]) continue;

            rep(k, n_arm) {
                agt[i].theta[k][t] = agt[i].theta[k][t - 1] + (agt[i].X_tilde[k][t] - agt[i].X_tilde[k][t - 1]);
            }
        }
    }

    output("gossip.txt", agt); // global.cpp
}
