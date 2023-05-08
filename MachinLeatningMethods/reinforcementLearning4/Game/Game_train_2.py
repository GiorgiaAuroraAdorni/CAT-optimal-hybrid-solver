import gymnasium as gym
from gymnasium import spaces
import numpy as np
from .Move_pretrain import executeInstruction

from pylab import *
import random




color_map = {
        -2: (128, 128, 128), # grigio
        -1: (0, 0, 0),       # nero
         0: (255, 255, 255), # bianco
         1: (255, 0, 0),     # rosso
         2: (255, 255, 0),   # giallo
         3: (0, 0, 255),     # blu
         4: (0, 255, 0),     # verde
    }


ZOOM_COEFF = 10

def create_big_matrix(inputMat):
    n = ZOOM_COEFF
    flag_a = 0
    result = []
    for i in range(len(inputMat)):
        flag_b = 0
        new_zoom_column = []
        for j in range(len(inputMat[0])):
            color_index = inputMat[i][j]
            color =  tuple(map(lambda x: np.uint8(x), color_map[color_index]))
            tmpMat = np.full((n, n, 3), color)

            if flag_b == 0:
                new_zoom_column = tmpMat
                flag_b = 1
            else:
                new_zoom_column = np.concatenate((new_zoom_column, tmpMat), axis=1)

        if flag_a == 0:
            result = new_zoom_column
            flag_a = 1
        else:
            result = np.concatenate((result, new_zoom_column), axis=0)
    

    return result












class GameEnvironmentTrain(gym.Env):
    def __init__(self, boards,voidMat, max_id, instructions, patterns, num_colors,map_value, n):
        super().__init__()

        # ciò che vede il ML
        self.observation_space = spaces.Box(low=-2, high=num_colors, shape=(n * n + n*n,), dtype=np.int64)
        #self.observation_space = spaces.Box(low=-2, high=num_colors, shape=(2, n, n), dtype=np.int)

        # lo spazio dell'azione
        self.action_space = spaces.MultiDiscrete([
            len(instructions),                          # instruction_idx
            4,                                          # length della mossa
            len(patterns)                               # pattern_idx
        ])

        self.map_value = map_value                      # è la mappa dei valori per valutare un id
        self.boards = boards                            # il totale delle boards su cui allenarci
        rdm_idx = random.randint(0, len(boards) - 1)
        self.V = self.boards[rdm_idx]                   # è la board completa da riprodurre
        self.max_id = max_id                            # è l'id della board piena
        self.instructions = instructions                # array di istruzioni
        self.patterns = patterns                        # array di patterns
        self.n = len(self.V)                            # dimensione V in R^{nxn}
        self.voidMat = voidMat                          # la board vuota (con anche gli spazi di fuori board)
        self.old_action = (-1, -1,-1,-1,-1)             # old action
        self.reset()                                    # inizializzare l'id, la current board e gli steps
    
    def get_state(self, state_print=None):
        A = np.copy(self.V)
        if state_print is not None:
            B = np.copy(state_print)
        else:
            B = np.copy(self.currentMat)
        A = A.flatten()
        B = B.flatten()
        C = np.concatenate((A, B))
        return C

    # reset, rigenera la board vuota, resetta il current id, resetta gli steps
    def reset(self):
        self.currentMat = np.copy(self.voidMat)         # current board
        self.current_id = 0                             # current id associato alla current board
        self.steps = 0                                  # numero di mosse
        rdm_idx = random.randint(0, len(self.boards) - 1)
        self.V = self.boards[rdm_idx]                   # è la board completa da riprodurre

        state = self.get_state()
        return state, {}                   
        
    # funzioni di debugg: Printa la board
    def print_current(self):
        print(self.currentMat)

    # funzioni di debugg: Printa la mossa
    def print_move(self, action):
        instruction, lengthOfInst, pattern = action
        print("node:", instruction[0], " - instruction:", self.instructions[instruction[1]], " - lenOfInst :", lengthOfInst, " - Pattern :", self.patterns[pattern])

    # funzioni di debugg: Printa tutte le info possibili
    def print_info_state(self, action):
        self.print_move(action)
        self.print_current()
        print("steps: ", self.steps, " - current id:", self.current_id)
    
    def print_state(self):
        A = np.copy(self.V)
        B = np.copy(self.currentMat)
        A = create_big_matrix(A)
        B = create_big_matrix(B)
        C = np.hstack((A, B))
        figure(1)
        plt.imshow(C, interpolation='nearest')
        plt.show()
        
    # esegue un azione sulla board
    def execute_instruction(self, action):
        node_i, node_j, instruction, lengthOfInst, pattern = action
        self.current_id, num_new_colored_cells, self.currentMat, legit_move = executeInstruction(self.current_id, node_i, node_j, instruction, lengthOfInst, pattern, self.V, self.currentMat, self.map_value)
        return num_new_colored_cells
    

    # Singolo step, che esegue un azione
    def step(self, action):

        #come prima cosa si estrapolano le info interessate dall'action
        instruction_idx, length, pattern_idx = action
        instruction_complete = self.instructions[instruction_idx]
        pattern = self.patterns[pattern_idx]
        node_i, node_j = instruction_complete[0]
        instruction = instruction_complete[1]

        # si esegie l'azione e si calcolano i reward 
        # nota c'è una grande penalizzazione se la colorazione avviene fuori dalla board (mossa non valida)
        if self.V[node_i][node_j] == -1:
            state = self.get_state()
            return state, -2, False,False, {'current_id': self.current_id}
        
        num_new_colored_cells = self.execute_instruction((node_i, node_j, instruction, length, pattern))
        reward = self.calculate_reward(num_new_colored_cells)
        done = self.is_done()
        next_state = np.copy(self.currentMat)
        self.steps += 1
        info = {'current_id': self.current_id}

        if done:
            reward += 1
        
        if length == 4:
            reward += 1

        state = self.get_state(state_print=next_state)
        return state, reward, done, False, info


    # reward basato su il premiare quanto riesci a colorare valorizzato da quanto presto sei riusito
    # se la colorazione non è avvenuta penalizza (esempio troppi cancellati, colora fuori dalla board)
    def calculate_reward(self, num_new_colored_cells):
        multiplier = max(4 - self.steps, 0)
        if num_new_colored_cells == 0:
           num_new_colored_cells = -1
        reward =  num_new_colored_cells/4*(1+ multiplier/10)
        return reward


    # controllo se è finito il game, cioè se la board è colorata completamente
    def is_done(self):
        return self.current_id >= self.max_id
