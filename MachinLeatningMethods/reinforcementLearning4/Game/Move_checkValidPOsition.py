import numpy as np
import copy

class Instruction:
    def __init__(self):
        self.instruction = []
        self.pattern = []
        self.len = 0

WRONG_COLOR = -2

def executeInstruction(node_i, node_j, instruction_input, lengthOfInst, mat):

    instruction = instruction_input.copy()

    idx_istruct = 0
    i = node_i
    j = node_j
    if mat[i][j] == -1:
        return False
    

    type_of_inst = instruction[idx_istruct]
    instruction.pop(0)

    if type_of_inst == 2:
        lengthOfInst = len(instruction) // 2 + 1
    
    

    while True:
        if mat[i][j] == -1:
            return False
        i += instruction[idx_istruct]
        idx_istruct += 1
        j += instruction[idx_istruct]
        idx_istruct += 1

        if idx_istruct >= len(instruction):
            idx_istruct = 0

        lengthOfInst -= 1
        if lengthOfInst == 0:
            break

        if i >= len(mat) or i < 0 or j >= len(mat) or j < 0 or mat[i][j] == -1:
            return False

    return True
