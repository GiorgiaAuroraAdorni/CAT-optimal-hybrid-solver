from Tools.fileReader import file_reader 
from Game.ValidationMove import *
from Game.Game import GameEnvironmentTrain
from Game.Instruction import *


def get_key(input_arr, len):
    res = ""
    for s in input_arr:
        res += str(s)
    res += str(len)
    return res

def correctPatt(patt):
    idx = 0
    while len(patt) < 4:
        patt.append(patt[idx])
    return patt

def getEnviroment(filename):
    paths = [filename]
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

    instructions = TOT_istructions
    num_colors = 4    

    
    result = [[[[0,0]], 2, [3,0], [0,0,0,0]],[[[0,0]], 2, [3,1], [0,0,0,0]],[[[0,0]], 2, [3,2], [0,0,0,0]],[[[0,0]], 2, [3,3], [0,0,0,0]]] #First move are the possible mirror

    for inst in range(len(instructions)):
        fake_hash = []
        for i in range(n):
            for j in range(n):
                for k in range(n): 
                    if instructions[inst][0] == 2:
                        k = n-1
                    patt = checkInstAndgetPatt(i, j, instructions[inst], (k+1), V)
                    if patt and k == 0:
                        result.append([[[i,j]],(k+1),instructions[inst],patt])
                    if patt and k > 0:
                        patt = correctPatt(patt)
                        patt_key = get_key(patt, k)
                        if patt_key in fake_hash:
                            continue
                        fake_hash.append(patt_key)

                        node = []
                        if k > 0:
                            for a in range(n):
                                for b in range(n):
                                    flag_copy = checkCopyValidity(a, b, instructions[inst], (k+1),patt, V)
                                    if flag_copy:
                                        node.append([a,b])
                        print([node,(k+1),instructions[inst],patt])
                        result.append([node,(k+1),instructions[inst],patt])

    
    instructions = result
    env = GameEnvironmentTrain(boards, voidMat,max_id, instructions, num_colors, map_value,n)
    return env
