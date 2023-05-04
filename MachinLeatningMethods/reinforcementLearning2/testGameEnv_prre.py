from Game.Game_pretrain import GameEnvironmentPreTrain
from Game.Instruction import *
from Tools.fileReader import file_reader



paths = ["./Graph/TestGraph_TEST.txt" ]
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
num_colors = 2
patterns = generate_combinations(num_colors)
env = GameEnvironmentPreTrain(boards, voidMat,max_id, instructions, patterns, num_colors, map_value,n)

action = (2,0,4,1,patterns.index([1, 1, 1, 1])) # Scegli un'azione valida (node_i, node_j, instruction_idx, length, pattern_idx)
next_state, reward, done, _, _ = env.step(action)
env.print_info_state(action)
rew = reward


action = (2,2,4,1,patterns.index([2, 1, 2,2])) # Scegli un'azione valida (node_i, node_j, instruction_idx, length, pattern_idx)
next_state, reward, done, _, _ = env.step(action)
env.print_info_state(action)
rew += reward


action = (2,2,4,1,patterns.index([1,1,1,1])) # Scegli un'azione valida (node_i, node_j, instruction_idx, length, pattern_idx)
next_state, reward, done, _, _ = env.step(action)
env.print_info_state(action)
rew += reward

print(env.current_id,env.max_id )
print(rew)

env.reset()

# python3 -m tensorboard.main --logdir logs --load_fast=false