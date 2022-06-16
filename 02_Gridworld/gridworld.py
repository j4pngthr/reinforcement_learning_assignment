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

# class Agent():
#     def __init__(self, env, theta, gamma):
#         self.env = env
#         self.theta = theta
#         self.gamma = gamma
#         self.states = [[i, j] for i in range(env.width) for j in range(env.height)]
#         self.state_val = np.zeros((env.width, env.height)) # The state values, v_\pi(s)
#
#         # Policy, \pi(s)
#         self.random_policy = np.full((env.width, env.height, 4), 0.25)
#         # Let the random policy at the terminal states to be 0, i.e., \pi_random(a|s) = 0
#         # 終端状態に着いたら終わり どこにも移動しない
#         for s in env.terminal_states:
#             for a in Actions:
#                 self.random_policy[s[0], s[1], a.value] = 0.0
#
#         # optimal policy
#         # enum -> name, value
#         self.greedy_policy = [[[Actions.UP.name, Actions.DOWN.name, Actions.RIGHT.name, Actions.LEFT.name] for i in range(env.width)]for j in range(env.height)]
#         for s in env.terminal_states:
#             self.greedy_policy[s[0]][s[1]] = []
#
#     # num_steps = 1
#     # main.py/policy_iteration
#     def policy_eval(self, num_steps): # 方策評価 方策に対する状態価値関数を計算
#         # 図4.1のアルゴリズムの実行
#         for i in range(num_steps):
#             delta = 0 # The indicator of improvement.
#             # 状態空間全体をスイープした後状態価値を更新
#             # Otherwise, the state values for each state will be updated based on the new state values.
#             copied_state_val = copy.copy(self.state_val)
#             for s in self.states:
#                 # _s = copy.copy(s)
#                 new_state_val = 0
#                 # 状態価値関数の値の計算
#                 # (4.4)
#                 for a in Actions:
#                     next_state, reward = self.env.proc_action(copy.copy(s), a)
#                     # Each action is taken with probability 0.25
#                     new_state_val += self.random_policy[s[0], s[1], a.value] * (reward + self.gamma * self.state_val[next_state[0]][next_state[1]])
#                 delta = max(delta, abs(new_state_val - self.state_val[s[0]][s[1]]))
#                 # copyをupdate
#                 copied_state_val[s[0]][s[1]] = new_state_val
#                 # if _s != s:
#                 #   print("s is changed")
#                 # else:
#                 #   print("s is not changed")
#
#             # Update the state values
#             self.state_val = copy.copy(copied_state_val)
#
#             # Terminate the iterations, when the improvement is sufficiently small.
#             if delta <= self.theta:
#                 break
#
#     # 新しい方策を求める
#     def get_greedy_choice(self, s): # policy_impr
#         greedy_actions = []
#         max_act_val = -10000
#         # 方策改善定理 削られたアクションが後で追加されることはない
#         for a in Actions:
#             next_state, reward = self.env.proc_action(copy.copy(s), a)
#             exp_act_val = self.random_policy[s[0], s[1], a.value] * (reward + self.gamma * self.state_val[next_state[0]][next_state[1]])
#             if max_act_val < exp_act_val:
#                 max_act_val = exp_act_val
#                 greedy_actions = [a.name]
#             elif max_act_val == exp_act_val:
#                 greedy_actions.append(a.name)
#             # これまでより悪くなることはない 方策改善定理
#         return greedy_actions
#
#     # main.py/policy_iteration
#     def policy_impr(self): # 方策改善
#         stable = True # 安定→改善されない
#         for s in self.states:
#             # 終端状態はとばす
#             if s in self.env.terminal_states:
#                 continue
#
#             # For non-terminal states
#             old_action = copy.copy(self.greedy_policy[s[0]][s[1]])
#             self.greedy_policy[s[0]][s[1]] = self.get_greedy_choice(s)
#             # print(str(old_action), str(self.greedy_policy[s[0]][s[1]]))
#             # print(stable)
#             if old_action != self.greedy_policy[s[0]][s[1]]:
#                 stable = False
#             # print(stable, old_action, self.greedy_policy[s[0]][s[1]])
#
#         return stable

