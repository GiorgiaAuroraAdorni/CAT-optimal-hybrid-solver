from Game.Game_IMG import GameEnvironment
from Game.Instruction import *
from Tools.fileReader import file_reader
from stable_baselines3.common.env_checker import check_env
from stable_baselines3 import PPO
from stable_baselines3.common.vec_env import DummyVecEnv
from Game.Wrappen import *

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

def getEnviroment():
    env = GameEnvironment(V, voidMat,max_id, instructions, patterns, num_colors, map_value,n)
    check_env(env)
    env = DummyVecEnv([lambda: env])
    return env

env = getEnviroment()

agent = PPO("CnnPolicy", env, verbose=1)
agent.learn(total_timesteps=50000)
agent.save("PPO_model_CNN.zip")
agent = PPO.load("PPO_model_CNN.zip")

envv = GameEnvironment(V, voidMat,max_id, instructions, patterns, num_colors, map_value,n)
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
            print(action)
            envv.step(action[0])
            envv.print_state()

            next_state, reward, done, info = env.step(action)
            state = next_state
            episode_reward += reward
            step_iter += 1
            old_id = info[0]['current_id']

            if step_iter > 10:
                print("break")
                break


        print(f"Episode {episode + 1}: Reward = {episode_reward}: Steps = {step_iter}")
