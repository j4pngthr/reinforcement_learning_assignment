#include"global.hpp"

void output(string filename, vector<Agent> &agt) {
    ofstream ofs(filename, ios::out);
    rep(t, MAX_STEPS) {
        double ave = 0;
        rep(machine_id, n_machine) {
            int arm_id = agt[machine_id].a[t];
            ave += agt[machine_id].X[arm_id][t];
        }
        ofs << t << " " << ave / n_machine << endl;
    }
}
