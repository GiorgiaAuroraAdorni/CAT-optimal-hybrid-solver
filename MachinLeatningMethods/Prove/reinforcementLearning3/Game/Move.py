import numpy as np
import copy

class Instruction:
    def __init__(self):
        self.instruction = []
        self.pattern = []
        self.len = 0

WRONG_COLOR = -2

def executeInstruction(id, node_i, node_j, instruction_input, lengthOfInst, pattern, mat, actualRes, value_index):
    
    tmp_input_mat = copy.deepcopy(actualRes)
    tmp_id_imput = id

    instruction = instruction_input.copy()

    n = len(mat)
    idx_pattern = 0
    idx_istruct = 0
    i = node_i
    j = node_j
    if mat[i][j] == -1:
        return id,0, tmp_input_mat,0
    

    min_to_color = 0
    type_of_inst = instruction[idx_istruct]
    instruction.pop(0)

    if type_of_inst == 2:
        lengthOfInst = len(instruction) // 2 + 1
    
    

    while True:
        if actualRes[i][j] == mat[i][j] and pattern[idx_pattern] != mat[i][j]:
            id = id - 2**value_index[i*n+j]
            min_to_color -= 1

        if actualRes[i][j] != mat[i][j] and pattern[idx_pattern] == mat[i][j]:
            id = id + 2**value_index[i*n+j]
            min_to_color += 1

        if mat[i][j] == pattern[idx_pattern]:
            actualRes[i][j] = pattern[idx_pattern]
        else:
            actualRes[i][j] = WRONG_COLOR

        idx_pattern += 1
        if idx_pattern >= len(pattern):
            idx_pattern = 0

        i += instruction[idx_istruct]
        idx_istruct += 1
        #print(idx_istruct, len(instruction), "\n")
        j += instruction[idx_istruct]
        idx_istruct += 1

        if idx_istruct >= len(instruction):
            idx_istruct = 0

        lengthOfInst -= 1
        if lengthOfInst == 0:
            break

        if i >= len(mat) or i < 0 or j >= len(mat) or j < 0 or mat[i][j] == -1:
            actualRes = copy.deepcopy(tmp_input_mat)
            return tmp_id_imput, 0, tmp_input_mat, 0

    #if min_to_color <= 0:
        #actualRes = copy.deepcopy(tmp_input_mat)
        #return tmp_id_imput, 0, actualRes
    return id, min_to_color, actualRes, 1
