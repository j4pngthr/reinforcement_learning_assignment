import numpy as np
from enum import Enum
import copy

class Actions(Enum):
    UP = 0
    DOWN = 1
    RIGHT = 2
    LEFT = 3

class Gridworld():
    def __init__(self, width, height, terminal_states):
        self.width = width
        self.height = height
        self.terminal_states = terminal_states

    def proc_action(self, state, action): # proceed? process?
        # Note that [0, 0] is located at the left top cell.
        # The UP action decrements the y-axis by one
        if action is Actions.UP:
            if state[0] - 1 >= 0:
                state[0] -= 1
        elif action is Actions.DOWN:
            if state[0] + 1 < self.height:
                state[0] += 1
        elif action is Actions.RIGHT:
            if state[1] + 1 < self.width:
                state[1] += 1
        else: # LEFT
            if state[1] - 1 >= 0:
                state[1] -= 1

        # r(s, a, s') = -1 for all s, s' in S, S+ and a in A.
        # 動いたら報酬が-1
        return state, -1

class Agent():
    def __init__(self, env, theta, gamma):
        self.env = env
        self.theta = theta
        self.gamma = gamma
        self.states = [[i, j] for i in range(env.width) for j in range(env.height)]
        self.state_val = np.zeros((env.width, env.height)) # The state values, v_\pi(s)

        # Let the random policy at the terminal states to be 0, i.e., \pi_random(s|a) = 0
        self.random_policy = np.full((env.width, env.height, 4), 0.25) # Policy, \pi(s)
        for s in env.terminal_states:
            for a in Actions:
                self.random_policy[s[0], s[1], a.value] = 0.0

        # optimal policy
        self.greedy_policy = [[[Actions.UP.name, Actions.DOWN.name, Actions.RIGHT.name, Actions.LEFT.name] for i in range(env.width)]for j in range(env.height)]
        for s in env.terminal_states:
            self.greedy_policy[s[0]][s[1]] = []

    def policy_eval(self, num_steps):
        for i in range(num_steps):
            delta = 0 # The indicator of improvement.
            # Note that the state values needs to be udpated after sweeping all the states.
            # Otherwise, the state values for each state will be updated based on the new state values.
            copied_state_val = copy.copy(self.state_val)
            for s in self.states:
                new_state_val = 0
                for a in Actions:
                    next_state, reward = self.env.proc_action(copy.copy(s), a)
                    # Each action is taken with probability 0.25
                    new_state_val += self.random_policy[s[0], s[1], a.value] * (reward + self.gamma * self.state_val[next_state[0]][next_state[1]])
                delta = max(delta, abs(new_state_val - self.state_val[s[0]][s[1]]))
                copied_state_val[s[0]][s[1]] = new_state_val

            # Update the state values
            self.state_val = copy.copy(copied_state_val)

            # Terminate the iterations, when the improvement is sufficiently small.
            if delta <= self.theta:
                break

    def get_greedy_choise(self, s):
        greedy_actions = []
        max_act_val = -10000
        for a in Actions:
            next_state, reward = self.env.proc_action(copy.copy(s), a)
            exp_act_val = self.random_policy[s[0], s[1], a.value] * (reward + self.gamma * self.state_val[next_state[0]][next_state[1]])
            if max_act_val < exp_act_val:
                max_act_val = exp_act_val
                greedy_actions = [a.name]
            elif max_act_val == exp_act_val:
                greedy_actions.append(a.name)

        return greedy_actions

    def policy_impr(self):
        stable = True
        for s in self.states:
            if s in self.env.terminal_states:
                continue

            # For non-terminal states
            old_action = copy.copy(self.greedy_policy[s[0]][s[1]])
            self.greedy_policy[s[0]][s[1]] = self.get_greedy_choise(s)
            print(str(old_action), str(self.greedy_policy[s[0]][s[1]]))
            if old_action != self.greedy_policy[s[0]][s[1]]:
                stable = False

        return stable
