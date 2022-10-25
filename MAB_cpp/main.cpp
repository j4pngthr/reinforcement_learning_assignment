#include"global.hpp"
#include"mab.hpp"
#include"egreedy.hpp"
#include"ucb.hpp"
// #include"bandit_algorithm.hpp"
#include"getRealTrace.hpp"
#include"proposed.hpp"

int n_arm = 10, n_machine = 78, MAX_STEPS = 1000;
double init_val = 0; // 78, 1000
string filename = "egreedy.txt";

int main() {
    vector<Lever> lv(n_arm);
    rep(k, n_arm) cerr << lv[k].getMu() << " "; cerr << endl;

    // egreedy(lv);
    ucb(lv);

    vector<vector<pii> > contact_nodes;
    getRealTrace(contact_nodes);
    proposed(lv, contact_nodes);
}
