from Game.Instruction import *
from stable_baselines3.common.env_checker import check_env
from stable_baselines3 import PPO
from stable_baselines3.common.vec_env import DummyVecEnv
from Tools.setup_enviroment import *
import sys
import os

def main(filenumber):
    envStr= "./Graph/TestGraph_"+str(filenumber)+".txt"
    env = getEnviroment(envStr)
    check_env(env)
    env = DummyVecEnv([lambda: env])

    custom_objects = {
        'action_space': env.action_space,
        'observation_space': env.observation_space,
        'CustomEnv': env
    }

    new_agent2 = PPO.load("./AI/AI_Graph_"+str(filenumber)+".zip", env=env,custom_objects=custom_objects)

    # Test model Perform
    envv = getEnviroment(envStr)

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
                action, _ = new_agent2.predict(state, deterministic=True)
                envv.step(action[0])
                next_state, reward, done, info = env.step(action)
                print("A")

                state = next_state
                episode_reward += reward
                print(reward)
                step_iter += 1
                old_id = info[0]['current_id']

                if step_iter > 10:
                    break

            envv.print_state()

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("Usage: python script.py <filename>")
        sys.exit(1)
        
    filename = sys.argv[1]
    main(filename)