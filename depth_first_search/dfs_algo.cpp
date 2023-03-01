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
std::vector<std::vector<int>> istructions = {{0,0,1},{0,1,0}};
std::vector<std::vector<int>> patterns = {{4},{3},{4,3},{4,4,3},{4,3,3,3},{3,4}};

int dfs_algo(int id, std::vector<int> & memory, int n, int m, std::vector<std::vector<int>> & current_state, std::vector<std::vector<int>> * end_solution, int n_iter, std::vector<int> & value_index){
    if(memory[id] != -1){
        printf("\n content memory id = %d AND id = %d \n",memory[id], id);
        return memory[id];
    }else{
        int min_value = INFINITY;
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
                                    int a = dfs_algo(new_id, memory, n,m,new_current_state,end_solution,(n_iter+1),value_index);                                
                                    min_value = std::min(min_value, (1+a));
                                }
                            }
                        }
                    }
                }
            }
        }
        int sfdas = memory[id];
        memory[id] = min_value;
        return min_value;
    }
}

int dfs_algo_2( std::vector<std::vector<int>> & node_sol, std::vector<std::string> & move_sol, std::vector<std::vector<int>> & pattern_sol,std::vector<int> & len_move_sol , int id, std::vector<int> & memory, int n, int m, std::vector<std::vector<int>> & current_state, std::vector<std::vector<int>> * end_solution, int n_iter, std::vector<int> & value_index){
    if(memory[id] != -1){                                   //BASE CASE the state is visited
        return memory[id];
    }else{                                                  //ELSE CASE the state is a new state
        //BEST checker
        int min_value = INT16_MAX;                          
        auto best_move_sol = move_sol;
        auto best_pattern_sol = pattern_sol;
        auto best_len_move_sol = len_move_sol;

        for (const auto & instruction : istructions) {      
            for(int len = n; len >= 1; --len){
                for(const auto & pattern : patterns){
                    for(int i = 0; i < n; ++i){
                        for(int j = 0; j < m; ++j){
                            if(current_state[i][j] == 0){
                                auto new_current_state = current_state;
                                auto new_node_sol = node_sol;
                                new_node_sol.push_back({i,j});
                                auto new_move_sol = move_sol;
                                if(instruction[1] == 1){
                                    new_move_sol.push_back("vertical");
                                }else{
                                    new_move_sol.push_back("orizontal");
                                }
                                auto new_pattern_sol = pattern_sol;
                                new_pattern_sol.push_back(pattern);
                                auto new_len_move_sol = len_move_sol;
                                new_len_move_sol.push_back(len);
                                int new_id = moveFinal_2(id, i,  j,instruction,  len,  pattern, end_solution, &new_current_state,value_index);
                                if(new_id > 0){
                                    int a = dfs_algo_2(new_node_sol,new_move_sol,new_pattern_sol,new_len_move_sol, new_id, memory, n,m,new_current_state,end_solution,(n_iter+1),value_index);                         
                                    min_value = std::min(min_value, (1+a));
                                }
                            }
                        }
                    }
                }
            }
        }
        int sfdas = memory[id];
        memory[id] = min_value;
        return min_value;
    }
}

/*
std::vector<std::vector<int>> istructions_2 = {{0,0,1}};
std::vector<std::vector<int>> patterns_2 = {{4},{3}};
*/
std::vector<std::vector<int>> istructions_2 = {{0,0,1}};
std::vector<std::vector<int>> patterns_2 = {{3}};
int flag_deb = 0;
int dfs_algo_deb( std::vector<std::vector<int>> & node_sol, std::vector<std::string> & move_sol, std::vector<std::vector<int>> & pattern_sol,std::vector<int> & len_move_sol , int id, std::vector<int> & memory, int n, int m, std::vector<std::vector<int>> & current_state, std::vector<std::vector<int>> * end_solution, int n_iter, std::vector<int> & value_index){
    if(memory[id] != -1){
        flag_deb += 1;
        return memory[id];
    }else{
        int min_value = INT16_MAX;
        auto best_node_sol = node_sol;
        auto best_move_sol = move_sol;
        auto best_pattern_sol = pattern_sol;
        auto best_len_move_sol = len_move_sol;
        for (const auto & instruction : istructions_2) {
            for(int len = 1; len >= 1; --len){
                for(const auto & pattern : patterns_2){
                    for(int i = 0; i < n; ++i){
                        for(int j = 0; j < m; ++j){
                            if(current_state[i][j] == 0){

                                //MOVE SAVE
                                auto new_current_state = current_state;
                                auto new_node_sol = node_sol;
                                new_node_sol.push_back({i,j});
                                auto new_move_sol = move_sol;
                                if(instruction[1] == 1){
                                    new_move_sol.push_back("vertical");
                                }else{
                                    new_move_sol.push_back("orizontal");
                                }
                                auto new_pattern_sol = pattern_sol;
                                new_pattern_sol.push_back(pattern);
                                auto new_len_move_sol = len_move_sol;
                                new_len_move_sol.push_back(len);

                                //MOVE
                                int new_id = moveFinal_2(id, i,  j,instruction,  len,  pattern, end_solution, &new_current_state,value_index);
                                printArray(new_current_state);
                                printf("\n\n");
                                if(new_id > 0){
                                    int a = dfs_algo_deb(new_node_sol,new_move_sol,new_pattern_sol,new_len_move_sol, new_id, memory, n,m,new_current_state,end_solution,(n_iter+1),value_index);
                                    /*
                                    printArray(new_current_state);
                                    for(int sol_idx = 0;sol_idx < new_move_sol.size(); ++sol_idx){
                                        printf("instruction = %s, len_inst = %d, start node = {%d, %d}, pattern ={",new_move_sol[sol_idx].c_str(), new_len_move_sol[sol_idx],new_node_sol[sol_idx][0],new_node_sol[sol_idx][1]);
                                        for(int patt_l = 0; patt_l < new_pattern_sol[sol_idx].size();++patt_l){
                                            printf(" %d",new_pattern_sol[sol_idx][patt_l]);
                                        }
                                        printf("}     TOTAL MOVE = %d\n", 1+a);
                                    }  
                                    */
                                      
                                   if(min_value < (1+a)){

                                        best_node_sol = new_node_sol;
                                        best_move_sol = new_move_sol;
                                        best_pattern_sol = new_pattern_sol;
                                        best_len_move_sol = new_len_move_sol;
                                   }
                                    min_value = std::min(min_value, (1+a));
                                }
                            }
                        }
                    }
                }
            }
        }
        /*
        printf("min value = %d\n", min_value);
        if(min_value == 4){
            for(int sol_idx = 0;sol_idx < best_move_sol.size(); ++sol_idx){
                                        printf("instruction = %s, len_inst = %d, start node = {%d, %d}, pattern ={",best_move_sol[sol_idx].c_str(), best_len_move_sol[sol_idx],best_node_sol[sol_idx][0],best_node_sol[sol_idx][1]);
                                        for(int patt_l = 0; patt_l < best_pattern_sol[sol_idx].size();++patt_l){
                                            printf(" %d",best_pattern_sol[sol_idx][patt_l]);
                                        }
                                        printf("}     TOTAL MOVE = %d\n", min_value);
                                    } 
        }*/
        int sfdas = memory[id];
        memory[id] = min_value;

        //printf("end ciclo id = %d, min value = %d\n",id, min_value);
        return min_value;
    }
}



