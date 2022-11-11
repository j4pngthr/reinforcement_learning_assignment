#include"global.hpp"

// Xをoutput
void output(string filename, vector<Agent> &agt) {
    ofstream ofs(filename, ios::out);
    for (int t = 1000; t < MAX_STEPS; t += 10) {
        double ave = 0;
        rep(machine_id, n_machine) {
            int arm_id = agt[machine_id].a[t];

            assert(arm_id != -1);
            ave += agt[machine_id].X[arm_id][t];
        }
        assert(n_machine);
        ofs << t << " " << ave / n_machine << endl;
    }
}

// X->X_tilde
void calcXTilde(int i, int k, int t, Agent &agt) {
    double sum = 0;
    int cnt = 1; // t=0
    rep3(tau, 1, t) {
        if (agt.a[tau] != k) continue;
        sum += agt.X[k][tau];
        ++cnt;
    }
    agt.X_tilde[k][t] = sum / cnt;
}
