#include <fstream>
#include <iostream>
#include <string>
#include <random>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <cmath>
#include <limits>


#include "./file_reader.hpp"
#include "./Move_tmp.hpp"


float MIRROR_PROB = 0.3;
float DOUBLE_MIRR_PROB = 0.05;


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


/*

int optimalSol(int n, std::vector<std::vector<int>> V, int non_colored_point, int total_point, int total_move, std::vector<std::vector<int>> actualColor, std::vector<std::vector<int>> array_of_instruction,std::vector<std::vector<int>> array_of_pattern ){
    if(non_colored_point == 0){
        return total_move;
    }else{
        int min = INFINITY;
        for(int i = 0; i < n; ++i){
            for(int j = 0; j < n; ++j){
                for(int k = 0; k < array_of_instruction.size(); ++k){
                    for(int l = 0; l < array_of_pattern.size(); ++l){
                        auto newPath = ComplexmoveWithColor(i,j,array_of_instruction[k],array_of_pattern[l], V,actualColor);
                        for(int m = 0; m < newPath.size(); ++m){
                            auto newColoredSol = newPath[m];
                            int non_colored_point_tmp = countNonColored(newColoredSol);
                            int tmp = optimalSol(n,V,non_colored_point_tmp,total_point,(total_move+1),newColoredSol,array_of_instruction,array_of_pattern);
                            if(total_move < 8){
                                printf("%d\n",(total_move+1));
                            }
                            if(min < tmp){
                                min = tmp;
                            }
                        }
                    }
                }
            }
        }
        return min;
    }
}
*/


std::unordered_map<std::string, std::vector<int>> GET_INSTRUCTION = {
        {"orizontal", {0,0,1}},
        {"vertical", {0,1,0}},
        {"diagonal-U-R", {0,-1,1}},
        {"diagonal-U-L", {0,-1,-1}},
        {"square", {2,0,1,-1,0,0,-1}},
        {"L-UP-R", {2,-1,0,-1,0,0,-1,0,-1}},
        {"L-UP-L", {2,-1,0,-1,0,0,1,0,1}},
        {"L-R-UP", {2,0,-1,0,-1,-1,0,-1,0}},
        {"L-L-UP", {2,0,1,0,1,-1,0,-1,0}},
        {"ZigZag-Oriz-UP-DOW", {0,-1,1,1,1}},
        {"ZigZag-Oriz-DOW-UP", {0,1,1,-1,1}},
        {"ZigZag-Vert-R-L", {1,1,1,1,-1}},
        {"ZigZag-Vert-L-R", {1,1,-1,1,1}}
};

//std::vector<std::vector<int>> istructions = {{0,0,1},{0,0,1},{0,1,0},{0,1,0},{0,-1,1},{0,-1,-1},{2,0,1,-1,0,0,-1},{2,-1,0,-1,0,0,-1,0,-1},{2,-1,0,-1,0,0,1,0,1}, {2,0,-1,0,-1,-1,0,-1,0},{2,0,1,0,1,-1,0,-1,0},{0,-1,1,1,1},{0,1,1,-1,1},{1,1,1,1,-1},{1,1,-1,1,1}};
std::vector<std::vector<int>> istructions = {{0,0,1},{0,0,1},{0,1,0},{0,1,0}};
std::vector<std::vector<int>> patterns = {{4},{3},{4,3},{4,4,3},{4,3,3,3},{3,4}};

int skldahsd = 0;
int dfs_algo(int id, std::vector<int> & memory, int n, int m, std::vector<std::vector<int>> & current_state, std::vector<std::vector<int>> * end_solution, int n_iter, std::vector<int> & value_index){
    skldahsd += 1;
    printf("\n%d\n",skldahsd);
    if(memory[id] != -1){
        int sfdas = memory[id];
        return memory[id];
    }else{
        int min = INFINITY;
        //Todo avere una lista o qualcosa per sfoltire le mosse e pattern
        for (const auto & instruction : istructions) {
            for(int len = n; len >= 1; --len){
                for(const auto & pattern : patterns){
                    for(int i = 0; i < n; ++i){
                        for(int j = 0; j < m; ++j){
                            if(current_state[i][j] == 0){
                                auto new_current_state = current_state;
                                int new_id = moveFinal_2(id, i,  j,instruction,  len,  pattern, end_solution, &new_current_state,value_index);
                                if(new_id > 0){
                                    /*
                                     printf("_____ START______\n");
                                     for(int r =0; r < instruction.size(); ++ r){
                                            printf("%d ", instruction[r]);
                                    }
                                    printf("\n len inst %d\n new id = %d\n", len,new_id);
                                    for(int r = 0; r < pattern.size(); ++r){
                                        printf("%d ", pattern[r]);
                                    }
                                    printf(" \n");
                                    printArray(new_current_state);
                                    printf("\n\n\n");
                                    */
                                    int a = dfs_algo(new_id, memory, n,m,new_current_state,end_solution,(n_iter+1),value_index);                                
                                    min = std::min(min, new_id);
                                }
                            }
                        }
                    }
                }
            }
        }
        int sfdas = memory[id];
        memory[id] = min;//(n_iter+1);
        return min;
    }
}


int main(int argc, char *argv[])
{   
    std::string path = "./Graph/TestGraph.txt";
    auto V = file_reader(path);
    int n = V.size();
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
    
    int size = std::pow(2, total_colored); 
    std::vector<int> memory(size, -1);
    int res = dfs_algo(0, memory,  n,  n, voidMat, &V, 0,map_value);
    printf("res = %d\n",res);
}
