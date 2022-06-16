from gridworld import Gridworld, Agent
import numpy as np

GAMMA = 1
THETA = 0.001

def run_gridworld(num_steps):
    max_x = 4 # The width of a grid
    max_y = 4 # The height of a grid
    terminal_states = [[0,0], [max_x - 1, max_y - 1]]
    env = Gridworld(max_x, max_y, terminal_states)
    agent = Agent(env, THETA, GAMMA)
    agent.policy_eval(num_steps)
    agent.policy_impr()

    print("+++++ k =", num_steps, "+++++")
    print("The state values")
    print(agent.state_val, "\n")
    print("The policy")
    print(agent.greedy_policy, "\n")

def policy_iteration(): # 方策反復
    max_x = 4 # The width of a grid
    max_y = 4 # The height of a grid
    terminal_states = [[0,0], [max_x - 1, max_y - 1]]
    env = Gridworld(max_x, max_y, terminal_states) # gridworld.py
    agent = Agent(env, THETA, GAMMA) # gridworld.py

    num_steps = 0
    while num_steps < 1000: # 方策改善を1000回で打ち切る
        agent.policy_eval(1) # 方策評価 方策に対する状態価値関数を計算
        stable = agent.policy_impr() # 方策改善
        num_steps += 1

        print("+++++ k =", num_steps, "+++++")
        print("The state values")
        print(agent.state_val, "\n")
        # print("The action values")
        # print(agent.action_val, "\n")

        # Break the loop if the policy imporvement does not change the policy
        if stable:
            break


    # print("+++++ k =", num_steps, "+++++")
    # print("The state values")
    # print(agent.state_val, "\n")
    print("The policy")
    print(agent.greedy_policy, "\n")

# python main.pyのみ実行
if __name__ == "__main__":
    #for i in [1, 2, 3, 10, 1000]:
    #    run_gridworld(i)

    policy_iteration()
