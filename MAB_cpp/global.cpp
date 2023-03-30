#include"global.hpp"

int itv = 50;

void changeTimeInterval(string filename) {
    vector<pid> v;
    {
        ifstream ifs(filename);
        string line;
        while (getline(ifs, line)) { // round_id==0 -> 入らない
            istringstream stream(line);
            int t = -1;
            double d;
            string str;
            while (getline(stream, str, ' ')) {
                if (t == -1) t = stoi(str);
                else d = stod(str);
            }
            // cerr << t << " " << d << endl;
            v.eb(t, d);
        }
    }

    ofstream ofs(filename, ios::out);
    for (int t = 0; t < MAX_STEPS; t += itv) {
        ofs << v[t].F << " " << v[t].S << endl;
    }
}

string outputRegret(string filename, vector<Agent> &agt, vector<Lever> &lv) {
    filename += "_regret.txt";
    cerr << filename << endl;

    // 下でv[].S+に使う
    vector<pid> v;
    if (round_id != 0) { // バグった値が入ってるかもしれない 例:e-322
        ifstream ifs(filename);
        string line;
        while (getline(ifs, line)) { // round_id==0 -> 入らない
            istringstream stream(line);
            int t = -1;
            double d;
            string str;
            while (getline(stream, str, ' ')) {
                if (t == -1) t = stoi(str);
                else d = stod(str);
            }
            // cerr << t << " " << d << endl;
            v.eb(t, d);
        }
    }
    // cerr << "before ofs" << endl;

    ofstream ofs(filename, ios::out);
    vector<double> cum_reg({0});
    rep(t, MAX_STEPS) { // need t=0 for output
        double cur = 0;
        if (t != 0) {
            double ave = 0;
            rep(machine_id, n_machine) {
                int arm_id = agt[machine_id].a[t];
                assert(arm_id != -1);
                ave += lv[opt_lv_id].getMu() - lv[arm_id].getMu();
                // if (machine_id == 0) cerr << "lv " << lv[opt_lv_id].getMu() << " " << lv[arm_id].getMu() << endl;
                // if (t == 1620) cerr << "lv " << arm_id << " " << opt_lv_id << endl;
            }
            ave /= n_machine;
            cur = cum_reg.back() + ave;
            cum_reg.eb(cur);
            // if (t > 1600) cerr << t << " " << cum_reg.back() << " " << ave << endl;
        }
        if (round_id == 0) {
            ofs << t << " " << cur / N_ROUND << endl;
            // if (t % 100 == 0) cerr << t << " " << cur << endl;
            // if (t < 100) cerr << t << " " << ave / N_ROUND << endl;
        } else {
            ofs << t << " " << v[t].S + cur / N_ROUND << endl;
            // if (t < 100) cerr << t << " " << v[t / itv].S << " " << ave / N_ROUND << endl;
        }
    }

    return filename;
}

// Xをoutput
// round_idごとに
string outputReward(string filename, vector<Agent> &agt) {
    filename += "_reward.txt";
    // cerr << filename << " " << round_id << endl;
    // 下でv[].S+に使う
    vector<pid> v;
    if (round_id != 0) {
        ifstream ifs(filename);
        string line;
        while (getline(ifs, line)) { // round_id==0 -> 入らない
            istringstream stream(line);
            int t = -1;
            double d;
            string str;
            while (getline(stream, str, ' ')) {
                if (t == -1) t = stoi(str);
                else d = stod(str);
            }
            v.eb(t, d);
            // if (round_id == 1) cerr << t << " " << d << endl;
            // if (round_id == 1 && t > MAX_STEPS - 50) cerr << t << " " << d << endl;
        }
    }

    ofstream ofs(filename, ios::out);
    rep(t, MAX_STEPS) {
        double ave = 0;
        if (t != 0) {
            rep(machine_id, n_machine) {
                int arm_id = agt[machine_id].a[t];
                assert(arm_id != -1);

                // if (filename == "ucb2_0.txt" && MAX_STEPS <= t + itv) cerr << machine_id << " " << arm_id << " " << agt[machine_id].X[arm_id][t] << endl;
                ave += agt[machine_id].X[arm_id][t];
                // ave += agt[machine_id].Q[arm_id][t];
            }
            ave /= n_machine;
        }
        // if (round_id == 1 && t > 1900) cerr << v[t].S << " " << ave << endl;
        if (round_id == 0) ofs << t << " " << ave / N_ROUND << endl;
        else ofs << t << " " << v[t].S + ave / N_ROUND << endl;
    }

    return filename;
}

