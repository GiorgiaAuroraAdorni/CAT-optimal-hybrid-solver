from Tools.setup_enviroment import *

env = getEnviroment()
print(len(env.instructions))

env.step(24)
env.print_state()