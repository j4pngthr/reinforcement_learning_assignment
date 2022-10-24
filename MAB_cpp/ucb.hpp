#ifndef ucb_hpp
#define ucb_hpp

// #include"bandit_algorithm.hpp"

class Ucb /* : public Agent */ {
    MAB env;
    double c, init_val;
    vector<double> Q;
    vector<int> N;
public:

    Ucb(MAB env, double c, double init_val) : env(env), c(c), init_val(init_val) {
        Q.resize(n_arm);
        N.resize(n_arm);

        if (init_val != 0) {
            rep(i, n_arm) Q[i] = init_val;
        }
    }

    // int chooseArm(params &p) {
    //     double ma = -100000.0, c = epsilon;
    //     int arm_id = -1;
    //     for (int i = 0; i < n_arm; ++i) {
    //         double temp = -100000.0;
    //         if (N[i] == 0) {
    //             arm_id = i;
    //             break;
    //         } else {
    //             temp = Q[i] + c * sqrt(log(p.t) / N[i]);
    //             if (chmax(ma, temp)) {
    //                 arm_id = i;
    //             }
    //         }
    //     }
    //     assert(arm_id != -1);
    //     return arm_id;
    // }

    int action(int machine_id, params &p) {
        double ma = -100000.0;
        int arm_id = -1;
        for (int i = 0; i < n_arm; ++i) {
            double temp = -100000.0;
            if (N[i] == 0) {
                arm_id = i;
                break;
            } else {
                temp = Q[i] + c * sqrt(log(p.t) / N[i]);
                if (chmax(ma, temp)) {
                    arm_id = i;
                }
            }
        }
        assert(arm_id != -1);

        double r = env.bandit(arm_id, machine_id, p);
        // cerr << p.t << " " << p.avg_mac_rs[p.t] << endl;
        ++N[arm_id];

        if (N[arm_id] == 1) { // 最初
            Q[arm_id] = r;
        } else {
            Q[arm_id] = Q[arm_id] + 1.0 / N[arm_id] * (r - Q[arm_id]);
        }
        return arm_id;
    }
};

#endif
