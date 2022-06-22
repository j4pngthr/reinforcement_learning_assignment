import numpy as np
import mab
import bandit_algorithm
import random

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

  # main.py
  def action(self, p):
    # Select an action # 確率で行動を決めるのか
    if (self.epsilon > np.random.rand()): # exploration # rand() -> [0.0, 1.0)
      arm_id = np.random.randint(self.env.k) # 一様分布で引くレバーを決める
    else: # exploit
      # The arm with the smallest index is selected when there are more than one optimal arms
      # _max = self.Q[:].max()
      # indexes = np.argwhere(self.Q[:] == _max)
      # # print(indexes)
      # random.shuffle(indexes)
      # arm_id = indexes[0][0]
      arm_id = np.argmax(self.Q)

    # Geting reward and incremental update
    r = self.env.bandit(arm_id, p)
    self.N[arm_id] += 1 # そのレバーを引いた回数を+1
    # そのレバーの価値Qを求める
    if self.N[arm_id] == 1: # 初期値
      self.Q[arm_id] = r
    else:
      self.Q[arm_id] = self.Q[arm_id] + 1.0 / self.N[arm_id] * (r - self.Q[arm_id]) # Incremental update

    # self.Q[arm_id] = self.env.bandit(arm_id)
    # print(arm_id, is_rand, self.Q[arm_id], self.env.mu[arm_id])

  def get_name(self):
    return "egreedy_" + str(self.epsilon) + "_" + str(self.init_val)
