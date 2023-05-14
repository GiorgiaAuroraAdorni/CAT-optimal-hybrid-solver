from Tools.setup_enviroment import *
from stable_baselines3.common.env_checker import check_env
from stable_baselines3 import PPO
from stable_baselines3.common.vec_env import DummyVecEnv

env = getEnviroment("./Graph/TestGraph_8.txt")
check_env(env)
env = DummyVecEnv([lambda: env])


import os
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

agent.learn(total_timesteps=200000, reset_num_timesteps=False, tb_log_name="PPO_COPY_8")
agent.save("PPO_model_MLP_8.zip")
