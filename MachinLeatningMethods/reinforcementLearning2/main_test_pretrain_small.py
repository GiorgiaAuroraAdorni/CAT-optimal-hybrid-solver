from Game.Game_pretrain import GameEnvironmentPreTrain
from Game.Game_pretrain_2 import GameEnvironmentPreTrain2
from Game.Game_train import GameEnvironmentTrain as GameEnvironment
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



paths = ["./Graph/TestGraph_TEST_4COL.txt" ]
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


env = GameEnvironmentPreTrain(boards, voidMat,max_id, instructions, patterns, num_colors, map_value,n)
check_env(env)
env = DummyVecEnv([lambda: env])


import os
logdir = "logs_HyperParameter"

if not os.path.exists(logdir):
    os.makedirs(logdir)

agent = PPO("MlpPolicy", 
            env, verbose=1,
            n_steps=2048,
            batch_size=128,
            n_epochs=30,
            learning_rate=0.0003,
            clip_range=0.15,
            ent_coef=0.01,
            tensorboard_log=logdir)

agent = PPO("MlpPolicy", env, verbose=1)

agent.learn(total_timesteps=300000, reset_num_timesteps=False, tb_log_name="entropy_001_1")
agent.save("PPO_model_Pretrain_small_1.zip")


env2 = GameEnvironmentPreTrain2(boards, voidMat,max_id, instructions, patterns, num_colors, map_value,n)
check_env(env)
env2 = DummyVecEnv([lambda: env2])
agent = PPO.load("PPO_model_Pretrain_small_1.zip")
agent.set_env(env2)
agent.learn(total_timesteps=300000, reset_num_timesteps=False, tb_log_name="entropy_001_2")
agent.save("PPO_model_Pretrain_small_2.zip")

env3 = GameEnvironment(boards, voidMat,max_id, instructions, patterns, num_colors, map_value,n)
check_env(env3)
env3 = DummyVecEnv([lambda: env3])
agent = PPO.load("PPO_model_Pretrain_small_2.zip")
agent.set_env(env3)
agent.learn(total_timesteps=500000, reset_num_timesteps=False, tb_log_name="entropy_001_3")
agent.save("PPO_model_Pretrain_small_3.zip")



# Test model Perform
envv = GameEnvironmentPreTrain(boards, voidMat,max_id, instructions, patterns, num_colors, map_value,n)

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
