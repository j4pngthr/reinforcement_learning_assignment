#ifndef egreedy_hpp
#define egreedy_hpp

class Agent {
    MAB env;
    double epsilon, init_val;
    vector<double> Q;
    vector<int> N;
public:

    Agent(MAB env, double epsilon, double init_val) : env(env), epsilon(epsilon), init_val(init_val) {
        Q.resize(n_arm);
        N.resize(n_arm);

        if (init_val != 0) {
            rep(i, n_arm) Q[i] = init_val;
        }
    }

    int action(params &p) {
        int arm_id;
        if (epsilon > 1.0 * rand() / RAND_MAX) {
            arm_id = rand() % n_arm;
        } else {
            double ma = -1000000.0;
            rep(i, n_arm) if (chmax(ma, Q[i])) arm_id = i;
        }

        double r = env.bandit(arm_id, p);
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
