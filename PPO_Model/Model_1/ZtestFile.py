from Tools.setup_enviroment import *

env = getEnviroment("./Graph/TestGraph_8.txt")
print(len(env.instructions))

env.step(24)
env.print_state()