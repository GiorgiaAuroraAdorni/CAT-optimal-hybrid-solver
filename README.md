# CAT Optimal Hybrid Solver

[![DOI](https://zenodo.org/badge/605107045.svg)](https://zenodo.org/badge/latestdoi/605107045)



## Run Program

```shell
  cd ExactMethods/BreadthFirstSearch/
  clang++ -std=c++11 -stdlib=libc++ -g BfsAlgo.cpp -o bfs
  ./bfs NAME_GARPH
```

## Description

The program takes a `.txt` file containing a cat array as input, prints the number of minimum moves to complete the problem and also prints the program to get to the result.

### CAT_BFS* + GA

```shell
  cd MetaMethods/GA2/
  clang++ -std=c++11 -stdlib=libc++ -g GA2.cpp -o ga
  ./ga
```

Note: to change the graph, you must modify the main function in `GA2.cpp`.

### CAT_BFS* + Hybrid approach

```shell
  cd PseudoML/MetaMethods/ExpAlgo2
  clang++ -std=c++11 -stdlib=libc++ -g ExpAlgo.cpp -o exA
  ./exA
```

Note: to change the graph, you must modify the main function in `ExpAlgo.cpp`.

#### Run Reinforcement Learning

```shell
  cd SlurmModel/
  cd chose the model you prefer
```

Look the run file to check fthe ormat of run some need a number to choose file and AI, and other need change run file.

**Example**:

```shell
  cd SlurmModel/Model_8_slurm1
  python3 run_Model.py 4
```

If you need training graph enter it in a folder of an AI and run

```shell
  python3 -m tensorboard.main --logdir logs --load_fast=false
```

