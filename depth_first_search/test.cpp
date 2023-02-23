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

int TEST(std::string Test_name, int expected_result,int i, int j, std::vector<int> instruction,  std::vector<int> pattern, std::vector<std::vector<int> > V){
    printf("%s", Test_name.c_str());
    if(expected_result == moveNoColor(i,j,instruction,pattern, V)){
        printf("\033[32m Test passed\033[0m\n");
        return 1;
    }else{
        printf("\033[31m Test failed\033[0m\n");
        return 0;
    }
}

int TEST2(std::string Test_name, int expected_result,int i, int j, std::vector<int> instruction,  std::vector<int> pattern, std::vector<std::vector<int> > V){
    printf("%s", Test_name.c_str());
    if(expected_result == moveNoColor(i,j,instruction,pattern, V)){
        printf("\033[32m Test passed\033[0m\n");
        return 1;
    }else{
        printf("\033[31m Test failed\033[0m\n");
        return 0;
    }
}

void test_try_possibility_color(){
    std::string path = "./Graph/TestGraph.txt";

    //Step 1 build vertex and edge matrix
    auto V = file_reader(path);
    int n = V.size();
    //printArray(V);
    
    int tot_pass = 0;
    int tot_test = 0;
    std::vector<int> inst = {0,0,1};
    std::vector<int> patt = {3};
    tot_pass += TEST("ORIZONTAL 1 COLOR", 6, 2, 0,inst,patt, V);
    tot_test += 1;
    std::vector<int> inst_2 = {0,0,1};
    std::vector<int> patt_2 = {4,3};
    tot_pass += TEST("ORIZONTAL 2 COLOR", 5, 3,0,inst_2,patt_2, V);
    tot_test += 1;

    std::vector<int> inst_3 = {0,0,1};
    std::vector<int> patt_3 = {4,3,4};
    tot_pass += TEST("ORIZONTAL 3 COLOR", 1, 3,0,inst_3,patt_3, V);
    tot_test += 1;

    std::vector<int> inst_4 = {0,0,1};
    std::vector<int> patt_4 = {4,3,4,3};
    tot_pass += TEST("ORIZONTAL 4 COLOR", 3, 3,0,inst_4,patt_4, V);
    tot_test += 1;

    std::vector<int> inst_v = {0,1,0};
    std::vector<int> patt_v = {3};
    tot_pass += TEST("VERTICAL  1 COLOR", 6, 0,3,inst_v,patt_v, V);
    tot_test += 1;

    std::vector<int> inst_v_1 = {0,1,0};
    std::vector<int> patt_v_2 = {4,4,3};
    tot_pass += TEST("VERTICAL  3 COLOR", 4, 0,2,inst_v_1,patt_v_2, V);
    tot_test += 1;

    std::vector<int> inst_diag = {0,-1,1};
    std::vector<int> patt_diag = {4,3};
    tot_pass += TEST("DIAG UP R 2 COLOR", 3, 3,0,inst_diag,patt_diag, V);
    tot_test += 1;

    std::vector<int> inst_diag_2 = {0,-1,-1};
    std::vector<int> patt_diag_2 = {3};
    tot_pass += TEST("DIAG UP L 1 COLOR", 3, 3,5,inst_diag_2,patt_diag_2, V);
    tot_test += 1;

    std::vector<int> inst_square = {2,0,1,-1,0,0,-1};
    std::vector<int> patt_square = {4,3,3,3};
    tot_pass += TEST("SQUARE    4 COLOR", 1, 3,0,inst_square,patt_square, V);
    tot_test += 1;

    std::vector<int> inst_L_up_left = {2,-1,0,-1,0,0,-1,0,-1};
    std::vector<int> patt_L_up_left = {3};
    tot_pass += TEST("SCAL UP L 1 COLOR", 1, 4,3,inst_L_up_left,patt_L_up_left, V);
    tot_test += 1;

    std::vector<int> inst_L_up_right = {2,-1,0,-1,0,0,1,0,1};
    std::vector<int> patt_L_up_right = {3};
    tot_pass += TEST("SCAL UP R 3 COLOR", 1, 4,3,inst_L_up_right,patt_L_up_right, V);
    tot_test += 1;

    std::vector<int> inst_L_right_up = {2,0,1,0,1,1,0,1,0};
    std::vector<int> patt_L_right_up = {3};
    tot_pass += TEST("SCAL R UP 3 COLOR", 1, 2,1,inst_L_right_up,patt_L_right_up, V);
    tot_test += 1;

    std::vector<int> inst_L_left_up = {2,0,-1,0,-1,1,0,1,0};
    std::vector<int> patt_L_left_up = {3};
    tot_pass += TEST("SCAL L UP 3 COLOR", 1, 2,5,inst_L_left_up,patt_L_left_up, V);
    tot_test += 1;

    std::vector<int> inst_zig_1 = {1,-1,1,1,1};
    std::vector<int> patt_zig_1 = {4,3};
    tot_pass += TEST("ZIG-ZAG 1 2 COLOR", 4, 3,0,inst_zig_1,patt_zig_1, V);
    tot_test += 1;

    std::vector<int> inst_zig_2 = {1,1,1,-1,1};
    std::vector<int> patt_zig_2 = {3};
    tot_pass += TEST("ZIG-ZAG 2 1 COLOR", 4, 2,0,inst_zig_2,patt_zig_2, V);
    tot_test += 1;

    std::vector<int> inst_zig_3 = {1,1,1,1,-1};
    std::vector<int> patt_zig_3 = {4,3,3,3};
    tot_pass += TEST("ZIG-ZAG 3 4 COLOR", 3, 0,2,inst_zig_3,patt_zig_3, V);
    tot_test += 1;

    std::vector<int> inst_zig_4= {1,1,-1,1,1};
    std::vector<int> patt_zig_4 = {3,4};
    tot_pass += TEST("ZIG-ZAG 4 2 COLOR", 3, 0,3,inst_zig_4,patt_zig_4, V);
    tot_test += 1;

    printf("\n____________________________\n");
    printf("%d out of %d tests passed\n\n",tot_pass, tot_test);
}

