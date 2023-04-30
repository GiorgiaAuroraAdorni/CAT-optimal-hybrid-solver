import os
import sys
import gymnasium as gym
from gymnasium import spaces
import numpy as np
from pylab import *
from PIL import Image


# Aggiungi il percorso del pacchetto padre alla variabile di ambiente PYTHONPATH
parent_path = os.path.abspath(os.path.join(os.getcwd(), ".."))
sys.path.append(parent_path)

# Importa la funzione file_reader dal modulo fileReader
from .Move import executeInstruction



from PIL import Image

color_map = {
        -2: (128, 128, 128), # grigio
        -1: (0, 0, 0),       # nero
         0: (255, 255, 255), # bianco
         1: (255, 0, 0),     # rosso
         2: (255, 255, 0),   # giallo
         3: (0, 0, 255),     # blu
         4: (0, 255, 0),     # verde
    }


ZOOM_COEFF = 1

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


class GameEnvironment(gym.Env):
    def __init__(self, V,voidMat, max_id, instructions, patterns, num_colors,map_value, n):
        super().__init__()

        # ciò che vede il ML
        #self.observation_space = spaces.Box(low=-2, high=num_colors, shape=(2, n, n), dtype=np.int)
        self.observation_space = spaces.Box(low=0, high=255, shape=(ZOOM_COEFF * n, 2 * ZOOM_COEFF * n, 3), dtype=np.uint8)


        # lo spazio dell'azione
        self.action_space = spaces.MultiDiscrete([
            n,                                          # node_i
            n,                                          # node_j
            len(instructions),                          # instruction_idx
            4,                                          # length della mossa
            num_colors,                                 # pattern 1
            num_colors,                                 # pattern 2
            num_colors,                                 # pattern 3
            num_colors,                                 # pattern 4
        ])

        self.old_action = [0,0,0,0,0,0]

        self.map_value = map_value                      # è la mappa dei valori per valutare un id
        self.V = V                                      # è la board completa da riprodurre
        self.max_id = max_id                            # è l'id della board piena
        self.instructions = instructions                # array di istruzioni
        self.patterns = patterns                        # array di patterns
        self.n = len(V)                                 # dimensione V in R^{nxn}
        self.voidMat = voidMat                          # la board vuota (con anche gli spazi di fuori board)
        self.reset()                                    # inizializzare l'id, la current board e gli steps

    # genera l'immagine dello stato
    def get_state(self, state_print=None):
        A = np.copy(self.V)
        if state_print is not None:
            B = np.copy(state_print)
        else:
            B = np.copy(self.currentMat)
        A = create_big_matrix(A)
        B = create_big_matrix(B)
        C = np.hstack((A, B))
        return C
    
    # reset, rigenera la board vuota, resetta il current id, resetta gli steps
    def reset(self):
        self.currentMat = np.copy(self.voidMat)         # current board
        self.current_id = 0                             # current id associato alla current board
        self.steps = 0                                  # numero di mosse
        state = self.get_state()
        return state, {}                       
        
    # funzioni di debugg: Printa la board
    def print_current(self):
        print(self.currentMat)

    # funzioni di debugg: Printa la mossa
    def print_move(self, action):
        node_i, node_j, instruction, lengthOfInst, a,b,c,d = action
        a = a+1
        b = b+1
        c = c+1
        d = d+1
        lengthOfInst += 1
        print("node:", node_i, node_j, " - instruction:", self.instructions[instruction], " - lenOfInst :", lengthOfInst, " - Pattern :", (a,b,c,d))

    # funzioni di debugg: Printa tutte le info possibili
    def print_info_state(self, action):
        self.print_move(action)
        self.print_current()
        print("steps: ", self.steps, " - current id:", self.current_id)

    def print_state(self):
        C = self.get_state()
        figure(1)
        plt.imshow(C, interpolation='nearest')
        plt.show()
        
    # esegue un azione sulla board
    def execute_instruction(self, action):
        node_i, node_j, instruction, lengthOfInst, pattern = action
        self.current_id, num_new_colored_cells, self.currentMat, legit_move = executeInstruction(self.current_id, node_i, node_j, instruction, lengthOfInst, pattern, self.V, self.currentMat, self.map_value)
        return num_new_colored_cells, legit_move
    

    # Singolo step, che esegue un azione
    def step(self, action):
        self.steps += 1
        # if(self.steps > 10):
            # state = self.get_state()
            # return state, -1, True,True, {'current_id': self.current_id}

        #come prima cosa si estrapolano le info interessate dall'action
        node_i, node_j, instruction_idx, length, a,b,c,d = action
        a = a+1
        b = b+1
        c = c+1
        d = d+1
        length = length + 1
        instruction = self.instructions[instruction_idx]
        pattern = (a,b,c,d)

        # si esegie l'azione e si calcolano i reward 
        # nota c'è una grande penalizzazione se la colorazione avviene fuori dalla board (mossa non valida)
        if self.V[node_i][node_j] == -1:
            state = self.get_state()
            self.old_action = action
            return state, -10, False,False, {'current_id': self.current_id}
        
        num_new_colored_cells, legit_move = self.execute_instruction((node_i, node_j, instruction, length, pattern))
        
        reward = self.calculate_reward(num_new_colored_cells, legit_move,action)
        done = self.is_done()
        next_state = np.copy(self.currentMat)
        info = {'current_id': self.current_id}

        self.old_action = action

        state = self.get_state(state_print=next_state)
        return state, reward, done, False, info


    # reward basato su il premiare quanto riesci a colorare valorizzato da quanto presto sei riusito
    # se la colorazione non è avvenuta penalizza (esempio troppi cancellati, colora fuori dalla board)
    def calculate_reward(self, num_new_colored_cells, legit_move, action):
        reward = num_new_colored_cells
        node_i, node_j, instruction_idx, length, a,b,c,d = action
        #dai vantaggi più lunga

        if self.is_done():
            return 100/self.steps
        
        
        return 0


    # controllo se è finito il game, cioè se la board è colorata completamente
    def is_done(self):
        return self.current_id >= self.max_id

