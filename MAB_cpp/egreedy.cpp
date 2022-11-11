// #include"global.hpp"
// #include"egreedy.hpp"
//
// void egreedy(vector<Lever> &lv) {
//     for (double epsilon : {0.1}) {
//         // rep(i, MAX_STEPS) { // epsilonが複数なら初期化が必要
//         //     p.avg_mac_rs[i] = 0;
//         // }
//         vector<Agent> agt(n_machine);
//
//         rep(machine_id, n_machine) {
//             vector<double> _mu, _sig;
//             rep(k, n_arm) {
//                 _mu.eb(lv[k].getMu());
//                 _sig.eb(1);
//             }
//             MAB env(_mu, _sig);
//             Egreedy a(env, epsilon, init_val);
//
//             rep(t, MAX_STEPS) {
//                 int arm_id = a.action(machine_id, t, agt[machine_id]);
//                 agt[machine_id].a[t] = arm_id;
//             }
//         }
//
//         output("egreedy.txt", agt);
//     }
// }
