#ifndef mab_hpp
#define mab_hpp

#include"global.hpp"

class MAB {
    vector<double> mu, sig;
    int inited = 0;
public:
    MAB() {}
    MAB(vector<double> _mu, vector<double> _sig) : mu(_mu), sig(_sig) {}

    void init(vector<double> _mu, vector<double> _sig) {
        inited = 1;

        mu = _mu; sig = _sig;
    }

    double getMu(int i) { return mu[i]; }

    double bandit(int arm_id, int machine_id, int t, Agent &agt) {
        assert(inited);
        assert(arm_id >= 0 && arm_id < n_arm);

        random_device seed_gen;
        default_random_engine engine(seed_gen());
        normal_distribution<> dist(mu[arm_id], sig[arm_id]);
        double r = dist(engine);
        agt.X[arm_id][t] = r;
        // agt.X[arm_id][t] = r + agt.bias[arm_id];

        return r;
    }
};

#endif
