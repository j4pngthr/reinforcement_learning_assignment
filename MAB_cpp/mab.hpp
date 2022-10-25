#ifndef mab_hpp
#define mab_hpp

#include"global.hpp"

class MAB {
    vector<double> mu, sig;
public:
    MAB(vector<double> mu, vector<double> sig) : mu(mu), sig(sig) {

    }
    double bandit(int arm_id, int machine_id, int t, Agent &agt) {
        assert(arm_id >= 0 && arm_id < n_arm);

        random_device seed_gen;
        default_random_engine engine(seed_gen());
        normal_distribution<> dist(mu[arm_id], sig[arm_id]);
        double r = dist(engine);
        agt.X[arm_id][t] = r;

        return r;
    }
};

#endif
