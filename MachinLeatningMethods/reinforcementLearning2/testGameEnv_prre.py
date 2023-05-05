from Game.Game_pretrain import GameEnvironmentPreTrain
from Game.Instruction import *
from Tools.fileReader import file_reader
from Game.Game_pretrain_curriculum import GameEnvironmentPreTrainCurriculum



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
env = GameEnvironmentPreTrainCurriculum(boards, voidMat,max_id, instructions, patterns, num_colors, map_value,n)

#patterns.index([1, 1, 1, 1])
action = (1,2,2,0,16) # Scegli un'azione valida (node_i, node_j, instruction_idx, length, pattern_idx)
next_state, reward, done, _, _ = env.step(action)
env.print_info_state(action)
rew = reward

action = (2,1,0,0,172) # Scegli un'azione valida (node_i, node_j, instruction_idx, length, pattern_idx)
next_state, reward, done, _, _ = env.step(action)
env.print_info_state(action)
rew += reward


print(env.current_id,env.max_id )
print(rew)

env.reset()

# python3 -m tensorboard.main --logdir logs --load_fast=false