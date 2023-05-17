import numpy as np
import copy
import math


class Instruction:
    def __init__(self):
        self.instruction = []
        self.pattern = []
        self.len = 0

WRONG_COLOR = -2




def get_mirror_vertical_left(old_id, res_mat, curr_mat, value_index):
    res = []
    new_id = old_id
    n = len(res_mat)
    m = n - 1
    color = 0
    for i in range(n):
        for j in range(n//2):
            if curr_mat[i][j] == -1 or res_mat[i][m-j] == -1:
                continue
            if res_mat[i][m-j] == curr_mat[i][j]:
                if curr_mat[i][m-j] == 0 or curr_mat[i][m-j] == WRONG_COLOR:
                    new_id += math.pow(2, value_index[i*n+(m-j)])
                    color += 1
                curr_mat[i][m-j] = curr_mat[i][j]
                res.append([i,j])
    return new_id,color, curr_mat, True

def get_mirror_vertical_right(old_id, res_mat, curr_mat, value_index):
    res = []
    new_id = old_id
    n = len(res_mat)
    m = n - 1
    color = 0
    for i in range(n):
        for j in range(n//2):
            if res_mat[i][j] == -1 or curr_mat[i][m-j] == -1:
                continue
            if res_mat[i][j] == curr_mat[i][m-j]:
                if curr_mat[i][j] == 0 or curr_mat[i][j] == WRONG_COLOR:
                    new_id += math.pow(2, value_index[i*n+j])
                    color += 1
                curr_mat[i][j] = curr_mat[i][m-j]
                res.append([i,(m-j)])
    return new_id,color, curr_mat, True

def get_mirror_horizontal_top(old_id, res_mat, curr_mat, value_index):
    res = []
    new_id = old_id
    n = len(res_mat)
    m = n - 1
    color = 0
    for i in range(n//2):
        for j in range(n):
            if curr_mat[i][j] == -1 or res_mat[m-i][j] == -1:
                continue
            if res_mat[m-i][j] == curr_mat[i][j]:
                if curr_mat[m-i][j] == 0 or curr_mat[m-i][j] == WRONG_COLOR:
                    new_id += math.pow(2, value_index[(m-i)*n+j])
                    color += 1
                curr_mat[m-i][j] = curr_mat[i][j]
                res.append([i,j])
    return new_id,color, curr_mat, True

def get_mirror_horizontal_down(old_id, res_mat, curr_mat, value_index):
    res = []
    new_id = old_id
    n = len(res_mat)
    m = n - 1
    color = 0
    for i in range(n//2):
        for j in range(n):
            if res_mat[i][j] == -1 or curr_mat[m-i][j] == -1:
                continue
            if curr_mat[m-i][j] == res_mat[i][j]:
                if curr_mat[i][j] == 0 or curr_mat[i][j] == WRONG_COLOR:
                    new_id += math.pow(2, value_index[i*n+j])
                    color += 1
                curr_mat[i][j] = curr_mat[m-i][j]
                res.append([(m-i),j])
    return new_id,color, curr_mat, True






def moveMirror(type_mirror, id, mat, actualRes, value_index):
    if type_mirror == 0:
        return get_mirror_vertical_left(id, mat, actualRes, value_index)
    elif type_mirror == 1:
        return get_mirror_vertical_right(id, mat, actualRes, value_index)
    elif type_mirror == 2:
        return get_mirror_horizontal_top(id, mat, actualRes, value_index)
    elif type_mirror == 3:
        return get_mirror_horizontal_down(id, mat, actualRes, value_index)
    return 1


def fill(mat, actualRes, id, max_id):
    tot_color = 0
    color = 0
    first = True
    for i in range(len(actualRes)):
        for j in range(len(actualRes[i])):

            if actualRes[i][j] == 0 and first:
                color = mat[i][j]
                first = False

            if actualRes[i][j] == 0 and not(first) and color != mat[i][j]:
                return id,0, actualRes, False
            
            if actualRes[i][j] == 0 and not(first) and color == mat[i][j]:
                tot_color += 1

    return max_id,tot_color, np.copy(mat), True

    



#________________
#________________
#___ EX MOVE ___
#________________
#________________



def executeInstruction(id, node_i, node_j, instruction_input, lengthOfInst, pattern, mat, actualRes, value_index, max_id):
    
    tmp_input_mat = copy.deepcopy(actualRes)
    tmp_id_imput = id

    instruction = instruction_input.copy()
    n = len(mat)
    idx_pattern = 0
    idx_istruct = 0
    i = node_i
    j = node_j

    if instruction[0] == 3:
        return moveMirror(instruction[1], id, mat, actualRes, value_index)
    if instruction[0] == 4:
        return fill(mat, actualRes, id,max_id)
    if mat[i][j] == -1:
        return id,0, tmp_input_mat, False
    

    min_to_color = 0
    type_of_inst = instruction[idx_istruct]
    instruction.pop(0)

    if type_of_inst == 2:
        lengthOfInst = len(instruction) // 2 + 1
    
    

    while True:
        if actualRes[i][j] != mat[i][j] and pattern[idx_pattern] == mat[i][j]:
            id = id + 2**value_index[i*n+j]
            min_to_color += 1

        if mat[i][j] == pattern[idx_pattern]:
            actualRes[i][j] = pattern[idx_pattern]
        else:
            actualRes = copy.deepcopy(tmp_input_mat)
            return tmp_id_imput, 0, actualRes, False

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
            return tmp_id_imput, 0, actualRes, False

    return id, min_to_color, actualRes, True

import math

