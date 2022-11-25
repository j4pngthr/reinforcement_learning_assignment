#ifndef ucb_hpp
#define ucb_hpp

#include"global.hpp"
#include"mab.hpp"

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

        double r = env.bandit(arm_id, machine_id, t, agt); // agt.X[arm_id][t]

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
        // int cnt = 0; // 引けるレバーの個数
        rep(k, n_arm) { // arm_idを求める
            if (!agt.pullable[k]) continue;
            // ++cnt;

            if (agt.n[k][t] == 0) {
                arm_id = k;
                break;
            } else {
                double temp = agt.Q[k][t] + c * sqrt(log(t) / agt.n[k][t]);
                // if (machine_id == 1 && t == 3) cerr << agt.Q[k][t] << " " << agt.n[k][t] << " " << temp << endl;
                if (chmax(ma, temp)) {
                    arm_id = k;
                }
            }
        }
        // assert(cnt); // cntは正でなければならない
        // if (arm_id == -1) {
        //     cerr << "arm_id = -1, machine_id, t " << machine_id << " " << t << endl;
        // }
        assert(arm_id != -1);

        double r = env.bandit(arm_id, machine_id, t, agt) + agt.bias[arm_id]; // mab.hpp
        // double r = env.bandit(arm_id, machine_id, t, agt);
        // double r = _r + agt.bias[arm_id];
        // if (t == MAX_STEPS - 1) cerr << machine_id << " " << arm_id << " " << _r << " " << agt.bias[arm_id] << endl;

        ++agt.n[arm_id][t];

        if (agt.n[arm_id][t] == 1) { // 最初
            agt.Q[arm_id][t] = r;
        } else {
            agt.Q[arm_id][t] = agt.Q[arm_id][t] + 1.0 / agt.n[arm_id][t] * (r - agt.Q[arm_id][t]);
        }
        return arm_id;
    }
};

void ucb(vector<Agent> &agt, vector<MAB> &env);
void ucb2(vector<Agent> &agt, vector<vector<pii> > &contact_nodes, vector<MAB> &env, int excInfo, int weighted);

#endif
