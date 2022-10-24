#include"global.hpp"
#include"mab.hpp"
#include"egreedy.hpp"
#include"ucb.hpp"
// #include"bandit_algorithm.hpp"
#include"getRealTrace.hpp"

int n_arm = 10, n_machine = 78, MAX_STEPS = 1000;
double init_val = 0; // 78, 1000
string filename = "egreedy.txt";

int main() {
    params p;
    sort(all(p.mu));
    rep(i, n_arm) cerr << p.mu[i] << " "; cerr << endl;

    for (double epsilon : {0.1}) {
        // rep(i, MAX_STEPS) { // epsilonが複数なら初期化が必要
        //     p.avg_mac_rs[i] = 0;
        // }

        rep(machine_id, n_machine) {
            MAB env(p.mu, p.sig);
            Egreedy a(env, epsilon, init_val);

            rep(_t, MAX_STEPS) {
                p.t = _t;
                int arm_id = a.action(machine_id, p);
                // cerr << _t << " " << p.t << " " << p.avg_mac_rs[p.t] << endl;

                // if (machine_id == 0 && (_t < 50 || _t > 950)) cerr << _t << " " << arm_id << endl;
            }
        }
        // rep(_t, 10) cerr << p.avg_mac_rs[_t] << endl;

        // ofstream ofs(filename, ios::out);
        // rep(_t, MAX_STEPS) ofs << _t << " " << p.avg_mac_rs[_t] << endl;
        ofstream ofs(filename, ios::out);
        rep(_t, MAX_STEPS) {
            double ave = 0;
            rep(i, n_machine) {
                ave += p.X[i][_t];
            }
            ofs << _t << " " << ave / n_machine << endl;
        }
    }

    // filename = "ucb.txt";
    //
    // for (double c : {2}) {
    //     rep(i, MAX_STEPS) {
    //         p.avg_mac_rs[i] = 0;
    //     }
    //
    //     rep(machine_id, n_machine) {
    //         MAB env(p.mu, p.sig);
    //         Ucb a(env, c, init_val);
    //
    //         rep(_t, MAX_STEPS) {
    //             p.t = _t;
    //             int arm_id = a.action(p);
    //         }
    //     }
    //
    //     ofstream ofs(filename, ios::out);
    //     rep(_t, MAX_STEPS) ofs << _t << " " << p.avg_mac_rs[_t] << endl;
    // }

    filename = "ucb.txt";
    vector<vector<pii> > contact_nodes;
    getRealTrace(contact_nodes);
    // cerr << "got realtrace" << endl;

    for (double c : {2}) {
        // rep(i, MAX_STEPS) {
        //     p.avg_mac_rs[i] = 0;
        // }

        rep(machine_id, n_machine) {
            // cerr << "machine_id " << machine_id << endl;
            MAB env(p.mu, p.sig);
            Ucb a(env, c, init_val);

            rep(_t, MAX_STEPS) {
                // cerr << "_t " << _t << endl;
                p.t = _t;
                int arm_id = a.action(machine_id, p);
            }
        }

        ofstream ofs(filename, ios::out);
        rep(_t, MAX_STEPS) {
            double ave = 0;
            rep(i, n_machine) {
                ave += p.X[i][_t];
            }
            ofs << _t << " " << ave / n_machine << endl;
        }
    }
}