/*

int dfs_algo_2(int id, std::vector<int> & memory, int n, int m, std::vector<std::vector<int>> & current_state, std::vector<std::vector<int>> * end_solution, int n_iter, std::vector<int> & value_index){
    if(memory[id] != -1){                                                       //BASE CASE pattern visited
        return memory[id];
    }else{                                                                      //ELSE CASE new pattern to compute
        //min variable to check min move to reach this state
        int min_value = INFINITY;
        for (const auto & instruction : istructions) {                          //FOR every instruction
            for(int len = n; len >= 1; --len){                                  //FOR every length move
                for(const auto & pattern : patterns){                           //FOR every possible pattern
                    for(int i = 0; i < n; ++i){                                 //FOR every node {i,j}
                        for(int j = 0; j < m; ++j){
                            if(current_state[i][j] == 0){                       //IF node is not colored
                                //duplicate the current state
                                auto new_current_state = current_state;
                                //execute te move into the new_current_state
                                int new_id = moveFinal_2(id, i,  j,instruction,  len,  pattern, end_solution, &new_current_state,value_index);
                                if(new_id > 0){ // if the move is permitted AND the move add more node than the removed
                                    //cal recursion on new current state update from the move
                                    int a = dfs_algo(new_id, memory, n,m,new_current_state,end_solution,(n_iter+1),value_index);   
                                    //update min value if is min                             
                                    min_value = std::min(min_value, (1+a));
                                }
                            }
                        }
                    }
                }
            }
        }
        //set the min value to the state and return this value
        memory[id] = min_value;
        return min_value;
    }
}
*/


int main(int argc, char *argv[])
{   
    //std::string path = "./Graph/TestGraph.txt";
    //std::string path = "./Graph/Graph2.txt";
    std::string path = "./Graph/miniGraph.txt";

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
    int max_id = 0;
    for(int i = 0; i < total_colored; ++i){
        max_id +=  std::pow(2, i); 
    }
    int size = std::pow(2, total_colored); 
    std::vector<int> memory(size, -1);
    memory[max_id] = 0;
    
    printf("max len = %d , max id = %d",size,max_id);
    //int res = dfs_algo(0, memory,  n,  n, voidMat, &V, 0,map_value);
    //printf("res = %d\n",res);
    std::vector<std::vector<int>> node_sol = {};
    std::vector<std::string> move_sol = {};
    std::vector<std::vector<int>> pattern_sol = {};
    std::vector<int> len_move_sol = {};
    //int res = dfs_algo_deb(node_sol,move_sol, pattern_sol,len_move_sol, 0, memory,  n,  n, voidMat, &V, 0,map_value);
    //printf("END ) %d\n", res);
    int res = dfs_algo_2(node_sol,move_sol, pattern_sol,len_move_sol, 0, memory,  n,  n, voidMat, &V, 0,map_value);

    for(int sol_idx = 0;sol_idx < move_sol.size(); ++sol_idx){
        printf("instruction = %s, len_inst = %d, start node = {%d, %d}, pattern ={",move_sol[sol_idx].c_str(), len_move_sol[sol_idx],node_sol[sol_idx][0],node_sol[sol_idx][1]);
            for(int patt_l = 0; patt_l < pattern_sol[sol_idx].size();++patt_l){
                printf(" %d",pattern_sol[sol_idx][patt_l]);
            }
            printf("}     TOTAL MOVE = %d\n", res);
        }  
    printf("\nEND = %d\n", res);
}
