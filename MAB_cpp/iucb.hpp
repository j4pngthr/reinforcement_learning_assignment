#ifndef iucb_hpp
#define iucb_hpp

#include"global.hpp"
#include"mab.hpp"

class Iucb {
    MAB env;
    double c, init_val;
public:
    Iucb() {}

    void init(double _c, MAB _env) {
        c = _c;
        env = _env;
    }

    // Qはレバーを決めるのに使う
    // 出力はこれまでもX
    int action(int i, int t, Agent &agt, vector<Agent> &_agt, double alpha, bool isGlobal) {
        int arm_id = -1;
        double _v = (double)rand() / RAND_MAX;

        // cerr << v << " " << alpha << " " << (v >= alpha) << endl;
        // if (i == 0 && t >= 500 && t < 511) {
        //     if (_v >= alpha) cerr << "ucb" << endl;
        //     else cerr << "popular" << endl;
        // }

        if (_v >= alpha) { // ucb, O(M)
            // if (i == 0 && t >= 500 && t < 511) cerr << "ucb" << " ";
            double ma = -100000.0;
            rep(k, n_arm) {
                if (!agt.pullable[k]) continue;

                if (agt.n[k][t] == 0) {
                    arm_id = k;
                    break;
                } else {
                    double temp = agt.Q[k][t] + c * sqrt(log(t) / agt.n[k][t]);
                    if (chmax(ma, temp)) {
                        arm_id = k;
                    }
                }
            }
            // if (i == 0 && t < 10) cerr << "ucb " << arm_id << endl;
        } else { // popular
            // if (i == 0 && t >= 500 && t < 511) cerr << "popular" << " ";
            vector<pii> v;
            rep(k, n_arm) { // O(MNs+MlogM)=O(MNs)
                if (!agt.pullable[k]) continue;

                int p = 0;
                for (int j : agt.e) {
                    for (int s = max(0, t - 10); s < t; ++s) {
                        // 出会ってたらそれまでに引いたレバーの情報をもらえてるとする
                        bool cond = (agt.met_last[j] >= s && _agt[j].a[s] == k);
                        if (isGlobal) cond = (_agt[j].a[s] == k);
                        if (cond) {
                            // if (i == 0) cerr << t << " " << s << " " << k << "    " << i << " " << j << endl;
                            // if (i == 0 && t < 10) cerr << "pcount " << k << " " << j << " " << s << endl;
                            ++p;
                        }
                    }
                }
                v.eb(p, k);
                // if (i == 0 && t >= 500 && t < 511) cerr << "p " << p << endl;
                // if (!isGlobal && p && t < 1000) cerr << i << " " << t << " " << p << endl;
            }
            sort(all(v), greater<>()); // O(MlogM)
            // if (i == 0 && t < 10) {
            //     cerr << t << "    ";
            //     rep(k, n_arm) cerr << v[k].F << " " << v[k].S << "    ";
            // }
            vector<int> A;
            rep(k, n_arm) {
                if (v[k].F == v[0].F) A.eb(v[k].S);
                else break;
            }
            arm_id = A[rand() % sz(A)];
            // if (i == 0 && t < 10) {
            //     cerr << "A " << A << endl;
            //     cerr << "arm_id " << arm_id << endl;
            // }
        }
        assert(arm_id != -1);

        double r = env.bandit(arm_id, i, t, agt) + agt.bias[arm_id]; // mab.hpp

        ++agt.n[arm_id][t];

        // Q, Xの平均, O(1)
        if (agt.n[arm_id][t] == 1) { // 最初
            agt.Q[arm_id][t] = r;
        } else {
            agt.Q[arm_id][t] = agt.Q[arm_id][t] + 1.0 / agt.n[arm_id][t] * (r - agt.Q[arm_id][t]);
        }
        return arm_id;
    }
};

void iucb(vector<Agent> &agt, vector<vector<pii> > &contact_nodes, vector<MAB> &env, bool isGlobal);

#endif
