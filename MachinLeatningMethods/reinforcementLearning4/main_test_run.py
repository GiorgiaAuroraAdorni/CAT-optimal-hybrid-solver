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



result = []

for inst in range(len(instructions)):
    for i in range(n):
        for j in range(n):
            flag_valid = executeInstruction(i, j, instructions[inst], 1, V)
            if flag_valid:
                result.append([[i,j],instructions[inst]])



        

#for i in range(len(TOT_istructions_test)):
#    current_combinations = list(itertools.product(TOT_position_test[i], [TOT_istructions_test[i]]))
#    result.extend(current_combinations)

instructions = result


print(instructions)



env = GameEnvironmentTrain(boards, voidMat,max_id, instructions, patterns, num_colors, map_value,n)
check_env(env)
env = DummyVecEnv([lambda: env])
env_2 = GameEnvironmentTrain(boards, voidMat,max_id, instructions, patterns, num_colors, map_value,n)



custom_objects = {
    'action_space': env_2.action_space,
    'observation_space': env_2.observation_space,
    'CustomEnv': env_2
}

new_agent2 = PPO.load("PPO_model_CNN_2.zip", env=env_2,custom_objects=custom_objects)

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
            action, _ = new_agent2.predict(state, deterministic=False)
            print(action[0])
            envv.step(action[0])
            next_state, reward, done, info = env.step(action)
            

            state = next_state
            episode_reward += reward
            print(reward)
            print("state: ",next_state)
            step_iter += 1
            old_id = info[0]['current_id']

            if step_iter > 10:
                break

        envv.print_state()
        print(f"Episode {episode + 1}: Reward = {episode_reward}: Steps = {envv.steps}")
        print("id =", old_id, "real id =", envv.current_id, "max_id = ", max_id)
        envv.print_state()