class Agent():
  def __init__(self, env, theta, gamma):
    self.env = env
    self.theta = theta
    self.gamma = gamma
    self.states = [[i, j] for i in range(env.width) for j in range(env.height)]
    self.action_val = np.zeros((env.width, env.height, 4))

    # Policy, \pi(s)
    self.random_policy = np.full((env.width, env.height, 4), 0.25)
    # Let the random policy at the terminal states to be 0, i.e., \pi_random(a|s) = 0
    # 終端状態に着いたら終わり どこにも移動しない
    for s in env.terminal_states:
      for a in Actions:
        self.random_policy[s[0], s[1], a.value] = 0.0

    # optimal policy
    # enum -> name, value
    self.greedy_policy = [[[Actions.UP.name, Actions.DOWN.name, Actions.RIGHT.name, Actions.LEFT.name] for i in range(env.width)]for j in range(env.height)]
    for s in env.terminal_states:
      self.greedy_policy[s[0]][s[1]] = []

  # num_steps = 1
  # main.py/policy_iteration
  # exercise 4.5 and state_valのコード
  def policy_eval(self, num_steps): # 方策評価 方策に対する状態価値関数を計算
    # 図4.1のアルゴリズムの実行
    for i in range(num_steps):
      delta = 0 # The indicator of improvement.
      # 状態空間全体をスイープした後状態価値を更新
      # Otherwise, the state values for each state will be updated based on the new state values.
      copied_action_val = copy.copy(self.action_val)
      for s in self.states:
        for a in Actions:
          new_action_val = 0 # 足していく
          # s'
          next_state, reward = self.env.proc_action(copy.copy(s), a)
          # a'
          temp = 0
          for next_action in Actions:
            temp += self.greedy_policy[next_state[0], next_state[1], next_action.value] * self.action_val[next_state[0]][next_state[1]][next_action.value]
          new_action_val = self.random_policy[s[0], s[1], a.value] * (reward + self.gamma * temp)
          delta = max(delta, abs(new_action_val - self.action_val[s[0]][s[1]][a.value]))
          copied_action_val[s[0]][s[1]][a.value] = new_state_val

      self.action_val = copy.copy(copied_action_val)

      # Terminate the iterations, when the improvement is sufficiently small.
      if delta <= self.theta:
        break

  # 新しい方策を求める
  def get_greedy_choice(self, s): # policy_impr
    greedy_actions = []
    max_act_val = -10000
    # 方策改善定理 削られたアクションが後で追加されることはない
    for a in Actions:
      next_state, reward = self.env.proc_action(copy.copy(s), a)
      exp_act_val = self.random_policy[s[0], s[1], a.value] * (reward + self.gamma * self.state_val[next_state[0]][next_state[1]])
      if max_act_val < exp_act_val:
        max_act_val = exp_act_val
        greedy_actions = [a.name]
      elif max_act_val == exp_act_val:
        greedy_actions.append(a.name)
      # これまでより悪くなることはない 方策改善定理
    return greedy_actions

  # main.py/policy_iteration
  def policy_impr(self): # 方策改善
    stable = True # 安定→改善されない
    for s in self.states:
      # 終端状態はとばす
      if s in self.env.terminal_states:
        continue

      for a in Actions:
        old_action = copy.copy(self.greedy_policy[s[0]][s[1]])
        self.greedy_policy[s[0]][s[1]] = self.get_greedy_choice(s)
        # print(str(old_action), str(self.greedy_policy[s[0]][s[1]]))
        # print(stable)
        if old_action != self.greedy_policy[s[0]][s[1]]:
          stable = False
        # print(stable, old_action, self.greedy_policy[s[0]][s[1]])

    return stable

# pi(s), pi(a|s)の2つがある
