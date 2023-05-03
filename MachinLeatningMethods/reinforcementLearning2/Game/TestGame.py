
import os
import sys

# Aggiungi il percorso del pacchetto padre alla variabile di ambiente PYTHONPATH
parent_path = os.path.abspath(os.path.join(os.getcwd(), ".."))
sys.path.append(parent_path)

# Importa la funzione file_reader dal modulo fileReader
from Tools.fileReader import file_reader
from Move import *


TOT_istructions = [
[0, 0, 1],
[0, 1, 0],
[0, -1, 1],
[0, -1, -1],
[2, 0, 1, -1, 0, 0, -1],
[2, -1, 0, -1, 0, 0, -1, 0, -1],
[2, -1, 0, -1, 0, 0, 1, 0, 1],
[2, 0, -1, 0, -1, -1, 0, -1, 0],
[2, 0, 1, 0, 1, -1, 0, -1, 0],
[0, -1, 1, 1, 1],
[0, 1, 1, -1, 1],
[1, 1, 1, 1, -1],
[1, 1, -1, 1, 1]
]


def print_void_mat(voidMat):
    for i in range(len(voidMat)):
        for j in range(len(voidMat[i])):
            print(voidMat[i][j], end=' ')
        print()


path = "../Graph/TestGraph_6.txt"

# read file and convert information into a matrix
V = file_reader(path)
n = len(V)

# Create Base case with the void table
# in this step we can save the number of colored value
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

# with the information we can cmpute the dimension of the max id of a state (for the full colored graph)
# And we can also initialize the memory vector
max_id = 0
for i in range(total_colored):
    max_id += 2**i

size = 2**total_colored
memory = [-1] * size
memory[max_id] = 0 


end_id = executeInstruction(0, 2, 0,[0, 0, 1],  2, [4], V, voidMat,map_value)


print_void_mat(voidMat)
