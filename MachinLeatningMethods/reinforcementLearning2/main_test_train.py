from Game.Game_train import GameEnvironmentTrain
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



paths = ["./Graph/TestGraph_6.txt" ]
boards = [] 
n = 0
for path in paths:
    V = file_reader(path)
    n = len(V)
    boards.append(V)


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
num_colors = 4
patterns = generate_combinations(num_colors)
env = GameEnvironmentTrain(boards, voidMat,max_id, instructions, patterns, num_colors, map_value,n)
check_env(env)
env = DummyVecEnv([lambda: env])

# DQN Model
# agent = DQN(MlpPolicy, game_env, verbose=1)
# agent.learn(total_timesteps=100000)

# A2C Model
#agent = A2C("MlpPolicy", game_env, verbose=1)
#agent.learn(total_timesteps=100000)

import os
logdir = "logs"

if not os.path.exists(logdir):
    os.makedirs(logdir)

# PPO Model
#agent = PPO("MlpPolicy", env, verbose=1,tensorboard_log=logdir)

agent = PPO("MlpPolicy", 
            env, verbose=1,
            n_steps=2048,
            batch_size=128,
            n_epochs=30,
            learning_rate=0.0003,
            clip_range=0.15,
            ent_coef=0.01,
            tensorboard_log=logdir)

agent.learn(total_timesteps=2000000, reset_num_timesteps=False, tb_log_name="PPO_BIG_1")
agent.save("PPO_model_CNN.zip")
agent = PPO.load("PPO_model_CNN.zip")

# Test model Perform
envv = GameEnvironmentTrain(boards, voidMat,max_id, instructions, patterns, num_colors, map_value,n)

if 1:
    num_episodes = 1
    for episode in range(num_episodes):
        state = env.reset()
        done = False
        episode_reward = 0
        step_iter = 0
        old_id = 0
        while not done:
            envv.print_state()
            action, _ = agent.predict(state, deterministic=True)
            print(action[0])
            envv.step(action[0])
            
            #envv.print_info_state(action[0])
            next_state, reward, done, info = env.step(action)
            state = next_state
            episode_reward += reward
            print(reward)
            step_iter += 1
            old_id = info[0]['current_id']

            if step_iter > 10:
                break

        envv.print_state()
        print(f"Episode {episode + 1}: Reward = {episode_reward}: Steps = {envv.steps}")
        print("id =", old_id, "real id =", envv.current_id, "max_id = ", max_id)
        envv.print_state()
