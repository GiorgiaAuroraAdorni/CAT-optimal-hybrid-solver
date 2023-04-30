from Game.Game import GameEnvironment
from Game.Instruction import *
from Tools.fileReader import file_reader

import numpy as np
import gym

from stable_baselines3 import DQN
from stable_baselines3.dqn import MlpPolicy
from stable_baselines3.common.env_checker import check_env

from stable_baselines3 import A2C

from stable_baselines3 import PPO
from stable_baselines3.common.vec_env import DummyVecEnv


# Crea un agente DQN utilizzando la policy MlpPolicy

# from Machine.custom_agent import MyCustomAgent

print("ciao1\n")

# Crea l'ambiente di gioco


path = "./Graph/TestGraph_0.txt"

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
num_colors = 4

env = GameEnvironment(V, voidMat,max_id, instructions, patterns, num_colors, map_value,n)

check_env(env)


# Crea il tuo agente di reinforcement learning (ad esempio, un DQN)
# agent = DQN(MlpPolicy, game_env, verbose=1)
# agent.learn(total_timesteps=1000, log_interval=4)

#agent = A2C("MlpPolicy", game_env, verbose=1)
#agent.learn(total_timesteps=100000)


env = DummyVecEnv([lambda: env])
agent = PPO("MlpPolicy", env, verbose=1)
agent.learn(total_timesteps=10000)
#agent.learn(total_timesteps=100000, log_interval=100)



if 1:
    num_episodes = 5
    for episode in range(num_episodes):
        state = env.reset()
        done = False
        episode_reward = 0
        step_iter = 0
        old_id = 0
        while not done:
            # Scegli un'azione utilizzando la politica dell'agente
            action, _ = agent.predict(state, deterministic=True)
            #print(action)
            # Esegui l'azione nell'ambiente di gioco e ottieni il prossimo stato, il reward e il flag 'done'
            next_state, reward, done, info = env.step(action)

            if info[0]['current_id'] == old_id:
                action, _ = agent.predict(state, deterministic=False)
                next_state, reward, done, info = env.step(action)

            # print(next_state)

            # Aggiorna lo stato corrente e il reward dell'episodio
            state = next_state
            episode_reward += reward
            step_iter += 1
            old_id = info[0]['current_id']
            # print(f"Step Info: {info}")


        print(f"Episode {episode + 1}: Reward = {episode_reward}: Steps = {step_iter}")
