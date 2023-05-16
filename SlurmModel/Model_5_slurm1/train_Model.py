from Tools.setup_enviroment import *
from stable_baselines3.common.env_checker import check_env
from stable_baselines3 import PPO
from stable_baselines3.common.vec_env import DummyVecEnv, SubprocVecEnv
import os

import sys
import os

def make_env(filename):
    def _init():
        env = getEnviroment(filename)
        return env
    return _init

def main(filenumber):
    filename = "./Graph/TestGraph_"+str(filenumber)+".txt"

    print("____________graph name:   " + filename+"____________")
    single_env = make_env(filename)()
    check_env(single_env)
    num_envs = 16
    env = SubprocVecEnv([make_env(filename) for _ in range(num_envs)])

    print("____________created env____________")
    logdir = "logs"
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

    print("____________start learn____________")
    agent.learn(total_timesteps=2000000, reset_num_timesteps=False, tb_log_name=("log_PPO_graph_"+str(filenumber)))
    # 1 = copy si 0 = mirror no
    print("____________Save learn____________")
    agent.save("AI_Graph_"+str(filenumber)+".zip")
    print("____________END____________")

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("Usage: python script.py <filename>")
        sys.exit(1)
        
    filename = sys.argv[1]
    main(filename)