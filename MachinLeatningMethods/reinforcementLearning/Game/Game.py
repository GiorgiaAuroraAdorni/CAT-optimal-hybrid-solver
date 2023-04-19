import os
import sys
import gym
from gym import spaces
import numpy as np

# Aggiungi il percorso del pacchetto padre alla variabile di ambiente PYTHONPATH
parent_path = os.path.abspath(os.path.join(os.getcwd(), ".."))
sys.path.append(parent_path)

# Importa la funzione file_reader dal modulo fileReader
from .Move import executeInstruction


class GameEnvironment(gym.Env):
    def __init__(self, V, max_id, instructions, patterns, num_colors, n):
        super().__init__()
        self.observation_space = spaces.Box(low=-2, high=num_colors, shape=(n, n), dtype=np.int)

        
        self.V = V
        self.max_id = max_id
        self.instructions = instructions
        self.patterns = patterns
        self.n = len(V)

        num_actions = n * n * len(instructions) * 4 * len(patterns)
        self.action_space = spaces.Discrete(num_actions)

        self.reset()

    def reset(self):
        self.currentMat = np.zeros((self.n, self.n), dtype=np.int)
        self.current_id = 0
        self.steps = 0
        return self.currentMat
    
    def execute_instruction(self, action):
        node_i, node_j, instruction, lengthOfInst, pattern = action
        self.current_id, num_new_colored_cells = executeInstruction(self.current_id, node_i, node_j, instruction, lengthOfInst, pattern, self.currentMat, self.V, 0)
        return num_new_colored_cells

    def step(self, action):
        num_patterns = len(self.patterns)
        num_instructions = len(self.instructions)
        
        node_j, action = divmod(action, self.n)
        node_i, action = divmod(action, self.n)
        instruction_idx, action = divmod(action, num_instructions)
        length, action = divmod(action, 4)
        pattern_idx = action % num_patterns
        
        instruction = self.instructions[instruction_idx]
        pattern = self.patterns[pattern_idx]

        num_new_colored_cells = self.execute_instruction((node_i, node_j, instruction, length, pattern))
        reward = self.calculate_reward(num_new_colored_cells)
        done = self.is_done()
        next_state = self.currentMat
        self.steps += 1
        return next_state, reward, done, {}

    def calculate_reward(self, num_new_colored_cells):
        # Calcola il reward in base al progresso verso la soluzione desiderata
        # Utilizza un moltiplicatore dinamico che diminuisce con il tempo
        multiplier = max(1 - self.steps / 1000, 0)
        reward = multiplier * num_new_colored_cells
        return reward

    def is_done(self):
        # Controlla se il gioco è terminato confrontando current_id con max_id
        return self.current_id >= self.max_id
