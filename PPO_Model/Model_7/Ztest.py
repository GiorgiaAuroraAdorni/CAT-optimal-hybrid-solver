from Tools.setup_enviroment import *
import sys
import os


def main(filenumber):
    filename = "./Graph/TestGraph_"+str(filenumber)+".txt"
    env = getEnviroment(filename)

    iterAA = 10
    while iterAA > 0:
        env = getEnviroment(filename)
        env.print_state()
        print(env.current_id)
        iterAA -=1

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("Usage: python script.py <filename>")
        sys.exit(1)
        
    filename = sys.argv[1]
    main(filename)