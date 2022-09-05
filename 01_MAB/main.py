import egreedy
import mab
import ucb
import myio
import numpy as np
import sys
import random

n_arm = 10 # 1個のマシンの腕の数
n_machine = 2000 # 2000
MAX_STEPS = 1000 # 1000
init_val = 0

class params(): # parameters
    # average_machine_rewards
    avg_mac_rs = np.zeros(MAX_STEPS, dtype=float)
    # optimal
    opt_arm_id = np.zeros(n_machine, dtype=int)
    t = 0
    # 最後のマシンで選んだレバー
    chosen_arm_id = np.zeros(MAX_STEPS, dtype=int)

    def make_params(self):
        # わかりにくいけどn_machine * n_arm
        # 各箱には最後にはn_arm個の要素
        for i in range(0, n_arm):
            # n_machine個の箱に1要素ずつ加える
            _mu = np.random.normal(0, 1, n_machine).reshape((-1, 1)) # [[x_1], [x_2], ...]

            if i == 0:
                mu = _mu
            else:
                # 2周後
                # [[x_11, x_12], [x_21, x_22], ...]
                mu = np.hstack((mu, _mu))

        # 最大の平均値をもつ最適なレバーを求める
        for i in range(0, n_machine):
            # print(i, mu[i])
            temp = -100
            for j in range(0, n_arm):
                if temp < mu[i][j]:
                    temp = mu[i][j]
                    self.opt_arm_id[i] = j
        # print(self.opt_arm_id)

        sig = [[1] * n_arm] * n_machine
        return mu, sig

# mab
if __name__ == "__main__":
    key = 0
    if key == 0:
        p = params()
        mu, sig = p.make_params()

        # epsilon greedy
        for epsilon in [0.1]: # [0, 0.01, 0.1]:
            # rate 最適なレバーを選んだ割合
            # rat_opt_arm = np.zeros(MAX_STEPS, dtype=float)

            for i in range(0, MAX_STEPS):
                # machine rewards
                p.avg_mac_rs[i] = 0

            # 各マシンでシミュレーションをする
            for machine_id in range(0, n_machine):
                # シミュレーションの進捗の可視化
                if machine_id % 100 == 0:
                    print(epsilon, "machine_id", machine_id)

                # 各マシンのレバーを決める
                env = mab.MAB(mu[machine_id], sig[machine_id]) # mab.py
                a = egreedy.Agent(env, epsilon, init_val) # egreedy.py

                for _t in range(0, MAX_STEPS):
                    p.t = _t
                    arm_id = a.action(p) # egreedy.py/Agent.action
                    p.chosen_arm_id[_t] = arm_id
                    # pのavg_mac_rsの書き換え a.action->env.bandit
                    # rate
                    # if arm_id == p.opt_arm_id[machine_id]:
                    #     rat_opt_arm[_t] += 1.0 / n_machine

            # Output
            # epsilon = 0, 0.01, 0.1
            temp = str(epsilon)
            file_name = ""
            for _c in temp:
                if _c == '.':
                    file_name = file_name + '_'
                else:
                    file_name = file_name + _c
            file_name = "eps" + file_name
            # a.get_name() -> egreedy.py/Agent
            myio.write_data(a.get_name(), 0, p.avg_mac_rs, "data/" + file_name + ".txt")
            myio.write_data(a.get_name(), 0, p.avg_mac_rs, "data/" + file_name + "_arm" + ".txt")

            # get cumulative rewards
            # cum_avg_mac_rs = np.zeros(MAX_STEPS, dtype=float)
            # for i in range(1, MAX_STEPS):
            #     cum_avg_mac_rs[i] = cum_avg_mac_rs[i - 1] + p.avg_mac_rs[i]

            # myio.write_data(a.get_name(), 0, cum_avg_mac_rs, "data/cum_" + file_name + ".txt")

            # rate
            # myio.write_data(a.get_name(), 0, rat_opt_arm, "data/rat_" + file_name + ".txt")

        # ucb
        for c in [2]:
            for i in range(0, MAX_STEPS):
                # machine rewards
                p.avg_mac_rs[i] = 0

            for machine_id in range(0, n_machine):
                # シミュレーションの進捗の可視化
                if machine_id % 100 == 0:
                    print(epsilon, "machine_id", machine_id)

                env = mab.MAB(mu[machine_id], sig[machine_id])
                a = ucb.Agent(env, c, init_val);

                for _t in range(0, MAX_STEPS):
                    p.t = _t
                    arm_id = a.action(p) # ucb.py/Agent.action
                    p.chosen_arm_id[_t] = arm_id

            # Output the average machine rewards to a file
            temp = str(c)
            file_name = ""
            for _c in temp:
                if _c == '.':
                    file_name = file_name + '_'
                else:
                    file_name = file_name + _c
            file_name = "ucb" + file_name
            # a.get_name() -> egreedy.py/Agent
            myio.write_data(a.get_name(), 0, p.avg_mac_rs, "data/" + file_name + ".txt")
            myio.write_data(a.get_name(), 0, p.avg_mac_rs, "data/" + file_name + "_arm" + ".txt")
