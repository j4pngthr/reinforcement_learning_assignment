from gambler import Game, Agent
import matplotlib.pyplot as plt

GAMMA = 1
PROB_HEAD = 0.4

def run_policy_eval():
    data = [] # The estimated state values

    # Simulations
    env = Game(PROB_HEAD)
    for i in [1, 2, 3, 32]:
        agent = Agent(env, GAMMA)
        agent.policy_eval(i)
        data.append(agent.state_val)

        print("+++++", i, "sweeps +++++")
        print(agent.state_val)
    
    # Plot the estimated state values
    capital = [i for i in range(0, 100)]
    for i in range(len(data)):
        plt.plot(capital, data[i][:100])
    
    plt.xlabel('Capital')
    plt.ylabel('Estimated state values')
    plt.show()

def get_final_policy():
    # Simulations
    env = Game(PROB_HEAD)
    agent = Agent(env, GAMMA)
    agent.policy_eval(100)
    agent.policy_impr()
    
    print("+++++ Final policy +++++")
    print(agent.final_policy)

    # Plot the estimated state values
    capital = [i for i in range(0, 100)]
    for i in range(len(agent.final_policy)):
        plt.plot(capital, agent.final_policy[:100])
    
    plt.xlabel('Capital')
    plt.ylabel('Final policy')
    plt.show()

if __name__ == "__main__":
    # Policy evaluations
    run_policy_eval()

    # Final policy
    get_final_policy()
