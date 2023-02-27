#include <fstream>
#include <iostream>
#include <string>
#include <random>
#include <vector>
#include <sstream>
#include <filesystem>
#include <unordered_map>



#include "./file_reader.hpp"
#include "./Move.hpp"


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




void test_color_all_graph_final(){

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
                int tmp_idx = i*n+j;
                map_value[tmp_idx] = tmp_iter_val;
                tmp_iter_val += 1;
            }
        }
    }
    int size = std::pow(2, total_colored); 
    std::vector<int> memory(size, -1);

    std::vector<std::string> comand_move = {"orizontal","orizontal","orizontal","orizontal","vertical","vertical","vertical"};
    std::vector<int> length_move = {n,n,n,n,3,n,n};
    std::vector<std::vector<int>> start_move = {{3,0},{2,0},{2,0},{2,0},{0,2},{0,2},{0,3}};
    std::vector<std::vector<int>> pattern_move = {{4,3},{4},{3},{4},{4,4,3},{4,4,3},{3}};
    int id = 0;
    std::vector<int> expected_res = {6,0,6,-6,2,2,4};
    int idx = 0;
    while(idx < comand_move.size()){
        auto tmp_voidMat = voidMat;
        int res_int = 1;//oneStep(id, start_move[idx][0], start_move[idx][1], GET_INSTRUCTION[comand_move[idx]],  length_move[idx], pattern_move[idx], &V, &tmp_voidMat,map_value);
        if(res_int> 0){
            id = res_int;
            voidMat = tmp_voidMat;

        }
        if(expected_res[idx] == res_int){
                printf("\033[32m correct new colored\033[0m\n");
            }else{
                printf("\033[31m failed  new colored expected %d, get %d\033[0m\n",expected_res[idx],res_int);
        }
        idx += 1;
    }
    bool flag = true;
    for (size_t i = 0; i < V.size(); ++i) {
        for (size_t j = 0; j < V[0].size(); ++j) {
            if (V[i][j] != voidMat[i][j]) {
                flag = false;
            }
        }
    }

    if(flag){
         printf("\033[32m pass program test \033[0m\n");
    }else{
         printf("\033[31m fail program test \033[0m\n");
         printArray(voidMat);
    }
    printf("%d\n",id);



}

int main(int argc, char *argv[])
{   
    std::mt19937 gen(123123);
    std::uniform_real_distribution<double> dis(0, 1);
/*
    test_try_possibility_color();
    test_color_graph();
    printf("\n\n");
    test_color_all_graph();
    test_color_all_graph_sovrapposition();*/

    test_color_all_graph_final();
}
