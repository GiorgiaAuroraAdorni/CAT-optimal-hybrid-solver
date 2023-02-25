#include <fstream>
#include <iostream>
#include <string>
#include <random>
#include <vector>
#include <sstream>
#include <unordered_map>


#include "./file_reader.hpp"
#include "./Move.hpp"


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
std::vector<std::vector<int>> patterns = {{3},{4,3},{4,4,3},{4,3,3,3},{3,4}};;
int dfs_algo(int id, std::vector<int> & memory, int n, int m, std::vector<std::vector<int>> current_state, std::vector<std::vector<int>> * end_solution){
    if(memory[id] != -1){
        return memory[id];
    }else{
        int min = INFINITY;
        //Todo avere una lista o qualcosa per sfoltire le mosse e pattern
        for (const auto& instruction : istructions) {
           for(int i = 1; i < current_state.size(); ++i){
                for(const auto & pattern : patterns){
                    int new_id = moveFinal(id, n,m,instruction, i, pattern, end_solution, &current_state);
                    if(new_id > 0){
                        int a = dfs_algo(new_id, memory, n,m,current_state,end_solution);
                        min = std::min(min, new_id);
                    }
                }
           }

        }
        memory[id] = min;
        return min;
    }
}


int main(int argc, char *argv[])
{   
    std::string path = "./Graph/TestGraph.txt";
    auto V = file_reader(path);
    int n = V.size();

    std::vector<std::vector<int>> voidMat(n, std::vector<int> (n, 0));
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            if(V[i][j] < 0){
                voidMat[i][j] = V[i][j];
            }
        }
    }
    std::vector<std::string> program = {};
    //int res = dfs_algo(V, voidMat,0,0,program);
    printf("res = %d\n",res);
}
