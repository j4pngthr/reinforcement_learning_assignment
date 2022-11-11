#include"global.hpp"
#include"ucb.hpp"

// global knowledge, バイアスなし
void ucb(vector<Agent> &agt, vector<MAB> &env) {
    for (double c : {2}) {
        rep(machine_id, n_machine) agt[machine_id].init();

        vector<Ucb> a(n_machine);
        // aの初期化
        rep(machine_id, n_machine) {
            a[machine_id].init(c, env[machine_id]);
        }

        rep(t, MAX_STEPS) {
            if (t % 1000 == 0) cerr << "t " << t << endl;

            rep(machine_id, n_machine) {
                int arm_id = a[machine_id].action(machine_id, t, agt[machine_id]); // ucb.hpp
                agt[machine_id].a[t] = arm_id;
            }
        }

        output("ucb.txt", agt); // global.cpp/agt.X
    }
}

// 情報交換あり, バイアスあり
void ucb2(vector<Agent> &agt, vector<vector<pii> > &contact_nodes, vector<MAB> &env, int excInfo) {
    for (double c : {2}) {
        rep(machine_id, n_machine) agt[machine_id].init();

        vector<Ucb2> a(n_machine);
        rep(machine_id, n_machine) {
            a[machine_id].init(c, env[machine_id]);
        }

        { // t = 0
            rep(machine_id, n_machine) {
                rep(k, n_arm) {
                    if (!agt[machine_id].pullable[k]) continue;

                    double X = env[machine_id].bandit(k, machine_id, 0, agt[machine_id]);
                    agt[machine_id].theta[k][0] = X;
                    agt[machine_id].X_tilde[k][0] = X;
                }
            }
        }

        // int cnt = 0;
        rep3(t, 1, MAX_STEPS) {
            if (t % 1000 == 0) cerr << "t " << t << endl;

            // tに関して更新されていない値を更新
            rep(machine_id, n_machine) {
                rep(k, n_arm) {
                    agt[machine_id].X[k][t] = agt[machine_id].X[k][t - 1];
                    agt[machine_id].X_tilde[k][t] = agt[machine_id].X_tilde[k][t - 1];
                    agt[machine_id].theta[k][t] = agt[machine_id].theta[k][t - 1];
                    agt[machine_id].n[k][t] = agt[machine_id].n[k][t - 1];
                    agt[machine_id].Q[k][t] = agt[machine_id].Q[k][t - 1];
                    agt[machine_id].n_tilde[k][t] = agt[machine_id].n_tilde[k][t - 1];
                }
            }

            // 各エージェントがレバーを引く
            rep(machine_id, n_machine) {
                int arm_id = a[machine_id].action(machine_id, t, agt[machine_id]); // Xを求める
                agt[machine_id].a[t] = arm_id;
                // calcXTilde(machine_id, arm_id, t, agt[machine_id]);
            }

            if (!excInfo) continue;
            // 情報交換
            vector<int> used(n_machine);
            for (pii p : contact_nodes[t]) {
                int i = p.F, j = p.S;
                // ++cnt;
                // if (t >= 1000 && t % 100 == 0) cerr << t << " " << cnt << endl;
                assert(i >= 0 && i < n_machine && j >= 0 && j < n_machine);

                rep(k, n_arm) {
                    if (!agt[i].pullable[k] && !agt[j].pullable[k]) continue;
                    else if (agt[i].pullable[k] && agt[j].pullable[k]) {
                        rep(_, 2) {
                            agt[i].theta[k][t] = (agt[i].theta[k][t - 1] + agt[j].theta[k][t - 1]) / 2.0;
                            // agt[i].theta[k][t] = (agt[i].theta[k][t - 1] + agt[j].theta[k][t - 1]) / 2.0 + agt[i].X_tilde[k][t] - agt[i].X_tilde[k][t - 1];
                            swap(i, j);
                        }
                    } else if (agt[i].pullable[k]) {
                        agt[j].pullable[k] = 1;
                    } else if (agt[j].pullable[k]) {
                        agt[i].pullable[k] = 1;
                    }
                }
            }

            // tに関して更新されていない値を更新
            // rep(machine_id, n_machine) {
            //     if (used[machine_id]) continue;
            //
            //     rep(k, n_arm) {
            //         if (!agt[machine_id].pullable[k]) continue;
            //
            //         agt[machine_id].theta[k][t] = agt[machine_id].theta[k][t - 1] + (agt[machine_id].X_tilde[k][t] - agt[machine_id].X_tilde[k][t - 1]);
            //     }
            // }

            // Q←\theta
            rep(machine_id, n_machine) {
                rep(k, n_arm) agt[machine_id].Q[k][t] = agt[machine_id].theta[k][t];
            }
        }

        string filename = "ucb2_1.txt";
        if (!excInfo) filename = "ucb2_0.txt";
        output(filename, agt);
    }
}
