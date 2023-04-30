from Game.Game3 import GameEnvironment

from Game.Instruction import *
from Tools.fileReader import file_reader

#import numpy as np
#import gymnasium as gym
#from stable_baselines3 import DQN
from stable_baselines3 import A2C

from stable_baselines3.common.env_checker import check_env
from stable_baselines3 import PPO
from stable_baselines3.common.vec_env import DummyVecEnv


path = "./Graph/TestGraph_0.txt"
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

if 0:
    action = (2, 0, 4, 3, 10)  # Scegli un'azione valida (node_i, node_j, instruction_idx, length, pattern_idx)
    next_state, reward, done, _ = env.step(action)
    env.print_state()
    env.reset()
    env.print_state()



check_env(env)
env = DummyVecEnv([lambda: env])

# DQN Model
# agent = DQN(MlpPolicy, game_env, verbose=1)
# agent.learn(total_timesteps=100000)

# A2C Model
if 0:
    agent = A2C("CnnPolicy", env, verbose=1)
    agent.learn(total_timesteps=100000)
    agent.save("A2C_model.zip")

# PPO Model
if 0:
    agent = PPO("CnnPolicy", env, verbose=1)
    agent.learn(total_timesteps=100000)
    agent.save("PPO_model.zip")


if 1:
    agent = PPO("MlpPolicy", env, verbose=1)
    agent.learn(total_timesteps=200000)
    agent.save("PPO_model.zip")



agent = PPO.load("PPO_model.zip")
env2 = GameEnvironment(V, voidMat,max_id, instructions, patterns, num_colors, map_value,n)


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
            action_2 = action.copy()
            #print(action)
            env2.step(action[0])
            env2.print_move(action[0])

            next_state, reward, done_a, info = env.step(action)
            if info[0]['current_id'] == old_id:                         # nel caso il deterministc finisce in un blocco
                print("non det")
                #a = int(input("Inserisci il primo numero: "))
                #b = int(input("Inserisci il secondo numero: "))
                #c = int(input("Inserisci il terzo numero: "))
                #actionn = [[a,b,0,1,c,c,c,c]]

                action, _ = agent.predict(state, deterministic=False)
                next_state, reward, done_a, info = env.step(action)
                env2.step(action[0])
                
            else:
                print("det")

            #env2.print_state()
            #print(next_state)
            #print(f"Step Info: {info}")
            # Aggiorna lo stato corrente e il reward dell'episodio
            state = next_state
            episode_reward += reward
            step_iter += 1
            old_id = info[0]['current_id']
            print(action)

            print(episode_reward, old_id)
            env2.print_info_state(action[0])
            env2.print_state()

            done = done_a
            if step_iter > 10:
                break


        print("CIAO")
        print(old_id, env2.max_id, env2.is_done())
        env2.print_state()
        print(f"Episode {episode + 1}: Reward = {episode_reward}: Steps = {step_iter}")
