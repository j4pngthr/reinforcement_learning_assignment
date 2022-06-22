import numpy as np
import main

class MAB(): # 強化学習の環境
    def __init__(self, k, mu, sig): # どのレバーも同じ？
        self.k = k # The number of arms
        self.mu = mu # Mean
        self.sig = sig # σ 正規分布の標準偏差

    def bandit(self, arm_id, p): # 正規分布から報酬を得る # egreedy.py/action
        assert(arm_id >= 0 and arm_id < self.k)

        r = np.random.normal(self.mu[arm_id], self.sig[arm_id], 1) # 正規分布 平均, 標準偏差, サイズ?
        p.avg_mac_rs[p.t] += r / main.n_machine
        return r

    def print_rewards():
        for i in range(0, len(self.rewards)):
            print("step ", i, " : ", self.rewards[i])
