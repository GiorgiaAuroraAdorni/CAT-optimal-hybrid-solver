from Tools.setup_enviroment import *
import sys
import os
import math

def main(filenumber):
    filename = "./Graph/TestGraph_"+str(filenumber)+".txt"
    env = getEnviroment(filename, 2144)
    env.print_state()
    env.debug()

    env.step(0)
    env.print_state()
    env.debug()

    for i in range(6):
        for j in range(6):
            if env.V[i][j] == 4 and env.currentMat[i][j] == 0:
                env.currentMat[i][j] = 4
                env.current_id += math.pow(2, env.map_value[i*6+j])
    
    env.print_state()
    env.debug()

    a,b,c,d,e = env.step(0)
    print(a,b,c,d,e)
    env.print_state()
    env.debug()
    

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("Usage: python script.py <filename>")
        sys.exit(1)
        
    filename = sys.argv[1]
    main(filename)