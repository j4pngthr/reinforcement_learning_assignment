#ifndef mab_hpp
#define mab_hpp

#include"global.hpp"

class MAB {
    vector<double> mu;
    double sig;
public:
    MAB(vector<double> mu, double sig) : mu(mu), sig(sig) {

    }
    double bandit(int arm_id, params &p) {
        assert(arm_id >= 0 && arm_id < n_arm);

        random_device seed_gen;
        default_random_engine engine(seed_gen());
        normal_distribution<> dist(mu[arm_id], sig);
        double r = dist(engine);
        p.avg_mac_rs[p.t] += r / n_machine;
        // cerr << p.t << " " << p.avg_mac_rs[p.t] << endl;
        return r;
    }
};

#endif