// X->X_tilde, gossip.cpp
void calcXTilde(int i, int k, int t, Agent &agt) {
    // {
    //     double sum = 0; // sum = agt.X[k][0]じゃないと下がりそう
    //     int cnt = 1; // t=0
    //     rep3(tau, 1, t + 1) {
    //         if (agt.a[tau] != k) continue;
    //         sum += agt.X[k][tau];
    //         ++cnt;
    //     }
    //     assert(cnt == agt.n[k][t]);
    //     agt.X_tilde[k][t] = sum / cnt;
    // }

    {
        double sum = 0;
        int cnt = 1;
        rep3(tau, 1, t + 1) {
            if (agt.a[tau] != k) continue;
            sum += agt.X[k][tau];
            ++cnt;
        }
        agt.X_tilde[k][t] = sum / cnt;
    }

    // {
    //     // 前に引いたところを探す
    //     for (int tau = t - 1; tau >= 0; --tau) {
    //         // 前にtauで引いた
    //         if (agt.a[tau] == k) {
    //             // O(1)で平均値の計算, SuttonBartolPRLBook2ndEd-2章.pdf
    //             double temp = agt.X_tilde[k][tau] + 1.0 / agt.n[k][t] * (agt.X[k][t] - agt.X_tilde[k][tau]);
    //
    //             // // 計算量改善前のをコメントアウトしてない場合
    //             // if (abs(temp - agt.X_tilde[k][t]) > eps) {
    //             //     cerr << temp << " " << agt.X_tilde[k][t] << endl;
    //             //     assert(0);
    //             // }
    //
    //             agt.X_tilde[k][t] = temp;
    //             break;
    //         }
    //     }
    // }
}

// O(CNT_CONTACT * M)
void excInfo(int t, vector<Agent> &agt, vector<vector<pii> > &contact_nodes, int exc_Q, int weighted, vector<double> C_wc, int need_ngb, int update_unused) {
    double alpha = 0.5;
    vector<int> used(n_machine);

    for (pii p : contact_nodes[t]) {
        int i = p.F, j = p.S;
        assert(i >= 0 && i < n_machine && j >= 0 && j < n_machine);
        if (i > j) swap(i, j);
        // if (t < 200) cerr << t << "    " << i << " " << j << endl;
        used[i] = 1; used[j] = 1;

        // if (i == 0) {
        //     cerr << "t j    pullable " << t << " " << j << "    ";
        //     rep(k, n_arm) {
        //         if (agt[j].pullable[k]) {
        //             cerr << k << " ";
        //         }
        //     }
        //     cerr << endl;
        // }

        if (weighted) {
            assert(C_wc[i] + C_wc[j] > eps);
            alpha = C_wc[i] / (C_wc[i] + C_wc[j]);
        }

        // if (need_ngb) {
        //     rep(_, 2) {
        //         agt[i].ngb[j] = agt[j]; // O(MT)
        //         swap(i, j);
        //     }
        // }

        agt[i].met_last[j] = t;
        agt[j].met_last[i] = t;

        rep(k, n_arm) {
            if (!agt[i].pullable[k] && !agt[j].pullable[k]) continue;

            // 片方だけが知ってるレバー
            if (agt[i].pullable[k] && !agt[j].pullable[k]) {
                agt[j].pullable[k] = 1;
                continue;
            } else if (!agt[i].pullable[k] && agt[j].pullable[k]) {
                agt[i].pullable[k] = 1;
                continue;
            }

            if (exc_Q) {
                if (agt[i].pullable[k] && agt[j].pullable[k]) {
                    rep(_, 2) {
                        // 接触時だからjのt-1の情報を使える
                        if (!update_unused) agt[i].Q[k][t] = alpha * agt[i].Q[k][t - 1] + (1 - alpha) * agt[j].Q[k][t - 1];
                        else agt[i].theta[k][t] = alpha * agt[i].theta[k][t - 1] + (1 - alpha) * agt[j].theta[k][t - 1] + (agt[i].X_tilde[k][t] - agt[i].X_tilde[k][t - 1]);
                        swap(i, j);
                    }
                }
            }
        }
    }

    if (update_unused) { 
        // if (t < 20) {
        //     vector<int> v;
        //     rep(i, n_machine) if (!used[i]) v.eb(i);
        //     cerr << "unused " << v << endl;
        // }
        rep(i, n_machine) {
            if (used[i]) continue;

            rep(k, n_arm) {
                agt[i].theta[k][t] = agt[i].theta[k][t - 1] + (agt[i].X_tilde[k][t] - agt[i].X_tilde[k][t - 1]);
                // if (i == 0 && t < 20) cerr << agt[i].theta[k][t - 1] << " " << agt[i].X_tilde[k][t] << " " << agt[i].X_tilde[k][t - 1] << endl;
            }
        }
    }
}
