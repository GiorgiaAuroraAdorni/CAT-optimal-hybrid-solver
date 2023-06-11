# CAT_BFS
Cross Array Task Breadth-First Search

## Run Program
* cd ExactMethods/BreadthFirstSearch/
* clang++ -std=c++11 -stdlib=libc++ -g BfsAlgo.cpp -o bfs
* ./bfs NAME_GARPH

## Description
The program takes an .txt file containing a cat array as input, and prints the number of minimum moves to complete the problem and also print the program to get to the result

# CAT_BFS* + GA
* cd MetaMethods/GA2/
* clang++ -std=c++11 -stdlib=libc++ -g GA2.cpp -o ga
* ./ga

note for change graph you must modify main function in GA2.cpp

# CAT_BFS* + Hybrid approach

## Run Program
* cd PseudoML/MetaMethods/ExpAlgo2
* clang++ -std=c++11 -stdlib=libc++ -g ExpAlgo.cpp -o exA
* ./exA

note for change graph you must modify main function in ExpAlgo.cpp

## Run Reinforcement Learning
* cd SlurmModel/
* cd chose the model you prefer
* look the run file to check format of run some need a number to chose file and AI and other need change run file

Example 
cd SlurmModel/Model_8_slurm1
python3 run_Model.py 4

if you need training graph enter in a folder of an Ai and run
python3 -m tensorboard.main --logdir logs --load_fast=false
