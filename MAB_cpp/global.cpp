#include"global.hpp"

int itv = 50;

// Xをoutput
void output(string filename, vector<Agent> &agt) {
    ifstream ifs(filename);
    string line;
    // int cnt = 0;
    vector<pid> v;
    while (getline(ifs, line)) {
        istringstream stream(line);
        int t = -1;
        double d;
        string str;
        while (getline(stream, str, ' ')) {
            if (t == -1) t = stoi(str);
            else d = stod(str);
        }
        v.eb(t, d);
        // if (cnt++ < 5) cerr << t << " " << d << endl;
    }

    ofstream ofs(filename, ios::out);
    for (int t = 0; t < MAX_STEPS; t += itv) {
        double ave = 0;
        rep(machine_id, n_machine) {
            int arm_id = agt[machine_id].a[t];

            assert(arm_id != -1);
            // if (filename == "ucb2_0.txt" && MAX_STEPS <= t + itv) cerr << machine_id << " " << arm_id << " " << agt[machine_id].X[arm_id][t] << endl;
            ave += agt[machine_id].X[arm_id][t];
            // ave += agt[machine_id].Q[arm_id][t];
        }
        assert(n_machine);
        if (round_id == 0) ofs << t << " " << ave / n_machine / N_ROUND << endl;
        else ofs << t << " " << v[t / itv].S + ave / n_machine / N_ROUND << endl;
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
