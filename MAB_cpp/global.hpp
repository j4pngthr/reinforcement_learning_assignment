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
template<typename T, typename U> T pow_(T a, U b) { return b ? pow_(a * a, b / 2) * (b % 2 ? a : 1) : 1; }
template<class T, class U> ostream& operator << (ostream& os, const pair<T, U>& p) { os << p.F << " " << p.S; return os; }
template<class T, class U> void operator += (pair<T, U>& p, const pair<T, U>& q) { p.F += q.F; p.S += q.S; }
template<class T> ostream& operator << (ostream& os, const vector<T>& vec) { rep(i, sz(vec)) { if (i) os << " "; os << vec[i]; } return os; }
template<typename T> inline istream& operator >> (istream& is, vector<T>& v) { rep(j, sz(v)) is >> v[j]; return is; }
template<class T> void operator += (vector<T>& v, vector<T>& v2) { rep(i, sz(v2)) v.eb(v2[i]); }
template<class T> ostream& operator << (ostream& os, const set<T>& s) { for (auto si : s) os << si << " "; return os; }

using pii = pair<int, int>;
using pdi = pair<double, int>;
using pid = pair<int, double>;
using pdd = pair<double, double>;
using pip = pair<int, pii>;

const int inf = numeric_limits<int>::max();
const double dinf = numeric_limits<double>::infinity();

extern int n_arm, n_machine, MAX_STEPS;
extern double init_val;

class params {
public:
    vector<double> avg_mac_rs;
    vector<double> mu;
    double sig = 1;
    int t;
    vector<vector<double> > X;

    params() {
        avg_mac_rs.resize(MAX_STEPS);
        mu.resize(n_arm);
        {
            X.resize(n_machine);
            rep(machine_id, n_machine) X[machine_id].resize(MAX_STEPS);
        }

        rep(i, n_arm) {
            random_device seed_gen;
            default_random_engine engine(seed_gen());
            normal_distribution<> dist(0, 1);
            mu[i] = dist(engine);
        }
    }
};

#endif