void test_color_graph(){
    std::string path = "./Graph/TestGraph.txt";

    auto V = file_reader_test(path);
    int n = V.size();


    std::vector<std::vector<int>> voidMat(n, std::vector<int> (n, 0));
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            if(V[i][j] < 0){
                voidMat[i][j] = V[i][j];
            }
        }
    }

    std::vector<std::vector<int>> istruction = {{0,0,1},{0,0,1},{0,1,0},{0,1,0},{0,-1,1},{0,-1,-1},{2,0,1,-1,0,0,-1},{2,-1,0,-1,0,0,-1,0,-1},{2,-1,0,-1,0,0,1,0,1}, {2,0,-1,0,-1,-1,0,-1,0},{2,0,1,0,1,-1,0,-1,0},{0,-1,1,1,1},{0,1,1,-1,1},{1,1,1,1,-1},{1,1,-1,1,1}};
    std::vector<std::vector<int>> pattern = {{3},{4,3},{3},{4,4,3},{3},{3},{4,3,3,3},{3},{3},{3},{3},{4,3},{3},{4,3,3,3},{3,4}};
    std::vector<std::vector<int>> node_coord = {{2, 0},{3,0}, {0,3},{0,2},{3,1},{3,5},{3,0},{4,3},{4,3},{2,5},{2,1},{3,0},{2,0},{0,2},{0,3}};
    std::vector<int> length_inst = {6,4,6,4,3,4,1,1,1,1,1,5,6,6,5};
    std::vector<int> resultCheck = {6,4,6,4,3,3,4,5,5,5,5,5,6,6,5};
    int index = 0;

    for (const auto& file : std::__fs::filesystem::directory_iterator("./Graph/TEST")) {
        if (file.is_regular_file()) {
            std::string filename = file.path().filename().string();
            std::string input = filename;
            std::stringstream ss(input);
            std::string token;
            std::string nonValid = ".DS";
            

            while(std::getline(ss, token, '_')) {
                if(nonValid == token){
                    index = 0;
                }else{
                    index = std::stoi(token);
                }
                break;
            }
            if(index == 0 || index > 13){
                continue;
            }
            index -= 1;
            
            auto test_res = file_reader_test(file.path().string());
            auto tmp_res = voidMat;
            int res_int = moveAndColor(node_coord[index][0], node_coord[index][1], istruction[index],  length_inst[index], pattern[index], &V, &tmp_res);
            bool flag = true;
            

            for (size_t i = 0; i < test_res.size(); ++i) {
                for (size_t j = 0; j < test_res[0].size(); ++j) {
                    if (test_res[i][j] != tmp_res[i][j]) {
                        flag = false;
                    }
                }
            }
            printf("%s ",filename.c_str());
            if(flag){
                printf("\033[32m passed print\033[0m");
            }else{
                printf("\033[31m failed print\033[0m");
            }

            if(resultCheck[index] == res_int){
                printf("\033[32m passed correct_print\033[0m\n");
            }else{
                printf("\033[31m failed correct_print %d instead %d\033[0m\n", res_int, resultCheck[index]);
                printArray(tmp_res);
            }

            if(index == 1000){
                printArray(tmp_res);
                printf("\n");
                printArray(test_res);
            }
        }
    }
}



