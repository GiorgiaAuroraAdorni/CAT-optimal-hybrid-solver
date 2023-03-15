# CAT_BFS
Cross Array Task Breadth-First Search

## Run Program
* cd BreadthFirstSearch/
* clang++ -std=c++11 -stdlib=libc++ -g BfsAlgo.cpp -o bfs
* ./bfs

## Description
The program takes an .txt file containing a cat array as input, and prints the number of minimum moves to complete the problem and also print the program to get to the result

## Expectation BFS

Time for a size 9  is 30 secnod
Time for a size 20 is 30 minute only if you modify the code and change input move (you need to put max 3 instructions and 3 patterns)

| Name Graph | Total Number Move |
|------------|------------------|
| minGraph_1 |         2        |
| minGraph_2 |         2        |
| minGraph_3 |         2        |
| minGraph_4 |         2        |
| minGraph_5 |         2        |
| minGraph_6 |         1        |
| TestGraph  |        ...       | 


# CAT_BFS* + GA
Cross Array Task Breadth-First Search Modified with Euristic + Genetic Algo

## Run Program
* cd GeneticAlgo/
* clang++ -std=c++11 -stdlib=libc++ -g GA.cpp -o ga
* ./ga

## Description
The program takes an .txt file containing a cat array as input, and prints the number of minimum moves to complete the problem and also print the program to get to the result


## Expectation Genetic


| Name Graph | Total Number Move |  Total Time    |
|------------|------------------|------------------|
| minGraph_1 |         2        |                 |
| minGraph_2 |         2        |                 |
| minGraph_3 |         2        |                 |
| minGraph_4 |         2        |                 |
| minGraph_5 |         2        |                 |
| minGraph_6 |         1        |                 |
| TestGraph  |         2        | 
