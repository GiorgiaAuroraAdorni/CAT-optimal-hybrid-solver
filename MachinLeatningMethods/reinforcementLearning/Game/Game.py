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

        # ciò che vede il ML
        self.observation_space = spaces.Box(low=-2, high=num_colors, shape=(n, n), dtype=np.int)

        # lo spazio dell'azione
        self.action_space = spaces.MultiDiscrete([
            n,                                          # node_i
            n,                                          # node_j
            len(instructions),                          # instruction_idx
            4,                                          # length della mossa
            len(patterns)                               # pattern_idx
        ])

        self.map_value = map_value                      # è la mappa dei valori per valutare un id
        self.V = V                                      # è la board completa da riprodurre
        self.max_id = max_id                            # è l'id della board piena
        self.instructions = instructions                # array di istruzioni
        self.patterns = patterns                        # array di patterns
        self.n = len(V)                                 # dimensione V in R^{nxn}
        self.voidMat = voidMat                          # la board vuota (con anche gli spazi di fuori board)
        self.reset()                                    # inizializzare l'id, la current board e gli steps

    # reset, rigenera la board vuota, resetta il current id, resetta gli steps
    def reset(self):
        self.currentMat = np.copy(self.voidMat)         # current board
        self.current_id = 0                             # current id associato alla current board
        self.steps = 0                                  # numero di mosse
        return self.currentMat                          
        
    # funzioni di debugg: Printa la board
    def print_current(self):
        print(self.currentMat)

    # funzioni di debugg: Printa la mossa
    def print_move(self, action):
        node_i, node_j, instruction, lengthOfInst, pattern = action
        print("node:", node_i, node_j, " - instruction:", self.instructions[instruction], " - lenOfInst :", lengthOfInst, " - Pattern :", self.patterns[pattern])

    # funzioni di debugg: Printa tutte le info possibili
    def print_info_state(self, action):
        self.print_move(action)
        self.print_current()
        print("steps: ", self.steps, " - current id:", self.current_id)

    # esegue un azione sulla board
    def execute_instruction(self, action):
        node_i, node_j, instruction, lengthOfInst, pattern = action
        self.current_id, num_new_colored_cells, self.currentMat = executeInstruction(self.current_id, node_i, node_j, instruction, lengthOfInst, pattern, self.V, self.currentMat, self.map_value)
        return num_new_colored_cells
    

    # Singolo step, che esegue un azione
    def step(self, action):

        #come prima cosa si estrapolano le info interessate dall'action
        node_i, node_j, instruction_idx, length, pattern_idx = action
        instruction = self.instructions[instruction_idx]
        pattern = self.patterns[pattern_idx]

        # si esegie l'azione e si calcolano i reward 
        # nota c'è una grande penalizzazione se la colorazione avviene fuori dalla board (mossa non valida)
        if self.V[node_i][node_j] == -1:
            return self.currentMat, -5, self.is_done(),{'current_id': self.current_id}
        
        num_new_colored_cells = self.execute_instruction((node_i, node_j, instruction, length, pattern))
        reward = self.calculate_reward(num_new_colored_cells)
        done = self.is_done()
        next_state = np.copy(self.currentMat)
        self.steps += 1
        info = {'current_id': self.current_id}

        return next_state, reward, done,info


    # reward basato su il premiare quanto riesci a colorare valorizzato da quanto presto sei riusito
    # se la colorazione non è avvenuta penalizza (esempio troppi cancellati, colora fuori dalla board)
    def calculate_reward(self, num_new_colored_cells):
        multiplier = max(2 - self.steps, 1)
        if num_new_colored_cells == 0:
            return -1
        reward = multiplier * num_new_colored_cells
        return reward


    # controllo se è finito il game, cioè se la board è colorata completamente
    def is_done(self):
        return self.current_id >= self.max_id
