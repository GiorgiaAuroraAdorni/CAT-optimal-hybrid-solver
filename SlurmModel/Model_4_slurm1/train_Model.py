from Tools.setup_enviroment import *
from stable_baselines3.common.env_checker import check_env
from stable_baselines3 import PPO
from stable_baselines3.common.vec_env import DummyVecEnv, SubprocVecEnv
import os


def make_env():
    def _init():
        env = getEnviroment()
        return env
    return _init

def main():
    single_env = make_env()()
    check_env(single_env)

    num_envs = 14
    env = SubprocVecEnv([make_env() for _ in range(num_envs)])

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

    agent.learn(total_timesteps=20000000, reset_num_timesteps=False, tb_log_name="PPO_COPY_ALL_BOARD")
    # 1 = copy si 0 = mirror no
    agent.save("PPO_model_MLP_1_0_.zip")

if __name__ == '__main__':
    main()