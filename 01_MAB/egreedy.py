import numpy as np
import mab
import bandit_algorithm

class Agent(bandit_algorithm.BanditAlgorithm):
    def __init__(self, env, epsilon, init_val):
        # The parameter for the environment
        self.env = env # env->main
        self.Q = np.zeros(env.k, dtype=np.float) # Expected values for each arm
        self.N = np.zeros(env.k) # The number of trials for each arm

        # Algorithm parameters
        self.epsilon = epsilon # the epsilon value
        self.init_val = init_val # Initialization of the inital expected values
        if (init_val != 0):
            for i in range (0, len(self.Q)):
                self.Q[i] = 0

    def action(self):
        # Select an action
        if (self.epsilon > np.random.rand()): # exploration
            armid = np.random.randint(self.env.k)
        else: # exploit
            # The arm with the smallest index is selected when there are more than one optimal arms
            armid = np.argmax(self.Q)

        # Geting reward and incremental update
        r = self.env.bandit(armid)
        self.N[armid] += 1
        if self.N[armid] == 1:
            self.Q[armid] = r
        else:
            self.Q[armid] = self.Q[armid] + (r - self.Q[armid]) / self.N[armid] # Incremental update

    def get_name(self):
        return "egreedy_" + str(self.epsilon) + "_" + str(self.init_val)
