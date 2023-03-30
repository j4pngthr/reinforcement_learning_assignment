#ifndef gossip_hpp
#define gossip_hpp

void gossip(vector<Agent> &agt, vector<vector<pii> > &contact_nodes, vector<MAB> &env);

#include"mab.hpp"

class Gossip {
    MAB env;
public:
    Gossip() {}
    Gossip(MAB env) : env(env) {

    }

    void init(MAB _env) {
      env = _env;
    }

    int action(int i, int t, vector<Agent> &agt) {
        vector<int> A; // 引いた数の少ないレバーの集合
        rep(k, n_arm) {
            if (!agt[i].pullable[k]) continue;

            // iのt-1だからok
            agt[i].n[k][t] = agt[i].n[k][t - 1];
            {
                chmax(agt[i].n_tilde[k][t + 1], agt[i].n[k][t]);
                for (int j : agt[i].e) {
                    if (agt[i].met_last[j] == t) chmax(agt[i].n_tilde[k][t + 1], agt[j].n_tilde[k][t]);
                    // chmax(agt[i].n_tilde[k][t + 1], agt[j].n_tilde[k][t]);
                }
            }
            // if (i == 0 && t >= 500 && t < 511 && k == 0) cerr << "t k n n_tilde " << t << " " << k << " " << agt[i].n[k][t] << " " << agt[i].n_tilde[k][t] << endl;
            if (agt[i].n[k][t] < agt[i].n_tilde[k][t] - n_machine) {
                A.eb(k);
            }
        }
        // if (sz(A)) cerr << "A is not empty    t i A " << t << " " << i << " " << A << endl;

        int arm_id = -1;
        if (A.empty()) { // arm_idを決める, line 7
            rep(k, n_arm) {
                if (!agt[i].pullable[k]) continue;

                if (agt[i].n[k][t] == 0) {
                    arm_id = k;
                    A.eb(k);
                    break;
                }
            }

            if (arm_id == -1) {
                double ma = -100000.0;
                rep(k, n_arm) {
                    if (!agt[i].pullable[k]) continue;

                    // double alpha1 = 64.0; // Cの1つ上の式, p13
                    // rep(_, 17) alpha1 /= n_machine;
                    assert(agt[i].n[k][t]);
                    double C = sqrt(2.0 * n_machine / agt[i].n[k][t] * log(t)); // + alpha1; // p13
                    agt[i].Q[k][t] = agt[i].theta[k][t - 1] + C;
                    // if (i == 0 && t < 20) cerr << "k t theta " << k << " " << t << " " << agt[i].theta[k][t - 1] << endl;
                    chmax(ma, agt[i].Q[k][t]);
                }

                rep(k, n_arm) {
                    if (!agt[i].pullable[k]) continue;

                    if (abs(agt[i].Q[k][t] - ma) < eps) A.eb(k);
                }
            }
        }
        arm_id = A[rand() % sz(A)];
        assert(arm_id != -1);
        agt[i].a[t] = arm_id;
        assert(agt[i].pullable[arm_id]);

        // if (i == 0 && t < 20) {
        //     cerr << "t arm_id " << t << " " << arm_id << endl;
        //     cerr << "A \"" << A << "\""<< endl;
        //     cerr << "Q ";
        //     rep(k, n_arm) cerr << agt[i].Q[k][t] << " "; cerr << endl;
        // }

        double r = env.bandit(arm_id, i, t, agt[i]); + agt[i].bias[arm_id];

        if (agt[i].n[arm_id][t] == 1) { // 最初
            agt[i].Q[arm_id][t] = r;
        } else {
            agt[i].Q[arm_id][t] = agt[i].Q[arm_id][t] + 1.0 / agt[i].n[arm_id][t] * (r - agt[i].Q[arm_id][t]);
        }
        return arm_id;
    }
};

#endif
