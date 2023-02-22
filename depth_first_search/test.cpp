#include <fstream>
#include <iostream>
#include <string>
#include <random>
#include <vector>
#include <sstream>


#include "./file_reader.hpp"
#include "./Move.hpp"


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

int main(int argc, char *argv[])
{   
    std::mt19937 gen(123123);
    std::uniform_real_distribution<double> dis(0, 1);

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
    tot_pass += TEST("SCAL R UP 3 COLOR", 1, 2,5,inst_L_left_up,patt_L_left_up, V);
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
