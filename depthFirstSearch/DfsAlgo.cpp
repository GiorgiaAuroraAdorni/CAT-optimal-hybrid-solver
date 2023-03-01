#include <fstream>
#include <iostream>
#include <string>
#include <random>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <cmath>
#include <limits>

#include "./FileReader.hpp"
#include "./Move.hpp"


void printArray( std::vector<std::vector<int>> mat){
    for(int i = 0; i < mat.size(); ++i){
        for(int j = 0; j < mat.size(); ++j){
            if(mat[i][j] >= 0){
                printf(" %d ", mat[i][j]);
            }else{
                printf("%d ", mat[i][j]);
            }
        }
        printf("\n");
    }
}


std::vector<std::vector<int>> istructions = {{0,0,1},{0,0,1},{0,1,0},{0,1,0},{0,-1,1},{0,-1,-1},{2,0,1,-1,0,0,-1},{2,-1,0,-1,0,0,-1,0,-1},{2,-1,0,-1,0,0,1,0,1}, {2,0,-1,0,-1,-1,0,-1,0},{2,0,1,0,1,-1,0,-1,0},{0,-1,1,1,1},{0,1,1,-1,1},{1,1,1,1,-1},{1,1,-1,1,1}};
std::vector<std::vector<int>> patterns = {{4},{3},{4,3},{4,4,3},{4,3,3,3},{3,4}};

int dfs_algo(int id, std::vector<int> & memory, int n, int m, std::vector<std::vector<int>> & current_state, std::vector<std::vector<int>> * end_solution, int n_iter, std::vector<int> & value_index){
    if(memory[id] != -1){                               // BASE CASE --> We have alsa visited this case
        return memory[id];                              // return the value to reach the id-th state
    }else{                                              // ELSE CASE --> We need to compute the move to reach the state
        int min_value = INFINITY;  
        // We iterate, for every move, for every length of this move , for every patters, For every non colored Node
        for (const auto & instruction : istructions) {
            //If the move have a prefix length we set the length 1 else we need to iterate for every possibility
            int max_len_move = instruction[0] == 2 ? 1: n;
            // TODO: int min_len_move to have only orizontal can have len 1
            for(int len = max_len_move; len >= 1; --len){
                for(const auto & pattern : patterns){
                    for(int i = 0; i < n; ++i){
                        for(int j = 0; j < m; ++j){
                            if(current_state[i][j] == 0){
                                auto new_current_state = current_state;         //Duplicate the state to avoid to modify the object for other iteration
                                int new_id = executeInstruction(id, i,  j,instruction,  len,  pattern, end_solution, &new_current_state,value_index);
                                if(new_id > 0){                                 //If the result is accepted --> the function doesen't color outside the matrix
                                                                                //AND the instruction which non removes more colors than it adds
                                    int a = dfs_algo(new_id, memory, n,m,new_current_state,end_solution,(n_iter+1),value_index);                                
                                    min_value = std::min(min_value, (1+a));     //check if is better than actual min
                                }
                            }
                        }
                    }
                }
            }
        }
        memory[id] = min_value;             //Set this state visited
        return min_value;
    }
}


int main(int argc, char *argv[])
{   
    std::string path = "./Graph/miniGraph.txt";

    //read file and convert information into a matrix
    auto V = file_reader(path);
    int n = V.size();

    //Create Base case with the void table
    //in this step we can save the number of colored value
    std::vector<int> map_value(std::pow(n, 2), -1);
    int total_colored = 0;
    int tmp_iter_val = 0;
    std::vector<std::vector<int>> voidMat(n, std::vector<int> (n, 0));
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            if(V[i][j] < 0){
                voidMat[i][j] = V[i][j];
            }else{
                total_colored += 1;
                map_value[i*n+j] = tmp_iter_val;
                tmp_iter_val += 1;
            }
        }
    }    

    //with the information we can cmpute the dimension of the max id of a state (for the full colored graph)
    //And we can also initialize the memory vector
    int max_id = 0;
    for(int i = 0; i < total_colored; ++i){
        max_id +=  std::pow(2, i); 
    }
    int size = std::pow(2, total_colored);              //Memory have all possible combination of colore/no Color == 2^(n) with n = all colored cells
    std::vector<int> memory(size, -1);                  //Memory is initlized with -1 that means that this state is not reached in this moment
    memory[max_id] = 0;                                 //Set the value of complete colored case = 0, Used for the base case
    

    
    int res = dfs_algo(0, memory,  n,  n, voidMat, &V, 0,map_value);
    printf("\nEND = %d\n", res);
}

