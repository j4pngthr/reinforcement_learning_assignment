#include"global.hpp"
#include"mab.hpp"
#include"ucb.hpp"
// #include"proposed.hpp"

void calcXTilde(int i, int k, int t, Agent &agt) {
    double sum = 0;
    int cnt = 0;
    rep(tau, t) {
        if (agt.a[tau] != k) continue;
        sum += agt.X[k][tau];
        ++cnt;
    }
    if (cnt == 0) agt.X_tilde[k][t] = 0;
    else agt.X_tilde[k][t] = sum / cnt;
}

void proposed(vector<Lever> &lv, vector<vector<pii> > &contact_nodes) {
    for (double c : {2}) {
        // rep(i, MAX_STEPS) {
        //     p.avg_mac_rs[i] = 0;
        // }

        vector<Agent> agt(n_machine);

        rep(machine_id, n_machine) {
            vector<double> _mu, _sig;
            rep(k, n_arm) {
                _mu.eb(lv[k].getMu());
                _sig.eb(1);
            }
            MAB env(_mu, _sig);
            Ucb a(env, c, init_val);

            rep(t, MAX_STEPS) {
                int arm_id = a.action(machine_id, t, agt[machine_id]); // Xを求める
                // if (t < 1) cerr << machine_id << " " << agt[machine_id].X[arm_id][t] << endl;
                agt[machine_id].a[t] = arm_id;
                calcXTilde(machine_id, arm_id, t, agt[machine_id]);
                // if (t < 1) cerr << machine_id << " " << agt[machine_id].X_tilde[arm_id][t] << endl;

                vector<int> used(n_machine);
                for (pii p : contact_nodes[t]) {
                    int i = p.F, j = p.S;
                    assert(i >= 0 && i < n_machine && j >= 0 && j < n_machine);

                    rep(k, n_arm) {
                        rep(_, 2) {
                            agt[i].theta[k][t] = (agt[i].theta[k][t - 1] + agt[j].theta[k][t - 1]) / 2.0 + agt[i].X_tilde[k][t] - agt[i].X_tilde[k][t - 1];
                            swap(i, j);
                        }
                    }
                }
                rep(i, n_machine) {
                    if (used[i]) continue;
                    rep(k, n_arm) {
                        agt[i].theta[k][t] = agt[i].theta[k][t - 1] + agt[i].X_tilde[k][t] - agt[i].X_tilde[k][t - 1];
                    }
                }
            }
        }

        string filename = "proposed.txt";
        ofstream ofs(filename, ios::out);
        rep(t, MAX_STEPS) {
            double ave = 0;
            rep(machine_id, n_machine) {
                int arm_id = agt[machine_id].a[t];
                ave += agt[machine_id].theta[arm_id][t];
            }
            ofs << t << " " << ave / n_machine << endl;
        }
    }
}
