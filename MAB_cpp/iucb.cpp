#include"global.hpp"
#include"iucb.hpp"

double alpha = 0.5;

void iucb(vector<Agent> &agt, vector<vector<pii> > &contact_nodes, vector<MAB> &env, bool isGlobal) {
    for (double c : {2}) {
        rep(i, n_machine) agt[i].init();

        vector<Iucb> a(n_machine);
        rep(i, n_machine) {
            a[i].init(c, env[i]);
        }

        { // t = 0
            rep(i, n_machine) {
                rep(k, n_arm) {
                    if (!agt[i].pullable[k]) continue;

                    double X = env[i].bandit(k, i, 0, agt[i]);
                    agt[i].Q[k][0] = X;
                    ++agt[i].n[k][0];
                }
                // agt[i].a[0]は指定できない
            }
        }

        rep3(t, 1, MAX_STEPS) { // O(T * MN^2s), T=17*10^4
            if (t % 5000 == 0) cerr << "t " << t << endl;

            // tに関して更新されていない値を更新
            rep(i, n_machine) { // O(NM)
                rep(k, n_arm) {
                    agt[i].n[k][t] = agt[i].n[k][t - 1];
                    agt[i].Q[k][t] = agt[i].Q[k][t - 1];
                }
            }

            // 各エージェントがレバーを引く
            rep(i, n_machine) { // O(N * MNs), s=100, 6*10^6
                int arm_id = a[i].action(i, t, agt[i], agt, alpha, isGlobal); // Xを求める
                agt[i].a[t] = arm_id;
            }
            // if (t == 500) cerr << "arm_id ";
            // if (t >= 500 && t < 511) cerr << agt[0].a[t] << " ";
            // if (t == 510) cerr << endl;

            // 情報交換
            // O(CNT_CONTACT * M)
            excInfo(t, agt, contact_nodes, 0, 0, vector<double>(), 1, 0); // exc_Q, weighted, need_ngb, update_unused

            // if (t == 510 || t == 511) {
            //     cerr << "Q ";
            //     rep(k, n_arm) cerr << agt[0].Q[k][t] << " "; cerr << endl;
            // }
        }

        string filename = "iucb.txt";
        if (isGlobal) filename = "iucb_global.txt";
        output(filename, agt);
    }
}
