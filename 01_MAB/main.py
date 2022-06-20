import egreedy
import mab
import ucb
import myio
import numpy as np
import sys
import random

k = 10 # 1個のマシンの腕の数
# mu = np.array([0.2, -0.9, 1.5, 0.5, 1.2, -1.4, -0.2, -1.1, 0.9, -0.7])
# sig = np.array([1, 1, 1, 1, 1, 1, 1, 1, 1, 1])
mu = np.empty(0)
sig = np.empty(0)
n_machine = 100

init_val = 0

def make_params(): # parameters
  global mu, sig
  mu = np.empty(0)
  sig = np.empty(0)
  for i in range(0, k):
    mu = np.append(mu, random.gauss(0, 1))
    sig = np.append(sig, 1)

# mab
if __name__ == "__main__":
  key = 0
  if key == 0:
    for epsilon in [0, 0.01, 0.1]:
      for i in range(0, n_machine):
        print(epsilon, "i", i)
        # 毎回初期化しないとダメ
        make_params()
        env = mab.MAB(k, mu, sig) # mab.py
        a = egreedy.Agent(env, epsilon, init_val) # egreedy.py

        for t in range(0, env.MAX_STEPS):
          # print("t", t)
          a.action() # egreedy.py/Agent.action

        if i == 0:
          avg_avg_rewards = np.empty(0)
          for j in range(0, env.MAX_STEPS):
            avg_avg_rewards = np.append(avg_avg_rewards, env.avg_rewards[j] / n_machine)
        else:
          for j in range(0, env.MAX_STEPS):
            avg_avg_rewards[j] += env.avg_rewards[j] / n_machine

      # Output the results to a file
      # a.get_name() -> Agentの関数 egreedy.py
      temp = str(epsilon)
      file_name = ""
      for _c in temp:
        if _c == '.':
          file_name = file_name + '_'
        else:
          file_name = file_name + _c
      file_name = "eps" + file_name + ".txt"
      myio.write_data(a.get_name(), 0, avg_avg_rewards, file_name)
  # elif key == 1:
  #   epsilon = 0.1
  #   env = mab.MAB(k, mu, sig) # mab.py
  #   # envの後
  #   a = egreedy.Agent(env, epsilon, init_val)
  #
  #   for t in range(0, env.MAX_STEPS):
  #     a.action() # egreedy.py/Agent.action
  #
  #   myio.write_data(a.get_name(), 0, env.avg_rewards.tolist(), "result.txt")
  # elif key == 2:
  #   c = 1 # ucb
  #   env = mab.MAB(k, mu, sig) # mab.py
  #   a = ucb.Agent(env, epsilon, init_val) # egreedy.py
  #
  #   for t in range(0, env.MAX_STEPS):
  #     a.action(t, c)

    # Output the results to a file
    # a.get_name() -> Agentの関数 egreedy.py
