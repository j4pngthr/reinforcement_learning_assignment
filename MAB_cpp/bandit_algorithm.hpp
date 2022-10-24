// #ifndef bandit_algorithm_hpp
// #define bandit_algorithm_hpp
//
// class Agent {
// public:
//     MAB env;
//     double epsilon, init_val;
//     vector<double> Q;
//     vector<int> N;
//
//     void init(MAB _env, double _epsilon, double _init_val) {
//         env = _env;
//         epsilon = _epsilon;
//         init_val = _init_val;
//
//         Q.resize(n_arm);
//         N.resize(n_arm);
//
//         if (init_val != 0) {
//             rep(i, n_arm) Q[i] = init_val;
//         }
//     }
// 
//     virtual int chooseArm();
//     virtual int chooseArm(params &p);
//
//     int action(params &p) {
//         int arm_id = chooseArm();
//         double r = env.bandit(arm_id, p);
//         // cerr << p.t << " " << p.avg_mac_rs[p.t] << endl;
//         ++N[arm_id];
//
//         if (N[arm_id] == 1) { // 最初
//             Q[arm_id] = r;
//         } else {
//             Q[arm_id] = Q[arm_id] + 1.0 / N[arm_id] * (r - Q[arm_id]);
//         }
//         return arm_id;
//     }
//
//     // double getQ(int i) {
//     //     return Q[i];
//     // }
// };
//
// #endif
