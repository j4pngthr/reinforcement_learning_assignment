import numpy as np
import mab
import bandit_algorithm
import random
import main

class Agent(bandit_algorithm.BanditAlgorithm): # 強化学習のエージェント
    def __init__(self, env, epsilon, init_val):
        # The parameter for the environment
        self.env = env # env->main
        self.Q = np.zeros(main.n_arm, dtype=np.float) # Expected values for each arm
        self.N = np.zeros(main.n_arm) # そのレバーが引かれた回数

        # Algorithm parameters
        self.epsilon = epsilon # the epsilon value
        self.init_val = init_val # Initialization of the inital expected values
        if (init_val != 0):
            for i in range (0, len(self.Q)):
                self.Q[i] = init_val

    # main.py
    def action(self, p):
        # Select an action # 確率で行動を決めるのか
        if (self.epsilon > np.random.rand()): # exploration # rand() -> [0.0, 1.0)
            arm_id = np.random.randint(main.n_arm) # 一様分布で引くレバーを決める
        else: # exploit
            arm_id = np.argmax(self.Q)

        r = self.env.bandit(arm_id, p)
        self.N[arm_id] += 1 # そのレバーを引いた回数を+1
        # そのレバーの価値Qを求める
        if self.N[arm_id] == 1: # 初期値
            self.Q[arm_id] = r
        else:
            self.Q[arm_id] = self.Q[arm_id] + 1.0 / self.N[arm_id] * (r - self.Q[arm_id]) # Incremental update
        return arm_id

    def get_name(self):
        return "egreedy_" + str(self.epsilon) + "_" + str(self.init_val)
