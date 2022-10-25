#include"global.hpp"
#include"ucb.hpp"

void ucb(vector<Lever> &lv) {
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
                int arm_id = a.action(machine_id, t, agt[machine_id]);
                agt[machine_id].a[t] = arm_id;
            }
        }

        output("ucb.txt", agt);
    }
}
