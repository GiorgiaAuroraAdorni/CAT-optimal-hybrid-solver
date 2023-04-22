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
    def __init__(self, V,voidMat, max_id, instructions, patterns, num_colors,map_value, n):
        super().__init__()
        self.observation_space = spaces.Box(low=-2, high=num_colors, shape=(n, n), dtype=np.int)

        self.action_space = spaces.MultiDiscrete([
            n,  # node_i
            n,  # node_j
            len(instructions),  # instruction_idx
            4,  # length (da 1 a 4)
            len(patterns)  # pattern_idx
        ])

        self.map_value = map_value
        self.V = V
        self.max_id = max_id
        self.instructions = instructions
        self.patterns = patterns
        self.n = len(V)
        self.voidMat = voidMat
        self.reset()

    def reset(self):
        self.currentMat = np.copy(self.voidMat)
        self.current_id = 0
        self.steps = 0
        return self.currentMat
    
    def execute_instruction(self, action):
        node_i, node_j, instruction, lengthOfInst, pattern = action
        self.current_id, num_new_colored_cells, self.currentMat = executeInstruction(self.current_id, node_i, node_j, instruction, lengthOfInst, pattern, self.V, self.currentMat, self.map_value)
        return num_new_colored_cells
        
    def print_current(self):
        print(self.currentMat)

    def print_move(self, action):
        node_i, node_j, instruction, lengthOfInst, pattern = action
        print("node:", node_i, node_j, " - instruction:", self.instructions[instruction], " - lenOfInst :", lengthOfInst, " - Pattern :", self.patterns[pattern])

    def print_info_state(self, action):
        self.print_move(action)
        self.print_current()
        print("steps: ", self.steps, " - current id:", self.current_id)

    def step(self, action):
        node_i, node_j, instruction_idx, length, pattern_idx = action

        instruction = self.instructions[instruction_idx]
        pattern = self.patterns[pattern_idx]

        num_new_colored_cells = self.execute_instruction((node_i, node_j, instruction, length, pattern))
        reward = self.calculate_reward(num_new_colored_cells)
        done = self.is_done()
        next_state = np.copy(self.currentMat)
        self.steps += 1

        info = {}  # Aggiungi un dizionario vuoto per "info"

        return next_state, reward, done,info

    def calculate_reward(self, num_new_colored_cells):
        # Calcola il reward in base al progresso verso la soluzione desiderata
        # Utilizza un moltiplicatore dinamico che diminuisce con il tempo
        multiplier = max(1 - self.steps / 1000, 0)
        reward = multiplier * num_new_colored_cells
        return reward

    def is_done(self):
        # Controlla se il gioco è terminato confrontando current_id con max_id
        return self.current_id >= self.max_id
