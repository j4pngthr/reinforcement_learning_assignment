import numpy as np

class Game():
    def __init__(self, prob_head):
        self.prob_head = prob_head

    def proc_head(self, state, stake):
        if state + stake >= 100:
            return 100, 1
        else:
            return state + stake, 0

    def proc_tail(self, state, stake):
        if state - stake <= 0:
            return 0, 0
        else:
            return state - stake, 0

class Agent():
    def __init__(self, env, gamma):
        self.env = env
        self.gamma = gamma
        self.states = [i for i in range(100)]
        self.state_val = np.zeros(101)
        self.final_policy = np.zeros(100)

    def policy_eval(self, num_steps):
        for i in range(num_steps):
            for s in self.states:
                if s == 0:
                    continue
                
                action_space = [i for i in range(1, min(s + 1, 101 - s))]
                action_val = []
                for a in action_space:
                    new_state_val = 0
                    new_state, reward = self.env.proc_head(s, a)
                    new_state_val += self.env.prob_head * (reward + self.gamma * self.state_val[new_state])
                    new_state, reward = self.env.proc_tail(s, a)
                    new_state_val += (1 - self.env.prob_head) * (reward + self.gamma * self.state_val[new_state])
                    action_val.append(new_state_val)
                index = np.argmax(np.asarray(action_val))
                self.state_val[s] = action_val[index]
    
    def policy_impr(self):
        for s in self.states:
            if s == 0:
                continue
            
            action_space = [i for i in range(1, min(s + 1, 101 - s))]
            action_val = []
            for a in action_space:
                new_state_val = 0
                new_state, reward = self.env.proc_head(s, a)
                new_state_val += self.env.prob_head * (reward + self.gamma * self.state_val[new_state])
                new_state, reward = self.env.proc_tail(s, a)
                new_state_val += (1 - self.env.prob_head) * (reward + self.gamma * self.state_val[new_state])
                action_val.append(new_state_val)

            # Round floating points to remove the errors.
            for i in range(len(action_val)):
                action_val[i] = round(action_val[i], 8)
            
            # Find the greedy choise.
            print(s, ":", str(action_val))
            index = np.argmax(np.asarray(action_val))
            self.final_policy[s] = index + 1 # The index + 1 is the stake
