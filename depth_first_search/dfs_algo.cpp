#include <fstream>
#include <iostream>
#include <string>
#include <random>
#include <vector>
#include <sstream>


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

int main(int argc, char *argv[])
{   
    std::mt19937 gen(123123);
    std::uniform_real_distribution<double> dis(0, 1);

    std::string path = "./Graph/LineeGraph.txt";

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

    std::vector<std::vector<int>> istructions = {{0,0,1}, {0,1,0},{0,-1,1},{0,-1,-1},{2,0,1,-1,0,0,-1},{2,-1,0,-1,0,0,-1,0,-1},{2,-1,0,-1,0,0,1,0,1},{2,0,1,0,1,1,0,1,0},{2,0,-1,0,-1,1,0,1,0},{1,-1,1,1,1},{1,1,1,-1,1},{1,1,1,1,-1},{1,1,-1,1,1}};
    std::vector<std::vector<int>> pattern = {{1},{2},{3},{4},{1,2},{1,3},{1,4},{2,1},{2,3},{2,4},{3,1},{3,2},{3,4},{4,1},{4,2},{4,3}};
   


}
