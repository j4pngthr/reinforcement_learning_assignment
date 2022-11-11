#ifndef ucb_hpp
#define ucb_hpp

#include"global.hpp"
#include"mab.hpp"

// class Ucb /* : public Agent */ {
//     MAB env;
//     double c, init_val;
//     // vector<double> Q;
//     // vector<int> N;
// public:
//     Ucb(MAB env, double c, double init_val) : env(env), c(c), init_val(init_val) {
//         // Q.resize(n_arm);
//         // N.resize(n_arm);
//
//         // if (init_val != 0) {
//         //     rep(i, n_arm) Q[i] = init_val;
//         // }
//     }
//
//     // Qはレバーを決めるのに使う
//     // 出力はこれまでもX
//     int action(int machine_id, int t, Agent &agt) {
//         double ma = -100000.0;
//         int arm_id = -1;
//         rep(i, n_arm) { // arm_idを求める
//             if (!agt.pullable[i]) continue;
//
//             double temp = -100000.0;
//             if (agt.n[i][t] == 0) {
//                 arm_id = i;
//                 break;
//             } else {
//                 temp = agt.Q[i][t] + c * sqrt(log(t) / agt.n[i][t]);
//                 if (chmax(ma, temp)) {
//                     arm_id = i;
//                 }
//             }
//         }
//         assert(arm_id != -1);
//
//         double r = env.bandit(arm_id, machine_id, t, agt);
//
//         ++agt.n[arm_id][t];
//
//         if (agt.n[arm_id][t] == 1) { // 最初
//             agt.Q[arm_id][t] = r;
//         } else {
//             agt.Q[arm_id][t] = agt.Q[arm_id][t] + 1.0 / agt.n[arm_id][t] * (r - agt.Q[arm_id][t]);
//         }
//         return arm_id;
//     }
// };

// global knowledge, バイアスなし
class Ucb /* : public Agent */ {
    MAB env;
    double c, init_val;
    vector<double> Q;
    vector<int> N;
public:
    Ucb() {}

    void init(double _c, MAB &_env) {
        Q.resize(n_arm);
        N.resize(n_arm);

        c = _c;
        env = _env;
    }

    // Qはレバーを決めるのに使う
    // 出力はこれまでもX
    int action(int machine_id, int t, Agent &agt) {
        double ma = -100000.0;
        int arm_id = -1;

        rep(k, n_arm) { // arm_idを求める
            if (!agt.pullable[k]) continue;

            double temp = -100000.0;
            if (N[k] == 0) {
                arm_id = k;
                break;
            } else { // N[k] != 0
                temp = Q[k] + c * sqrt(log(t) / N[k]);
                if (chmax(ma, temp)) {
                    arm_id = k;
                }
            }
        }
        assert(arm_id != -1);

        double r = env.bandit(arm_id, machine_id, t, agt);

        ++N[arm_id];

        if (N[arm_id] == 1) { // 最初
            Q[arm_id] = r;
        } else {
            Q[arm_id] = Q[arm_id] + 1.0 / N[arm_id] * (r - Q[arm_id]);
        }
        return arm_id;
    }
};

// 情報交換あり, バイアスあり
class Ucb2 /* : public Agent */ {
    MAB env;
    double c, init_val;
    // vector<double> Q;
    // vector<int> N;
public:
    Ucb2() {}

    void init(double _c, MAB _env) {
        c = _c;
        env = _env;
    }

    // Qはレバーを決めるのに使う
    // 出力はこれまでもX
    int action(int machine_id, int t, Agent &agt) {
        double ma = -100000.0;
        int arm_id = -1;
        rep(k, n_arm) { // arm_idを求める
            if (!agt.pullable[k]) continue;

            double temp = -100000.0;
            if (agt.n[k][t] == 0) {
                arm_id = k;
                break;
            } else {
                temp = agt.Q[k][t] + c * sqrt(log(t) / agt.n[k][t]);
                if (chmax(ma, temp)) {
                    arm_id = k;
                }
            }
        }
        assert(arm_id != -1);

        double r = env.bandit(arm_id, machine_id, t, agt) + agt.bias[arm_id];

        ++agt.n[arm_id][t];

        if (agt.n[arm_id][t] == 1) { // 最初
            agt.Q[arm_id][t] = r;
        } else {
            agt.Q[arm_id][t] = agt.Q[arm_id][t] + 1.0 / agt.n[arm_id][t] * (r - agt.Q[arm_id][t]);
        }
        return arm_id;
    }
};

// class BiasedUcb /* : public Agent */ {
//     MAB env;
//     double c, init_val;
//     vector<double> Q;
//     vector<int> N;
// public:
//     BiasedUcb(MAB env, double c, double init_val) : env(env), c(c), init_val(init_val) {
//         Q.resize(n_arm);
//         N.resize(n_arm);
//
//         if (init_val != 0) {
//             rep(i, n_arm) Q[i] = init_val;
//         }
//     }
//
//     // Qはレバーを決めるのに使う
//     // 出力はこれまでもX
//     int action(int machine_id, int t, Agent &agt) {
//         double ma = -100000.0;
//         int arm_id = -1;
//         rep(i, n_arm) { // arm_idを求める
//             if (!agt.pullable[i]) continue;
//
//             double temp = -100000.0;
//             if (N[i] == 0) {
//                 arm_id = i;
//                 break;
//             } else {
//                 temp = Q[i] + c * sqrt(log(t) / N[i]);
//                 if (chmax(ma, temp)) {
//                     arm_id = i;
//                 }
//             }
//         }
//         assert(arm_id != -1);
//
//         double r = env.bandit(arm_id, machine_id, t, agt) + agt.bias[arm_id];
//
//         ++N[arm_id];
//
//         if (N[arm_id] == 1) { // 最初
//             Q[arm_id] = r;
//         } else {
//             Q[arm_id] = Q[arm_id] + 1.0 / N[arm_id] * (r - Q[arm_id]);
//         }
//         return arm_id;
//     }
// };

void ucb(vector<Agent> &agt, vector<MAB> &env);
void ucb2(vector<Agent> &agt, vector<vector<pii> > &contact_nodes, vector<MAB> &env, int excInfo);

#endif
