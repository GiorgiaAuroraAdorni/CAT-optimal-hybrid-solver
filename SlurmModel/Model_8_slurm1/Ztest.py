from Tools.setup_enviroment import *
import sys
import os


def main(filenumber):
    filename = "./Graph/TestGraph_"+str(filenumber)+".txt"
    env = getEnviroment(filename, 44)
    env.step(41)
    env.print_state()
    env.step(54)
    env.print_state()

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("Usage: python script.py <filename>")
        sys.exit(1)
        
    filename = sys.argv[1]
    main(filename)