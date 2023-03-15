#include <fstream>
#include <iostream>
#include <string>
#include <random>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <cmath>
#include <limits>
#include <chrono>


#include "./FileReader.hpp"
#include "./Move.hpp"
#include "./Patterns.hpp"
#include "./HashFile.hpp"
#include "./ProgramString.hpp"



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

std::vector<std::vector<int>> istructions = {{0,0,1},{0,1,0},{0,-1,1},{0,-1,-1},{2,0,1,-1,0,0,-1},{2,-1,0,-1,0,0,-1,0,-1},{2,-1,0,-1,0,0,1,0,1}, {2,0,-1,0,-1,-1,0,-1,0},{2,0,1,0,1,-1,0,-1,0},{0,-1,1,1,1},{0,1,1,-1,1},{1,1,1,1,-1},{1,1,-1,1,1}};
std::vector<std::vector<int>> patterns = {{4},{3},{4,3},{4,4,3},{4,3,3,3},{3,4},{3,3,4,4},{3,4,3,3},{3,4,3},{3,4,4,4}};



std::pair<int, std::string>  bfs_algo_program_4(std::vector<std::vector<int>> istructions,std::vector<std::vector<int>> patterns, std::vector<std::string> & memory_program, int id, std::vector<int> & memory, int n, int m, std::vector<std::vector<int>> & current_state, std::vector<std::vector<int>> * end_solution, int n_iter, std::vector<int> & value_index){

    if(memory[id] != -1){
        return std::make_pair(memory[id], memory_program[id]);   
    }
    int check_color = checkAllColor(end_solution, &current_state);
    if(check_color > 0){
        return std::make_pair(1, ("Nodes({0,0}){fill} len = {1}  Pattern = {"+std::to_string(check_color)+"}\n"));
    }

    int min_value = 10000;
    std::string best_prog = "";
    for (int inst = 0; inst < istructions.size(); ++inst) {
        
        const auto instruction = istructions[inst];

        for(int patt = 0; patt < patterns.size();++patt){
            const auto pattern = patterns[patt];
            int max_len_move = instruction[0] == 2 ? 1: n;
            int min_len_mov = instruction[0] == 2 ? 1 : pattern.size();

            for(int len = max_len_move; len >= min_len_mov; --len){
                
                for(int i = 0; i < n; ++i){

                    for(int j = 0; j < m; ++j){
                        if((current_state[i][j] == 0 ||  current_state[i][j] == 5)){
                            int new_id = id;
                            auto new_current_state = current_state;
                            int number_new = executeInstruction_number(0, i,  j,instruction,  len,  pattern, end_solution, &current_state);

                            if(number_new > 0){
                                new_id = executeInstruction(new_id, i,j,instruction,  len,  pattern, end_solution, &new_current_state,value_index);
                                

                                std::pair<int, std::string>  pair_rec = bfs_algo_program_4(istructions,patterns,memory_program, new_id, memory, n,m,new_current_state,end_solution,(n_iter+1),value_index);   
                                
                                auto actualProg2 = buildInstruction({{i,j}},len,instruction,pattern);
                                int a = (12 == pair_rec.second.find(removeFirst12Chars(actualProg2)))? pair_rec.first : (pair_rec.first+1);
                                

                                if(min_value > a){
                                    auto actualProg = actualProg2;
                                    best_prog =  actualProg + "\n" + pair_rec.second;
                                    min_value = a;
                                } 
                            }
                        }
                    }
                }
            }
        }
    }

    memory[id] = min_value;             //Set this state visited
    memory_program[id] = best_prog;
    return std::make_pair(min_value, best_prog);
}







int main(int argc, char *argv[])
{   
    std::string filename = argv[1];
    std::string path = "./Graph/" + filename;
    
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
    std::vector<std::string> memory_program(size,"");

    
    patterns = generatePatterns(4);
    std::string start_prog = "";


    auto start = std::chrono::high_resolution_clock::now();
    auto res_pair = bfs_algo_program_4(istructions,patterns, memory_program, 0, memory,  n,  n, voidMat, &V, 0,map_value);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration_sec = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Tempo di esecuzione: " << duration_sec.count() << " secondi" << std::endl;


    int res = res_pair.first;
    printf("value Final = %d\n", res);
    printf("______________________________ Program ______________________________\n%s", res_pair.second.c_str());
    printf("\n");
    auto prog_arr = parseString(res_pair.second);
    
    

    // Execute the program
    bool exec = false;
    //Exectute program
    if(exec){
        auto void_mat_res = voidMat;
        for(int i = 0; i < prog_arr.size(); ++i){
            std::vector<int> pattern_t;
            std::string prog_t;
            int len;
            std::vector<std::vector<int>> node_t;
            getProg(prog_arr[i],&prog_t, &len,pattern_t,node_t);
            printf("%s\n", buildInstruction(node_t,len,GET_INSTRUCTION[prog_t],pattern_t).c_str());
            for(int j = 0; j < node_t.size(); ++j){
                if(prog_t == "fill"){
                    break;
                }
                executeInstruction(0, node_t[j][0],  node_t[j][1],GET_INSTRUCTION[prog_t],  len,  pattern_t, &V, &void_mat_res,map_value);
            }
            printArray(void_mat_res);
            printf("\n");
        }
        printArray(void_mat_res);
    }
    
        
}