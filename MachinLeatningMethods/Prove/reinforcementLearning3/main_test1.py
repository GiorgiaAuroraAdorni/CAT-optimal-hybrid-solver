from Game.Game5 import GameEnvironment

from Game.Instruction import *
from Tools.fileReader import file_reader

from Machin.CustomPolicy import *

#import numpy as np
#import gymnasium as gym
#from stable_baselines3 import DQN
from stable_baselines3 import A2C

from stable_baselines3.common.env_checker import check_env
from stable_baselines3 import PPO
from stable_baselines3.common.vec_env import DummyVecEnv, VecTransposeImage
from stable_baselines3.common.torch_layers import BaseFeaturesExtractor


import os
import numpy as np
from stable_baselines3 import PPO
from stable_baselines3.common.monitor import Monitor
from stable_baselines3.common.callbacks import CheckpointCallback, EvalCallback
from stable_baselines3.common.env_util import make_vec_env
from stable_baselines3.common.evaluation import evaluate_policy



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




log_dir = "./log"
os.makedirs(log_dir, exist_ok=True)

# Funzione per creare e wrappare l'ambiente
def create_env(log_dir):
    env = GameEnvironment(V, voidMat, max_id, instructions, patterns, num_colors, map_value, n)
    check_env(env)
    env = Monitor(env, log_dir)
    return env

# Crea l'ambiente di addestramento
env = create_env(log_dir)
env = DummyVecEnv([lambda: env])
env = VecTransposeImage(env)

# Crea un ambiente di valutazione
eval_log_dir = os.path.join(log_dir, 'eval')
os.makedirs(eval_log_dir, exist_ok=True)
eval_env = create_env(eval_log_dir)
eval_env = DummyVecEnv([lambda: eval_env])
eval_env = VecTransposeImage(eval_env)



# Crea un modello DQN con l'architettura della rete neurale desiderata
model = PPO(
    "MlpPolicy",
    env,
    learning_rate=3e-4,
    n_steps=2048,
    batch_size=64,
    n_epochs=10,
    gamma=0.99,
    gae_lambda=0.95,
    clip_range=0.2,
    ent_coef=0.01,
    vf_coef=0.5,
    max_grad_norm=0.5,
    tensorboard_log=log_dir,
    verbose=1,
)


# Definisci i callback per il salvataggio e la valutazione del modello
checkpoint_callback = CheckpointCallback(save_freq=10000, save_path=log_dir, name_prefix="ppo_model")
eval_callback = EvalCallback(eval_env, best_model_save_path=log_dir, log_path=log_dir, eval_freq=10000)

# Addestra il modello per un numero specifico di passi
model.learn(total_timesteps=100000, callback=[checkpoint_callback, eval_callback])

# Valuta il modello addestrato
mean_reward, std_reward = evaluate_policy(model, eval_env, n_eval_episodes=10)
print(f"Mean reward: {mean_reward:.2f} +/- {std_reward:.2f}")

# Salva il modello addestrato
model.save("ppo_game_environment")

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

            action, _ = model.predict(state, deterministic=True)
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

                action, _ = model.predict(state, deterministic=False)
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
