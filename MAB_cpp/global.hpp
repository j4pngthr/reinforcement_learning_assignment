#ifndef global_hpp
#define global_hpp

#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

#define F first
#define S second
#define eb emplace_back
#define all(v) v.begin(), v.end()
#define rep(i, n) for (int i = 0; i < (n); ++i)
#define rep3(i, l, n) for (int i = l; i < (n); ++i)
#define sz(v) (int)v.size()
#define endl '\n'
#define abs(x) (x >= 0 ? x : -(x))
#define lb(v, x) (int)(lower_bound(all(v), x) - v.begin())
#define ub(v, x) (int)(upper_bound(all(v), x) - v.begin())
#define eps 1e-8

#define BETWEENNESS 14
#define CLOSENESS 211
#define CONNECTIVITY 214
#define DEGREE 34
#define RAFR 1705
#define RANDOM 17013

template<typename T1, typename T2> inline bool chmin(T1 &a, T2 b) { if (a > b) { a = b; return 1; } return 0; }
template<typename T1, typename T2> inline bool chmax(T1 &a, T2 b) { if (a < b) { a = b; return 1; } return 0; }
template<typename MAX_STEPS, typename U> MAX_STEPS pow_(MAX_STEPS a, U b) { return b ? pow_(a * a, b / 2) * (b % 2 ? a : 1) : 1; }
template<class MAX_STEPS, class U> ostream& operator << (ostream& os, const pair<MAX_STEPS, U>& p) { os << p.F << " " << p.S; return os; }
template<class MAX_STEPS, class U> void operator += (pair<MAX_STEPS, U>& p, const pair<MAX_STEPS, U>& q) { p.F += q.F; p.S += q.S; }
template<class MAX_STEPS> ostream& operator << (ostream& os, const vector<MAX_STEPS>& vec) { rep(i, sz(vec)) { if (i) os << " "; os << vec[i]; } return os; }
template<typename MAX_STEPS> inline istream& operator >> (istream& is, vector<MAX_STEPS>& v) { rep(j, sz(v)) is >> v[j]; return is; }
template<class MAX_STEPS> void operator += (vector<MAX_STEPS>& v, vector<MAX_STEPS>& v2) { rep(i, sz(v2)) v.eb(v2[i]); }
template<class MAX_STEPS> ostream& operator << (ostream& os, const set<MAX_STEPS>& s) { for (auto si : s) os << si << " "; return os; }

using pii = pair<int, int>;
using pdi = pair<double, int>;
using pid = pair<int, double>;
using pdd = pair<double, double>;
using pip = pair<int, pii>;

const int inf = numeric_limits<int>::max();
const double dinf = numeric_limits<double>::infinity();

extern int n_arm, n_machine, MAX_STEPS;
extern double init_val;


// class params {
// public:
//     vector<double> avg_mac_rs;
//     vector<double> mu;
//     double sig = 1;
//     int MAX_STEPS;
//     vector<vector<double> > X, X_tilde, theta;
//
//     params() {
//         avg_mac_rs.resize(MAX_STEPS);
//         mu.resize(n_arm);
//         {
//             X.resize(n_machine);
//             rep(machine_id, n_machine) X[machine_id].resize(MAX_STEPS);
//         }
//         {
//             X_tilde.resize(n_machine);
//             rep(machine_id, n_machine) X_tilde[machine_id].resize(MAX_STEPS);
//         }
//         {
//             theta.resize(n_machine);
//             rep(machine_id, n_machine) theta[machine_id].resize(MAX_STEPS);
//         }
//
//         rep(i, n_arm) {
//             random_device seed_gen;
//             default_random_engine engine(seed_gen());
//             normal_distribution<> dist(0, 1);
//             mu[i] = dist(engine);
//         }
//     }
// };

class Lever {
    double mu;
    const double sig = 1;
    random_device seed_gen;
public:
    // double mu;
    double getMu() { return mu; }
    double getSig() { return sig; }

    Lever() {
        default_random_engine engine(seed_gen());
        normal_distribution<> dist(0, 1);
        mu = dist(engine);
    }
    double play() {
        default_random_engine engine(seed_gen());
        normal_distribution<> dist(mu, sig);
        double r = dist(engine);
        return r;
    }
    // bool operator<(const Lever& other) const {
    //     return mu < other.mu;
    // };
};

class Agent {
public:
    vector<vector<double> > X, X_tilde, theta, n, Q, n_tilde;
    vector<int> a, pullable;
    vector<double> bias;
    set<int> e; // 隣接ノードの集合
    int limited = 0;
    vector<Agent> ngb;

    // すべての手法で共通
    Agent() {
        pullable.resize(n_arm);
        // 引けるレバーの数が限られているか
        if (limited) {
            int cnt = 0;
            while (cnt < 3) {
                int k = rand() % n_arm;
                if (pullable[k]) continue;
                pullable[k] = 1;
                ++cnt;
            }
        } else {
            rep(k, n_arm) {
                pullable[k] = 1;
            }
        }

        { // エージェントごとに各レバーにバイアスを作る
            random_device seed_gen;
            default_random_engine engine(seed_gen());
            normal_distribution<> dist(0, 1);
            bias.resize(n_arm);
            rep(i, n_arm) bias[i] = dist(engine);
            // rep(i, n_arm) cerr << bias[i] << " "; cerr << endl;
        }
    }

    void init() {
        assert(MAX_STEPS > 0);

        X = vector<vector<double> >(n_arm, vector<double>(MAX_STEPS + 5, 0));
        X_tilde = vector<vector<double> >(n_arm, vector<double>(MAX_STEPS + 5, 0));
        theta = vector<vector<double> >(n_arm, vector<double>(MAX_STEPS + 5, 0));
        n = vector<vector<double> >(n_arm, vector<double>(MAX_STEPS + 5, 0));
        n_tilde = vector<vector<double> >(n_arm, vector<double>(MAX_STEPS + 5, 0));
        Q = vector<vector<double> >(n_arm, vector<double>(MAX_STEPS + 5, 0));

        a = vector<int>(MAX_STEPS + 5, 0);
    }
};

void output(string filename, vector<Agent> &agt);
void calcXTilde(int i, int k, int t, Agent &agt);

#endif
