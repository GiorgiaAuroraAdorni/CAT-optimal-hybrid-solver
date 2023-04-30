import numpy as np
from gymnasium import ObservationWrapper
from gymnasium import spaces

class ChannelFirstWrapper(ObservationWrapper):
    def __init__(self, env):
        super().__init__(env)
        obs_shape = env.observation_space.shape
        self.observation_space = spaces.Box(low=0, high=255, shape=(obs_shape[2], obs_shape[0], obs_shape[1]), dtype=np.uint8)

    def observation(self, observation):
        return np.transpose(observation, (2, 0, 1))