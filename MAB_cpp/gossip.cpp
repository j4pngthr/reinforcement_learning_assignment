#include"global.hpp"
#include"mab.hpp"
#include"ucb.hpp"
#include"gossip.hpp"
#include"getRealTrace.hpp"

void gossip(vector<Agent> &agt, vector<vector<pii> > &contact_nodes, vector<MAB> &env) {
    rep(i, n_machine) agt[i].init();

    vector<Gossip> a(n_machine);
    // aの初期化
    rep(i, n_machine) {
        a[i].init(env[i]);
    }

    { // t = 0
        rep(i, n_machine) {
            rep(k, n_arm) {
                if (!agt[i].pullable[k]) continue;

                double X = env[i].bandit(k, i, 0, agt[i]); // mab.hpp, arm_id, machine_id, t, agt
                agt[i].X[k][0] = X; // action/banditがないから
                agt[i].n[k][0] = 1;
                agt[i].theta[k][0] = X;
                agt[i].X_tilde[k][0] = X;
                // agt[i].a[0]は使えない
            }
        }
    }

    // agt.thetaは必ず更新される
    // 使うの agt.n, X, X_tilde, Q, n_tilde, a, theta
    rep3(t, 1, MAX_STEPS) {
        // if (t % 1000 == 0) cerr << "t " << t << endl;

        // t←t-1する必要があるもの
        rep(i, n_machine) {
            rep(k, n_arm) {
                // n, n_tilde, a
                // gossip.hppでやってる
                // 不必要そうなの
                // X, X_tilde, Q
                // theta
                // 下でやってる

                // arm_idしか更新してない
                agt[i].X_tilde[k][t] = agt[i].X_tilde[k][t - 1];
            }
        }

        // レバーを引く
        rep(i, n_machine) {
            int arm_id = a[i].action(i, t, agt); // mab.hpp/banditでXに代入
            ++agt[i].n[arm_id][t];
            calcXTilde(i, arm_id, t, agt[i]); // SIGMETRIC2021 line 15, global.cpp
        }

        // 情報交換
        excInfo(t, agt, contact_nodes, 1, 0, vector<double>(), 0, 1); // exc_Q, weighted, need_ngb, update_unused
    }
}
