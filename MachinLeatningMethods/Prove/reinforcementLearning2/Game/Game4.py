import os
import sys
import gym
from gym import spaces
import numpy as np



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

def matrix_to_image(matrix):
    # Creare una mappa di colore per convertire i valori della matrice nei rispettivi colori.
    

    # Creare un'immagine vuota con le stesse dimensioni della matrice.
    height, width = matrix.shape
    image = Image.new("RGB", (width, height))

    # Popolare l'immagine con i colori corrispondenti ai valori della matrice.
    for i in range(height):
        for j in range(width):
            color = color_map[matrix[i, j]]
            image.putpixel((j, i), color)

    return image



def create_image2(n , matrix1, matrix2, color_map):

    # Crea un'immagine vuota
    img = Image.new('RGBA', (n*2+1, n+1))

    # Crea un'array numpy per l'immagine trasparente
    alpha = np.zeros((n, n*2), dtype=np.uint8)
    alpha[:,n-1:n+1] = 255

    # Popola l'immagine con i colori corrispondenti alla matrice 1
    for i in range(n):
        for j in range(n):
            color = color_map[matrix1[i][j]]
            img.putpixel((j, i), color)

    for i in range(n):
            color = (255, 0, 255)
            img.putpixel((n, i), color)
    

    # Popola l'immagine con i colori corrispondenti alla matrice 2
    for i in range(n):
        for j in range(n):
            color = color_map[matrix2[i][j]]
            img.putpixel((j+n+1, i), color)



    return img


def create_image(n, matrix1, matrix2, color_map):
    if n+1 < 36:
        scale_factor = int(np.ceil(36 / (n + 1)))+1
    else:
        scale_factor = 1
    
    img = Image.new('RGBA', (scale_factor * (n * 2 + 1), scale_factor * (n + 1)))

    for i in range(n):
        for j in range(n):
            color = color_map[matrix1[i][j]]
            for si in range(scale_factor):
                for sj in range(scale_factor):
                    img.putpixel((j * scale_factor + sj, i * scale_factor + si), color)

   
    for i in range(n):
        for j in range(n):
            color = color_map[matrix2[i][j]]
            for si in range(scale_factor):
                for sj in range(scale_factor):
                    img.putpixel(((j + n + 1) * scale_factor + sj, i * scale_factor + si), color)

    return img

from PIL import Image
import numpy as np

def create_image2(matrix1, matrix2, color_map):
    n = matrix1.shape[0]

    # Crea un'immagine vuota
    img = Image.new('RGBA', (n*2, n+1))

    # Crea un'array numpy per l'immagine trasparente
    alpha = np.zeros((n, n*2), dtype=np.uint8)
    alpha[:,n-1:n+1] = 255

    # Popola l'immagine con i colori corrispondenti alla matrice 1
    for i in range(n):
        for j in range(n):
            color = color_map[matrix1[i][j]]
            img.putpixel((j, i), color)

    # Popola l'immagine con i colori corrispondenti alla matrice 2
    for i in range(n):
        for j in range(n):
            color = color_map[matrix2[i][j]]
            img.putpixel((j+n, i), color)

    # Sovrappone l'immagine trasparente alle due matrici
    img.putalpha(Image.fromarray(alpha))

    return img


# Aggiungi il percorso del pacchetto padre alla variabile di ambiente PYTHONPATH
parent_path = os.path.abspath(os.path.join(os.getcwd(), ".."))
sys.path.append(parent_path)

# Importa la funzione file_reader dal modulo fileReader
from .Move import executeInstruction


class GameEnvironment(gym.Env):
    def __init__(self, V,voidMat, max_id, instructions, patterns, num_colors,map_value, n):
        super().__init__()

        # ciò che vede il ML
        #self.observation_space = spaces.Box(low=0, high=255, shape=(n, n, 3), dtype=np.uint8)
        #self.observation_space = spaces.Box(low=-2, high=num_colors, shape=(2, n, n), dtype=np.int)

        if n + 1 < 36:
            scale_factor = int(np.ceil(36 / (n + 1))) + 1
        else:
            scale_factor = 1

        # Calcola le nuove dimensioni dell'immagine
        scaled_width = scale_factor * (n * 2 + 1)
        scaled_height = scale_factor * (n + 1)

        # Definisci lo spazio delle osservazioni
        self.observation_space = spaces.Box(low=0, high=255, shape=(scaled_height, scaled_width, 4), dtype=np.uint8)

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

        next_state = np.array(create_image(self.n , self.V, self.currentMat, color_map))
        return next_state   
        
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
    
    def print_state(self):
        image_curr = create_image(self.n , self.V, self.currentMat, color_map)
        image_curr.show()

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
            next_state = create_image(self.n , self.V, self.currentMat, color_map)   
            next_state = np.array(next_state)
            return next_state, -4, self.is_done(),{'current_id': self.current_id}
        
        num_new_colored_cells = self.execute_instruction((node_i, node_j, instruction, length, pattern))
        reward = self.calculate_reward(num_new_colored_cells)
        done = self.is_done()
        next_state = np.copy(self.currentMat)
        self.steps += 1
        info = {'current_id': self.current_id}

        next_state = create_image(self.n , self.V, next_state, color_map)   
        next_state = np.array(next_state)
        return next_state, reward, done,info


    # reward basato su il premiare quanto riesci a colorare valorizzato da quanto presto sei riusito
    # se la colorazione non è avvenuta penalizza (esempio troppi cancellati, colora fuori dalla board)
    def calculate_reward(self, num_new_colored_cells):
        multiplier = max(4 - self.steps, 1)
        if num_new_colored_cells == 0:
           num_new_colored_cells = -2
        reward = multiplier * num_new_colored_cells
        return reward


    # controllo se è finito il game, cioè se la board è colorata completamente
    def is_done(self):
        return self.current_id >= self.max_id
