import gymnasium as gym
from gymnasium import spaces
import numpy as np
from .Move import executeInstruction
from pylab import *
import random
import time
import os
import math

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
    def __init__(self, boards,voidMat, max_id, instructions, num_colors,map_value, n,seed):
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
        self.seed = seed
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

        random.seed(seed)
        random_col = random.random()
        if random_col >= 0.5 and random_col <= 0.75:
            inst_random = random.randint(4,len(self.instructions))
            a, b, done, c, d = self.step(inst_random)
            if done:
                self.currentMat = np.copy(self.voidMat)
                self.current_id = 0
        elif random_col > 0.75:
            self.random_update()

        state = self.get_state()
        #debug
        bug = self.debug()
        if bug:
            self.print_info_state()
        
        self.currentMat = np.copy(self.V) 
        self.currentMat[0][2] = 0
        self.current_id = self.max_id
        self.current_id -= 1

        return state, {}   
                    
    def random_update(self):
        r = random.random() 
        for i in range(self.n):
            for j in range(self.n):
                if random.random() > r and self.V[i][j] > 0:
                    self.currentMat[i][j] = self.V[i][j]
                    self.current_id += math.pow(2, self.map_value[i*self.n+j])

    # funzioni di debugg: Printa tutte le info possibili
    def print_info_state(self, action):
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
        self.current_id, num_new_colored_cells, self.currentMat, legit_move = executeInstruction(self.current_id, node_i, node_j, instruction, lengthOfInst, pattern, self.V, self.currentMat, self.map_value, self.max_id)
        return num_new_colored_cells
    
    def debug(self):
        check_id = 0
        bug = False
        for i in range(self.n):
            for j in range(self.n):
                if self.currentMat[i][j] > 0 and self.currentMat[i][j] != self.V[i][j]:
                    bug = True
                    print("wrong Color in: i=", i,"-j =",j)
                if self.currentMat[i][j] > 0:
                    check_id += math.pow(2, self.map_value[i*self.n+j])

        if check_id != self.current_id:
            bug = True
            print("Bug wrong id")
        if self.current_id > self.max_id:
            bug = True
            print("Bug bigger id")
        return bug

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
        if done:
            reward += max(1, 10 - self.steps)

        state = self.get_state(state_print=next_state)
        #debug
        bug = self.debug()
        if bug:
            self.print_info_state()

        return state, reward, done, False, info


    # reward basato su il premiare quanto riesci a colorare valorizzato da quanto presto sei riusito
    # se la colorazione non è avvenuta penalizza (esempio troppi cancellati, colora fuori dalla board)
    def calculate_reward(self, num_new_colored_cells):
        if num_new_colored_cells < 0:
            return -1
        if num_new_colored_cells == 0:
            return -0.5
        return num_new_colored_cells/(self.steps+1)


    # controllo se è finito il game, cioè se la board è colorata completamente
    def is_done(self):
        return self.current_id >= self.max_id
