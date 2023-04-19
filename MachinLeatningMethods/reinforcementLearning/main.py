from Game.Game import GameEnvironment
from Game.Instruction import *
from Tools.fileReader import file_reader

import numpy as np

from stable_baselines3 import DQN
from stable_baselines3.dqn import MlpPolicy
from stable_baselines3.common.env_checker import check_env

# Crea un agente DQN utilizzando la policy MlpPolicy

# from Machine.custom_agent import MyCustomAgent

print("ciao1\n")

# Crea l'ambiente di gioco


path = "./Graph/TestGraph_6.txt"

# read file and convert information into a matrix
V = file_reader(path)
n = len(V)

# Create Base case with the void table
# in this step we can save the number of colored value
map_value = [-1] * (n ** 2)
total_colored = 0
tmp_iter_val = 0
voidMat = [[0] * n for _ in range(n)]

print("ciao2\n")
for i in range(n):
    for j in range(n):
        if V[i][j] < 0:
            voidMat[i][j] = V[i][j]
        else:
            total_colored += 1
            map_value[i*n+j] = tmp_iter_val
            tmp_iter_val += 1

# with the information we can cmpute the dimension of the max id of a state (for the full colored graph)
# And we can also initialize the memory vector
max_id = 0
for i in range(total_colored):
    max_id += 2**i

size = 2**total_colored
#memory = [-1] * size
#memory[max_id] = 0 

print("ciao3\n")

instructions = TOT_istructions
patterns = generate_combinations(4)


num_instructions = len(instructions)
num_patterns = len(patterns)
max_length = 4
num_actions = num_instructions * num_patterns * max_length



game_env = GameEnvironment(V, max_id, instructions, patterns, 4, n)
check_env(game_env)

agent = DQN("MlpPolicy", game_env, verbose=1)

# Crea il tuo agente di reinforcement learning (ad esempio, un DQN)
agent = DQN(MlpPolicy, game_env, verbose=1)
agent.learn(total_timesteps=1000, log_interval=4)

# Imposta il numero di episodi per l'addestramento
num_episodes = 1000

 
# Esegui l'addestramento
for episode in range(num_episodes):
    state = game_env.reset()
    done = False

    while not done:
        # Scegli un'azione utilizzando la politica dell'agente
        action, _ = agent.predict(state, deterministic=False)

        # Esegui l'azione nell'ambiente di gioco e ottieni il prossimo stato, il reward e il flag 'done'
        next_state, reward, done, _ = game_env.step(action)

        # Memorizza l'esperienza dell'agente (stato, azione, reward, next_state, done)
        agent.replay_buffer.add(state, action, reward, next_state, done)

        # Esegui un passo di apprendimento dell'agente
        agent.learn(total_timesteps=1)

        # Aggiorna lo stato corrente
        state = next_state