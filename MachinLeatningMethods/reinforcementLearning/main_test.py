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



path = "./Graph/TestGraph_TEST.txt"
V = file_reader(path)
n = len(V)


map_value = [-1] * (n ** 2)
total_colored = 0
tmp_iter_val = 0
voidMat = [[0] * n for _ in range(n)]

for i in range(n):
    for j in range(n):
        if V[i][j] < 0:
            voidMat[i][j] = V[i][j]
        else:
            total_colored += 1
            map_value[i*n+j] = tmp_iter_val
            tmp_iter_val += 1

max_id = 0
for i in range(total_colored):
    max_id += 2**i

instructions = TOT_istructions_test
num_colors = 2
patterns = generate_combinations(num_colors)
env = GameEnvironment(V, voidMat,max_id, instructions, patterns, num_colors, map_value,n)
check_env(env)
env = DummyVecEnv([lambda: env])

# DQN Model
# agent = DQN(MlpPolicy, game_env, verbose=1)
# agent.learn(total_timesteps=100000)

# A2C Model
#agent = A2C("MlpPolicy", game_env, verbose=1)
#agent.learn(total_timesteps=100000)

# PPO Model
agent = PPO("MlpPolicy", env, verbose=1)
agent.learn(total_timesteps=200000)


# Test model Perform
if 1:
    num_episodes = 1
    for episode in range(num_episodes):
        state = env.reset()
        done = False
        episode_reward = 0
        step_iter = 0
        old_id = 0
        while not done:

            action, _ = agent.predict(state, deterministic=True)
            next_state, reward, done, info = env.step(action)
            state = next_state
            episode_reward += reward
            step_iter += 1
            old_id = info[0]['current_id']

            if step_iter > 10:
                break


        print(f"Episode {episode + 1}: Reward = {episode_reward}: Steps = {step_iter}")
