import numpy as np

from Game.Game import GameEnvironment
from Game.Instruction import *
from Tools.fileReader import file_reader



def cerca_array(array_da_cercare, array_di_array):
    for i, array in enumerate(array_di_array):
        if array == array_da_cercare:
            return i
    return -1

path = "./Graph/TestGraph_6.txt"
# Inizializza le variabili necessarie per il tuo ambiente di gioco
V = file_reader(path)
n = len(V)

instructions = TOT_istructions_2
patterns = generate_combinations(4)
num_colors = 4


# Create Base case with the void table
# in this step we can save the number of colored value
map_value = [-1] * (n ** 2)
total_colored = 0
tmp_iter_val = 0
voidMat = [[0] * n for _ in range(n)]

print("ciao2\n")
for i in range(n):
    for j in range(n):
        if V[i][j] < 0:
            voidMat[i][j] = V[i][j]
        else:
            total_colored += 1
            map_value[i*n+j] = tmp_iter_val
            tmp_iter_val += 1

# with the information we can cmpute the dimension of the max id of a state (for the full colored graph)
# And we can also initialize the memory vector
max_id = 0
for i in range(total_colored):
    max_id += 2**i


# Crea un'istanza del tuo ambiente di gioco
game_env = GameEnvironment(V, voidMat,max_id, instructions, patterns, num_colors, map_value,n)


# Esegui alcune mosse manualmente
state = game_env.reset()
#print("Stato iniziale:")
#print(np.array(state))


if 0 :
    tmp_mat = np.copy(voidMat)
    executeInstruction(0, 3, 3, [0,0,1], 1, [1], V, tmp_mat, map_value)
    print(tmp_mat)


if 1 :
    action = (3, 3, 0, 3, 0)  # Scegli un'azione valida (node_i, node_j, instruction_idx, length, pattern_idx)
    next_state, reward, done, _ = game_env.step(action)
    game_env.print_info_state(action)

    action = (3, 3, 1, 3, 0)  # Scegli un'azione valida (node_i, node_j, instruction_idx, length, pattern_idx)
    next_state, reward, done, _ = game_env.step(action)
    game_env.print_info_state(action)


    action = (3, 0, 0, 3, 1)  # Scegli un'azione valida (node_i, node_j, instruction_idx, length, pattern_idx)
    next_state, reward, done, _ = game_env.step(action)
    game_env.print_info_state(action)

    action = (3, 2, 1, 3, 1)  # Scegli un'azione valida (node_i, node_j, instruction_idx, length, pattern_idx)
    next_state, reward, done, _ = game_env.step(action)
    game_env.print_info_state(action)

    action = (2, 0, 0, 3, 3)  # Scegli un'azione valida (node_i, node_j, instruction_idx, length, pattern_idx)
    next_state, reward, done, _ = game_env.step(action)
    game_env.print_info_state(action)

    action = (0, 2, 1, 3, 3)  # Scegli un'azione valida (node_i, node_j, instruction_idx, length, pattern_idx)
    next_state, reward, done, _ = game_env.step(action)
    game_env.print_info_state(action)


    action = (2, 3, 0, 3, 2)  # Scegli un'azione valida (node_i, node_j, instruction_idx, length, pattern_idx)
    next_state, reward, done, _ = game_env.step(action)
    game_env.print_info_state(action)

    action = (0, 3, 1, 3, 2)  # Scegli un'azione valida (node_i, node_j, instruction_idx, length, pattern_idx)
    next_state, reward, done, _ = game_env.step(action)
    game_env.print_info_state(action)

    print(game_env.is_done())

    game_env.reset()
    game_env.print_info_state(action)


    action = (2, 2, 4, 3, cerca_array([2,1,3,4], game_env.patterns))  # Scegli un'azione valida (node_i, node_j, instruction_idx, length, pattern_idx)
    next_state, reward, done, _ = game_env.step(action)
    action = (3, 2, 4, 3, cerca_array([2,1,3,4], game_env.patterns))  # Scegli un'azione valida (node_i, node_j, instruction_idx, length, pattern_idx)
    next_state, reward, done, _ = game_env.step(action)

    game_env.print_info_state(action)
    



# Esegui altre azioni se lo desideri
#action = (1, 1, 1, 2, 1)  # Un'altra azione valida
#next_state, reward, done, _ = game_env.step(action)

#print("Stato dopo l'azione 2:")
#print(np.array(next_state))