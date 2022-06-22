import egreedy
import mab
import ucb
import myio
import numpy as np
import sys
import random

k = 10 # 1個のマシンの腕の数
n_machine = 200
MAX_STEPS = 1000
init_val = 0

class params(): # parameters
    # average_machine_rewards
    avg_mac_rs = np.zeros(MAX_STEPS, dtype=np.float)
    t = 0
    def make_params(self):
        for i in range(k):
            _mu = np.random.normal(0, 1, n_machine).reshape((-1, 1)) # [[x_1], [x_2], ...]

            if i == 0:
                mu = _mu
            else:
                mu = np.hstack((mu, _mu)) # [[x_11, x_12], [x_21, x_22], ...]

        sig = [[1] * k] * n_machine
        return mu, sig

# mab
if __name__ == "__main__":
    key = 0
    if key == 0:
        p = params()
        mu, sig = p.make_params()

        for epsilon in [0, 0.01, 0.1]:
            for i in range(0, MAX_STEPS):
                p.avg_mac_rs[i] = 0

            for i in range(0, n_machine):
                print(epsilon, "i", i)

                env = mab.MAB(k, mu[i], sig[i]) # mab.py
                a = egreedy.Agent(env, epsilon, init_val) # egreedy.py

                for _t in range(0, MAX_STEPS):
                    p.t = _t
                    a.action(p) # egreedy.py/Agent.action

            # Output the results to a file
            # a.get_name() -> egreedy.py/Agent
            temp = str(epsilon)
            file_name = ""
            for _c in temp:
                if _c == '.':
                    file_name = file_name + '_'
                else:
                    file_name = file_name + _c
            file_name = "eps" + file_name + ".txt"
            myio.write_data(a.get_name(), 0, p.avg_mac_rs, file_name)
  
