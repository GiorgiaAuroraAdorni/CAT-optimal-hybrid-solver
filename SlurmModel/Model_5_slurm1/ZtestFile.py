from Tools.setup_enviroment import *

env = getEnviroment("./Graph/TestGraph_5.txt")
print(len(env.instructions))
env.currentMat[3][2] = env.V[3][2]
env.currentMat[3][3] = env.V[3][3]
env.print_state()
env.step(0)
env.print_state()
env.step(1)
env.print_state()
env.step(2)
env.print_state()
env.step(3)
env.print_state()