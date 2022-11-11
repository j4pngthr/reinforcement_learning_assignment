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

                double X = env[machine_id].bandit(k, machine_id, 0, agt[machine_id]);
                agt[machine_id].theta[k][0] = X;
                agt[machine_id].X_tilde[k][0] = X;
                agt[machine_id].n[k][0] = 1;
            }
        }
    }

    rep3(t, 1, MAX_STEPS) {
        if (t % 1000 == 0) cerr << "t " << t << endl;

        // レバーを引く
        rep(machine_id, n_machine) {
            int arm_id = a[machine_id].action(machine_id, t, agt); // Xを求める
            calcXTilde(machine_id, arm_id, t, agt[machine_id]); // SIGMETRIC2021 line 15, global.cpp
        }

        // 情報交換
        vector<int> used(n_machine);
        for (pii p : contact_nodes[t]) {
            int i = p.F, j = p.S;
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
        rep(machine_id, n_machine) {
            if (used[machine_id]) continue;

            rep(k, n_arm) {
                agt[machine_id].theta[k][t] = agt[machine_id].theta[k][t - 1] + (agt[machine_id].X_tilde[k][t] - agt[machine_id].X_tilde[k][t - 1]);
            }
        }
    }

    output("gossip.txt", agt); // global.cpp
}

// void gossip(vector<Lever> &lv) {
//     vector<Agent> agt(n_machine);
//     vector<vector<pii> > contact_nodes;
//     getRealTrace(contact_nodes, agt);
//
//     rep(machine_id, n_machine) {
//         if (machine_id % 10 == 0) cerr << "machine_id " << machine_id << endl;
//
//         vector<double> _mu, _sig;
//         rep(k, n_arm) {
//             _mu.eb(lv[k].getMu());
//             _sig.eb(1);
//         }
//         MAB env(_mu, _sig);
//         Gossip a(env);
//
//         { // t = 0
//             rep(k, n_arm) {
//                 if (!agt[machine_id].pullable[k]) continue;
//
//                 double X = lv[k].play();
//                 agt[machine_id].theta[k][0] = X;
//                 agt[machine_id].X_tilde[k][0] = X;
//                 agt[machine_id].n[k][0] = 1;
//             }
//         }
//         rep3(t, 1, MAX_STEPS) {
//             int arm_id = a.action(machine_id, t, agt); // Xを求める
//             calcXTilde(machine_id, arm_id, t, agt[machine_id]); // SIGMETRIC2021 line 15, global.cpp
//
//             vector<int> used(n_machine);
//             for (pii p : contact_nodes[t]) {
//                 int i = p.F, j = p.S;
//                 assert(i >= 0 && i < n_machine && j >= 0 && j < n_machine);
//
//                 rep(k, n_arm) {
//                     if (!agt[i].pullable[k] && !agt[j].pullable[k]) continue;
//                     else if (agt[i].pullable[k] && agt[j].pullable[k]) {
//                         rep(_, 2) {
//                             agt[i].theta[k][t] = (agt[i].theta[k][t - 1] + agt[j].theta[k][t - 1]) / 2.0 + agt[i].X_tilde[k][t] - agt[i].X_tilde[k][t - 1];
//                             swap(i, j);
//                         }
//                     } else if (agt[i].pullable[k]) {
//                         agt[j].pullable[k] = 1;
//                     } else if (agt[j].pullable[k]) {
//                         agt[i].pullable[k] = 1;
//                     }
//                 }
//             }
//             rep(i, n_machine) {
//                 if (used[i]) continue;
//                 rep(k, n_arm) {
//                     agt[i].theta[k][t] = agt[i].theta[k][t - 1] + agt[i].X_tilde[k][t] - agt[i].X_tilde[k][t - 1];
//                 }
//             }
//         }
//     }
//
//     string filename = "gossip.txt";
//     ofstream ofs(filename, ios::out);
//     rep(t, MAX_STEPS) {
//         if (t % 100 != 0) continue;
//
//         double ave = 0;
//         rep(machine_id, n_machine) {
//             int arm_id = agt[machine_id].a[t];
//             ave += agt[machine_id].X[arm_id][t];
//         }
//         ofs << t << " " << ave / n_machine << endl;
//     }
// }