void test_color_all_graph(){
    std::string path = "./Graph/TestGraph.txt";
    auto V = file_reader_test(path);
    int n = V.size();

    std::vector<std::vector<int>> voidMat(n, std::vector<int> (n, 0));
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            if(V[i][j] < 0){
                voidMat[i][j] = V[i][j];
            }
        }
    }

    std::vector<std::string> comand_move = {"orizontal","orizontal","vertical","vertical"};
    std::vector<int> length_move = {n,n,n,n};
    std::vector<std::vector<int>> start_move = {{3,0},{2,0},{0,2},{0,3}};
    std::vector<std::vector<int>> pattern_move = {{4,3},{3},{4,4,3},{3}};
    
    std::vector<int> expected_res = {6,6,4,4};


    int idx = 0;
    while(idx < comand_move.size()){
        int res_int = moveAndColor(start_move[idx][0], start_move[idx][1], GET_INSTRUCTION[comand_move[idx]],  length_move[idx], pattern_move[idx], &V, &voidMat);
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
    }


}


void test_color_all_graph_sovrapposition(){
    std::string path = "./Graph/TestGraph.txt";
    auto V = file_reader_test(path);
    int n = V.size();

    std::vector<std::vector<int>> voidMat(n, std::vector<int> (n, 0));
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            if(V[i][j] < 0){
                voidMat[i][j] = V[i][j];
            }
        }
    }

    std::vector<std::string> comand_move = {"orizontal","orizontal","orizontal","orizontal","vertical","vertical","vertical"};
    std::vector<int> length_move = {n,n,n,n,3,n,n};
    std::vector<std::vector<int>> start_move = {{3,0},{2,0},{2,0},{2,0},{0,2},{0,2},{0,3}};
    std::vector<std::vector<int>> pattern_move = {{4,3},{4},{3},{4},{4,4,3},{4,4,3},{3}};
    
    std::vector<int> expected_res = {6,0,6,-6,2,2,4};


    int idx = 0;
    while(idx < comand_move.size()){
        auto tmp_voidMat = voidMat;
        int res_int = moveAndColor(start_move[idx][0], start_move[idx][1], GET_INSTRUCTION[comand_move[idx]],  length_move[idx], pattern_move[idx], &V, &tmp_voidMat);
        if(res_int> 0){
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


}

int main(int argc, char *argv[])
{   
    std::mt19937 gen(123123);
    std::uniform_real_distribution<double> dis(0, 1);

    test_try_possibility_color();
    test_color_graph();
    printf("\n\n");
    test_color_all_graph();
    test_color_all_graph_sovrapposition();
}
