#include"global.hpp"
#include"ucb.hpp"

// global knowledge, バイアスなし
// 使うもの agt.a, X
void ucb(vector<Agent> &agt, vector<MAB> &env) {
    for (double c : {2}) {
        rep(i, n_machine) agt[i].init(); // agt.a, X

        vector<Ucb> a(n_machine);
        // aの初期化
        rep(i, n_machine) {
            a[i].init(c, env[i]);
        }

        rep(t, MAX_STEPS) {
            if (t % 1000 == 0) cerr << "t " << t << endl;

            rep(i, n_machine) {
                int arm_id = a[i].action(i, t, agt[i]); // ucb.hpp
                agt[i].a[t] = arm_id;
            }
        }

        output("ucb.txt", agt); // global.cpp/agt.X
    }
}

// 超指数分布の係数
double calcA(vector<Agent> &agt, int k, const vector<int>& path) {
  double A = 1;
  rep(i, sz(path) - 1) {
    if (i == k) continue;
    int a = path[i], b = path[i + 1];
    A *= agt[a].lambda[b] / (agt[a].lambda[b] + agt[path[k]].lambda[path[k + 1]]);
  }
  return A;
}

vector<int> getPath(vector<Agent> &agt, int s, int t) {
    vector<int> path;
    for (int cur = t; cur != -1; cur = agt[s].parent[cur]) {
        // cerr << cur << endl;
        path.eb(cur);
    }
    reverse(all(path)); // 逆順なのでひっくり返す
    return path;
}

// クエリの到達率
void calcP(vector<Agent> &agt, vector<vector<double> > &p) {
    rep(i, n_machine) rep3(j, i + 1, n_machine) { // rep(j, n)だとまずそう
        vector<int> path = getPath(agt, i, j);
        // cerr << i << " " << j << " " << path << endl;
        rep(k, sz(path) - 1) {
          p[i][j] += calcA(agt, k, path) * (1 - exp(-agt[path[k]].lambda[path[k + 1]] * MAX_STEPS));
          p[j][i] = p[i][j];
        }
    }
}

void calcWeightedConnectivity(vector<Agent> &agt, vector<double> &C_wc) {
    vector<vector<double> > p(n_machine, vector<double>(n_machine));
    calcP(agt, p);
    // cerr << "calced p" << endl;

    double ma = 0;
    rep(i, n_machine) {
        rep3(j, i + 1, n_machine) chmax(ma, p[i][j]);
    }

    rep(i, n_machine) {
        rep(j, n_machine) p[i][j] /= ma;
    }

    rep(i, n_machine) {
        rep(j, n_machine) {
            if (i == j) continue;
            assert(agt[i].d[j] > eps);
            assert(agt[i].hop[j]);
            C_wc[i] += p[i][j] / agt[i].d[j] / agt[i].hop[j];
        }
    }
}

// 情報交換あり or なし, バイアスあり
// 使うの agt.n, Q, X
// t←t-1, n, Q
void ucb2(vector<Agent> &agt, vector<vector<pii> > &contact_nodes, vector<MAB> &env, int excInfo, int weighted) {
    vector<double> C_wc(n_machine); // 中心性
    if (weighted) {
        calcWeightedConnectivity(agt, C_wc);
    }

    for (double c : {2}) {
        rep(machine_id, n_machine) agt[machine_id].init();

        vector<Ucb2> a(n_machine);
        rep(machine_id, n_machine) {
            a[machine_id].init(c, env[machine_id]);
        }

        { // t = 0
            rep(i, n_machine) {
                rep(k, n_arm) {
                    if (!agt[i].pullable[k]) continue;

                    double X = env[i].bandit(k, i, 0, agt[i]);
                    agt[i].Q[k][0] = X;
                    ++agt[i].n[k][0];
                }
            }
        }

        rep3(t, 1, MAX_STEPS) {
            if (t % 1000 == 0) cerr << "t " << t << endl;

            // tに関して更新されていない値を更新
            rep(i, n_machine) {
                rep(k, n_arm) {
                    agt[i].n[k][t] = agt[i].n[k][t - 1];
                    agt[i].Q[k][t] = agt[i].Q[k][t - 1];
                }
            }

            // 各エージェントがレバーを引く
            rep(machine_id, n_machine) {
                int arm_id = a[machine_id].action(machine_id, t, agt[machine_id]); // Xを求める
                agt[machine_id].a[t] = arm_id;
            }

            if (!excInfo) continue;
            // 情報交換
            for (pii p : contact_nodes[t]) {
                int i = p.F, j = p.S;
                assert(i >= 0 && i < n_machine && j >= 0 && j < n_machine);

                double alpha = 0.5;
                if (weighted) {
                    assert(C_wc[i] + C_wc[j] > eps);
                    alpha = C_wc[i] / (C_wc[i] + C_wc[j]);
                }

                rep(k, n_arm) {
                    if (!agt[i].pullable[k] && !agt[j].pullable[k]) continue;
                    else if (agt[i].pullable[k] && agt[j].pullable[k]) {
                        rep(_, 2) {
                            agt[i].Q[k][t] = alpha * agt[i].Q[k][t - 1] + (1 - alpha) * agt[j].Q[k][t - 1];
                            swap(i, j);
                        }
                    } else if (agt[i].pullable[k]) {
                        agt[j].pullable[k] = 1;
                    } else if (agt[j].pullable[k]) {
                        agt[i].pullable[k] = 1;
                    }
                }
            }
        }

        string filename = "ucb2_1.txt";
        if (!excInfo) filename = "ucb2_0.txt";
        if (weighted) filename = "ucb3.txt";
        output(filename, agt);
    }
}
