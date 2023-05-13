from Game.Game_train import GameEnvironmentTrain
from Game.Instruction import *
from Tools.fileReader import file_reader
from Game.Move_checkValidPOsition import *

import numpy as np


paths = ["./Graph/TestGraph_TEST_4COL.txt" ]
boards = [] 
n = 0
for path in paths:
    V = file_reader(path)
    n = len(V)
    boards.append(V)


map_value = [-1] * (n ** 2)
total_colored = 0
tmp_iter_val = 0
voidMat = [[0] * n for _ in range(n)]

for i in range(n):
    for j in range(n):
        if V[i][j] < 0:
            voidMat[i][j] = V[i][j]
        else:
            total_colored += 1
            map_value[i*n+j] = tmp_iter_val
            tmp_iter_val += 1

max_id = 0
for i in range(total_colored):
    max_id += 2**i

instructions = TOT_istructions_test
num_colors = 4
patterns = generate_combinations(num_colors)



result = []

for inst in range(len(instructions)):
    for i in range(n):
        for j in range(n):
            for k in range(n):
                patt = checkInstAndgetPatt(i, j, instructions[inst], (k+1), V)
                if patt:
                    result.append([[i,j],(k+1),instructions[inst],patt])



        
instructions = result
print(len(instructions))
print(instructions[0])

env = GameEnvironmentTrain(boards, voidMat,max_id, instructions, num_colors, map_value,n)


action = (45) # Scegli un'azione valida (node_i, node_j, instruction_idx, length, pattern_idx)
next_state, reward, done, _, _ = env.step(action)
rew = reward
env.print_state()

env.reset()

# python3 -m tensorboard.main --logdir logs --load_fast=false