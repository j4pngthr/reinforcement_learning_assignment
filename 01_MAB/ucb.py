import numpy as np
import mab
import bandit_algorithm

class Agent(bandit_algorithm.BanditAlgorithm): # 強化学習のエージェント
    def __init__(self, env, epsilon, init_val):
        # The parameter for the environment
        self.env = env # env->main
        self.Q = np.zeros(env.k, dtype=np.float) # Expected values for each arm
        self.N = np.zeros(env.k) # そのレバーが引かれた回数

        # Algorithm parameters
        self.epsilon = epsilon # the epsilon value
        self.init_val = init_val # Initialization of the inital expected values
        if (init_val != 0):
            for i in range (0, len(self.Q)):
                self.Q[i] = init_val

    def action(self, t, c):
        memo = 0
        for i in range(0, self.env.k):
          temp = 10000000
          if self.N[i] != 0:
            temp = self.Q[i] + c * np.sqrt(np.log(t) / self.N[i])
          if temp > memo:
            arm_id = i
            memo = temp

        # Geting reward and incremental update
        r = self.env.bandit(arm_id)
        self.N[arm_id] += 1 # そのレバーを引いた回数を+1
        # そのレバーの価値Qを求める
        if self.N[arm_id] == 1: # 初期値
            self.Q[arm_id] = r
        else:
            self.Q[arm_id] = self.Q[arm_id] + (r - self.Q[arm_id]) / self.N[arm_id] # Incremental update

    def get_name(self):
        return "ucb_" + str(self.epsilon) + "_" + str(self.init_val)
