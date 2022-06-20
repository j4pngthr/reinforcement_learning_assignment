import numpy as np

class MAB(): # 強化学習の環境
  MAX_STEPS = 1000 # The number of steps
  step = 0 # The current time strep
  # 配列 履歴を記録する
  rewards = np.zeros(MAX_STEPS, dtype=np.float) # Rewards
  avg_rewards = np.zeros(MAX_STEPS, dtype=np.float) # The average rewards

  def __init__(self, k, mu, sig): # どのレバーも同じ？
    self.k = k # The number of arms
    self.mu = mu # Mean
    self.sig = sig # σ 正規分布の標準偏差

  def bandit(self, arm_id): # 正規分布から報酬を得る # egreedy.py action
    assert(arm_id >= 0 and arm_id < self.k)

    # Getting reward
    r = np.random.normal(self.mu[arm_id], self.sig[arm_id], 1) # 正規分布 平均, 標準偏差, サイズ?

    # Save the average reward at time step self.step
    self.rewards[self.step] = r
    self.avg_rewards[self.step] = np.average(self.rewards[0:self.step + 1])
    self.step += 1

    # return r
    return self.avg_rewards[self.step - 1]

  def print_rewards():
    for i in range(0, len(self.rewards)):
      print("step ", i, " : ", self.rewards[i])
