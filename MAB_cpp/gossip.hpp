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

            agt[i].n[k][t] = agt[i].n[k][t - 1];
            {
                chmax(agt[i].n_tilde[k][t + 1], agt[i].n[k][t]);
                for (int j : agt[i].e) {
                    chmax(agt[i].n_tilde[k][t + 1], agt[j].n_tilde[k][t]);
                }
            }
            if (agt[i].n[k][t] < agt[i].n_tilde[k][t] - n_machine) {
                A.eb(k);
            }
        }

        int arm_id = -1;
        double max_val = -100000.0;
        if (A.empty()) { // arm_idを決める, line 7
            rep(k, n_arm) {
                if (!agt[i].pullable[k]) continue;

                double alpha1 = 64.0; // Cの1つ上の式, p13
                rep(_, 17) alpha1 /= n_machine;
                double C = sqrt(2.0 * n_machine / agt[i].n[k][t] * log(t)) + alpha1; // p13
                agt[i].Q[k][t] = agt[i].theta[k][t - 1] + C;
                if (chmax(max_val, agt[i].Q[k][t])) {
                    arm_id = k;
                }
            }
        } else { // line 12
            arm_id = A[rand() % sz(A)];
        }
        assert(arm_id != -1);
        agt[i].a[t] = arm_id;

        double r = env.bandit(arm_id, i, t, agt[i]) + agt[i].bias[arm_id];
        agt[i].X[arm_id][t] = r;

        if (agt[i].n[arm_id][t] == 1) { // 最初
            agt[i].Q[arm_id][t] = r;
        } else {
            agt[i].Q[arm_id][t] = agt[i].Q[arm_id][t] + 1.0 / agt[i].n[arm_id][t] * (r - agt[i].Q[arm_id][t]);
        }
        return arm_id;
    }
};

#endif
