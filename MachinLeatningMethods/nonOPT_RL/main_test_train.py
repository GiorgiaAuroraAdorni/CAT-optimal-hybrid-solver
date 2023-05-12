from Game.Game_train import GameEnvironmentTrain
from Game.Instruction import *
from Tools.fileReader import file_reader
from Game.Move_checkValidPOsition import *

import numpy as np
import gym

from stable_baselines3 import DQN
from stable_baselines3.dqn import MlpPolicy
from stable_baselines3.common.env_checker import check_env

from stable_baselines3 import A2C

from stable_baselines3 import PPO
from stable_baselines3.common.vec_env import DummyVecEnv

import numpy as np


paths = ["./Graph/TestGraph_8.txt" ]
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

instructions = TOT_istructions_2
num_colors = 4
patterns = generate_combinations(num_colors)



result = []

for inst in range(len(instructions)):
    for i in range(n):
        for j in range(n):
            for k in range(n):
                patt = checkInstAndgetPatt(i, j, instructions[inst], (k+1), V)
                if patt:
                    result.append([[i,j],(k+1),instructions[inst],patt])



        
instructions = result
print(len(instructions))
print(instructions[0])

env = GameEnvironmentTrain(boards, voidMat,max_id, instructions, num_colors, map_value,n)


check_env(env)
env = DummyVecEnv([lambda: env])

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


agent.learn(total_timesteps=5000000, reset_num_timesteps=False, tb_log_name="PPO_BIG_2")
agent.save("PPO_model_MLP_8.zip")
