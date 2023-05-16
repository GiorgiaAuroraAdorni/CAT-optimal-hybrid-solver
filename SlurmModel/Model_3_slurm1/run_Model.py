from Game.Instruction import *
from stable_baselines3.common.env_checker import check_env
from stable_baselines3 import PPO
from stable_baselines3.common.vec_env import DummyVecEnv
from Tools.setup_enviroment import *



env = getEnviroment(flag=None)
check_env(env)
env = DummyVecEnv([lambda: env])

custom_objects = {
    'action_space': env.action_space,
    'observation_space': env.observation_space,
    'CustomEnv': env
}

new_agent2 = PPO.load("PPO_model_MLP_1_0_.zip", env=env,custom_objects=custom_objects)

# Test model Perform
envv = getEnviroment(flag=None)

if 1:
    num_episodes = 1
    for episode in range(num_episodes):
        state = env.reset()
        half_length = len(state[0]) // 2
        array_half = state[0][half_length:]    
        # Creare la matrice 6x6
        matrix = np.reshape(array_half, (6, 6))
        envv.currentMat = matrix
        done = False
        episode_reward = 0
        step_iter = 0
        old_id = 0
        while not done:
            envv.print_state()
            action, _ = new_agent2.predict(state, deterministic=True)
            next_state, reward, done, info = env.step(action)
            half_length = len(state[0]) // 2
            array_half = state[0][half_length:]    
            # Creare la matrice 6x6
            matrix = np.reshape(array_half, (6, 6))
            envv.currentMat = matrix
            state = next_state
            episode_reward += reward
            print(reward)
            step_iter += 1
            old_id = info[0]['current_id']

            if step_iter > 10:
                break

        envv.print_state()
        print(f"Episode {episode + 1}: Reward = {episode_reward}: Steps = {envv.steps}")
        print("id =", old_id, "real id =", envv.current_id, "max_id = ", 1048575, "Done =", done)
        envv.print_state()
