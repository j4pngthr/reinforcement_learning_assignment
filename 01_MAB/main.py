import egreedy
import mab
import myio
import numpy as np
import sys

# The parmeters for environment
k = 10
mu = np.array([0.2, -0.9, 1.5, 0.5, 1.2, -1.4, -0.2, -1.1, 0.9, -0.7])
sig = np.array([1, 1, 1, 1, 1, 1, 1, 1, 1, 1])

if __name__ == "__main__":
    # The arguments from a terminal
    argv = sys.argv
    if len(argv) != 3: # python main.py x x -> argv = [main.py, epsilon, init_val]
        print("Invalid number of arguments.")
        quit()

    # Paramter setting
    epsilon = float(argv[1])
    init_val = float(argv[2])
    print("Running e-greedy", epsilon, init_val,"...")

    # Algorithm execution
    env = mab.MAB(k, mu, sig)
    a = egreedy.Agent(env, epsilon, init_val)
    for i in range(0, env.MAX_STEPS):
        a.action()

    # Output the results to a file
    # a.get_name() -> Agentの関数 egreedy.py
    myio.write_data(a.get_name(), 0, env.avg_rewards.tolist(), "result.txt")
