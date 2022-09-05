import numpy as np
import mab
import bandit_algorithm
import main

class Agent(bandit_algorithm.BanditAlgorithm): # 強化学習のエージェント
    def __init__(self, env, c, init_val):
        # The parameter for the environment
        self.env = env # env->main
        self.Q = np.zeros(main.n_arm, dtype=np.float) # Expected values for each arm
        self.N = np.zeros(main.n_arm) # そのレバーが引かれた回数

        # Algorithm parameters
        self.c = c # the epsilon value
        self.init_val = init_val # Initialization of the inital expected values
        if (init_val != 0):
            for i in range (0, len(self.Q)):
                self.Q[i] = init_val

    def action(self, p):
        memo = -10000000.0
        arm_id = -1
        try:
            for i in range(0, main.n_arm):
                temp = 10000000.0
                if self.N[i] != 0:
                    temp = self.Q[i] + self.c * np.sqrt(np.log(p.t) / self.N[i])
                    if temp > memo:
                      arm_id = i
                      memo = temp
                else: # 引いたことがないレバー
                    arm_id = i
                    break
            assert arm_id != -1, 'arm_id is -1'
        except AssertionError as err:
            print('AssertionError :', err)

        # Geting reward and incremental update
        r = self.env.bandit(arm_id, p)
        self.N[arm_id] += 1 # そのレバーを引いた回数を+1
        # そのレバーの価値Qを求める
        if self.N[arm_id] == 1: # 初期値
            self.Q[arm_id] = r
        else:
            self.Q[arm_id] = self.Q[arm_id] + (r - self.Q[arm_id]) / self.N[arm_id] # Incremental update
        return arm_id

    def get_name(self):
        return "ucb_" + str(self.c) + "_" + str(self.init_val)
