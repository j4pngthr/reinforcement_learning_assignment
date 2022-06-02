import numpy as np

class MAB():
    MAX_STEPS = 1000 # The number of steps
    step = 0 # The current time strep
    rewards = np.zeros(MAX_STEPS, dtype=np.float) # Rewards
    avg_rewards = np.zeros(MAX_STEPS, dtype=np.float) # The average rewards

    def __init__(self, k, mu, sig):
        self.k = k # The number of arms
        self.mu = mu # Mean
        self.sig = sig # sigma (variance = sig^2)

    def bandit(self, armid):
        assert(armid >= 0 and armid < self.k)

        # Getting reward
        r = np.random.normal(self.mu[armid], self.sig[armid], 1)

        # Save the average reward at time step self.step
        self.rewards[self.step] = r
        self.avg_rewards[self.step] = np.average(self.rewards[0:self.step + 1])
        self.step += 1

        return r

    def print_rewards():
        for i in range(0, len(self.rewards)):
            print("step ", i, " : ", self.rewards[i])
