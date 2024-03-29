import gymnasium as gym
from gymnasium import spaces
import numpy as np
from .Move import executeInstruction
from pylab import *
import random
import time
import os
# ___________________________________
# ___________________________________
# Function to print the enviroment
# ___________________________________
# ___________________________________

color_map = {
        -2: (128, 128, 128), # grigio
        -1: (0, 0, 0),       # nero
         0: (255, 255, 255), # bianco
         1: (255, 0, 0),     # red
         2: (255, 255, 0),   # green
         3: (0, 0, 255),     # blu
         4: (0, 255, 0),     # yellow
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











# ___________________________________
# ___________________________________
# Enviroment
# ___________________________________
# ___________________________________


class GameEnvironmentTrain(gym.Env):
    def __init__(self, boards,voidMat, max_id, instructions, num_colors,map_value, n):
        super().__init__()

        # ciò che vede il ML
        self.observation_space = spaces.Box(low=-2, high=num_colors, shape=(n * n + n*n,), dtype=np.int64)
        #self.observation_space = spaces.Box(low=-2, high=num_colors, shape=(2, n, n), dtype=np.int)

        # lo spazio dell'azione
        self.action_space = spaces.Discrete(len(instructions))

        self.map_value = map_value                      # è la mappa dei valori per valutare un id
        self.boards = boards                            # il totale delle boards su cui allenarci
        rdm_idx = random.randint(0, len(boards) - 1)
        self.V = self.boards[rdm_idx]                   # è la board completa da riprodurre
        self.max_id = max_id                            # è l'id della board piena
        self.instructions = instructions                # array di istruzioni
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

        random.seed(4554)
        rand_num = random.random()
        break_random = 2
        while rand_num > 100:                              # random move 
            random.seed(os.getpid() + int(time.time()))
            rm_action = random.randint(0, len(self.instructions) - 1)
            state, reward, done, asd , info = self.step(rm_action)
            random.seed(self.current_id + int(time.time()))
            rand_num = random.random()
            break_random -=1
            if break_random == 0:
                break
        

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

        nodes,length,instruction,pattern = self.instructions[action]

        tot_colored_cell = 0
        for node in nodes:
            node_i, node_j = node[0], node[1]
            tot_colored_cell += self.execute_instruction((node_i, node_j, instruction, length, pattern))
        
        reward = self.calculate_reward(tot_colored_cell)
        done = self.is_done()
        next_state = np.copy(self.currentMat)
        self.steps += 1
        info = {'current_id': self.current_id}
        print("rew:", reward)
        if done:
            reward += max(1, 10 - self.steps)

        state = self.get_state(state_print=next_state)
        return state, reward, done, False, info


    # reward basato su il premiare quanto riesci a colorare valorizzato da quanto presto sei riusito
    # se la colorazione non è avvenuta penalizza (esempio troppi cancellati, colora fuori dalla board)
    def calculate_reward(self, num_new_colored_cells):
        if num_new_colored_cells < 0:
            return -1
        if num_new_colored_cells == 0:
            return -0.5
        print("colCell = ",num_new_colored_cells)
        return num_new_colored_cells/(self.steps+1)


    # controllo se è finito il game, cioè se la board è colorata completamente
    def is_done(self):
        return self.current_id >= self.max_id
