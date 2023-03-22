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
#include <ctime>
#include <random>
#include <limits.h>


#include "./FileReader.hpp"
#include "./instruction.hpp"
#include "./Move.hpp"
#include "./Patterns.hpp"
#include "./HashFile.hpp"
#include "./ProgramString.hpp"


double MIN_SIMILARITY = 0.75;
int NO_COLOR = 5;


void printArray( std::vector<std::vector<int>> mat){
    for(int i = 0; i < mat.size(); ++i){
        for(int j = 0; j < mat.size(); ++j){
            if(mat[i][j] >= 0){
                if(mat[i][j] == 5){
                    printf(" 0 ");
                }else{
                    printf(" %d ", mat[i][j]);
                }
            }else{
                printf("%d ", mat[i][j]);
            }
        }
        printf("\n");
    }
}


int fillCheck(int n,  std::vector<std::vector<int>> & current_state, const std::vector<std::vector<int>> & end_solution){
    bool flag = true;
    int first_color = -1;
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            if(end_solution[i][j] == -1){
                continue;
            }
            if(first_color == -1 && current_state[i][j] != end_solution[i][j]){
                first_color = end_solution[i][j];
            }

            if(current_state[i][j] == NO_COLOR || current_state[i][j]  != first_color){
                flag = false;
                break;
            }
            
        }
        if(!flag){
                break;
        }
    }
    if(flag){
        return first_color;
    }
    return -1;
}

std::pair<int, std::string>  bfs_algo_program(std::vector<Instruction> Moves, int id, int n, std::vector<std::vector<int>> & current_state,std::vector<std::vector<int>> & end_solution, std::vector<int> & memory, std::vector<std::string> & memory_prog, std::vector<int> & value_index){
    
    if(memory[id] != -1){
        return std::make_pair(memory[id], memory_prog[id]);   
    }

    int checkFill = fillCheck(n,current_state,end_solution);
    if(checkFill > 0){
        std::make_pair(1, ("fill Pattern = {"+std::to_string(checkFill)+"}\n"));
    }


    std::string best_prog = "";
    int best_sol = INT_MAX;
    for(int mov = 0; mov < Moves.size(); mov++){
        auto best_copy = checkForBESTmove(Moves[mov].instruction, Moves[mov].len, Moves[mov].pattern,  &end_solution, &current_state).first;
        int new_id = id;
        auto new_current_state = current_state;
        std::vector<std::vector<int>> current_used_node; 
        for(int i = 0; i < best_copy.size(); ++i){
            current_used_node.push_back({best_copy[i][0],best_copy[i][1]});
            new_id = executeInstruction(new_id, best_copy[i][0],best_copy[i][1],Moves[mov].instruction,   Moves[mov].len,   Moves[mov].pattern, &end_solution, &new_current_state,value_index);
            auto current_res = bfs_algo_program(Moves, new_id,  n, new_current_state, end_solution, memory, memory_prog, value_index);

            if(best_sol > (current_res.first + 1)){
                best_prog =  createInst(current_used_node,Moves[mov]) + "\n" + current_res.second;
                best_sol = (current_res.first + 1);
            }

        }

    }

    memory[id] = best_sol;             //Set this state visited
    memory_prog[id] = best_prog;
    return std::make_pair(best_sol, best_prog);

}




int main(int argc, char *argv[])
{   
    //TODO 4
    std::time_t start_algo = std::time(nullptr);
    
    std::ios_base::sync_with_stdio(false);

    //3 solo 3 e non 2 PERCHE la sol non ha i primi numeri :)
    //2 troppo lento
    //std::string path = "./Graph/miniGraph_6.txt";
    std::string path = "./Graph/testGraph.txt";
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


    
    //TODO NON VEDO L
    std::vector<std::vector<int>> TOT_istructions = {{0,0,1},{0,1,0},{0,-1,1},{0,-1,-1},{2,0,1,-1,0,0,-1},{2,-1,0,-1,0,0,-1,0,-1},{2,-1,0,-1,0,0,1,0,1}, {2,0,-1,0,-1,-1,0,-1,0},{2,0,1,0,1,-1,0,-1,0},{0,-1,1,1,1},{0,1,1,-1,1},{1,1,1,1,-1},{1,1,-1,1,1}};
    std::string start_prog = "";

    auto allPssibleCombination = getPossibleMove(TOT_istructions ,V);

    std::vector<Instruction> Moves;

    for(int i = 0; i < TOT_istructions.size(); ++i ){
        for(int j = 0; j < allPssibleCombination.second[i].size(); ++j){
            for(int a = 0; a < allPssibleCombination.first[i].size(); ++a){
                for(int b = 0; b < allPssibleCombination.first[i][a].size(); ++b){
                    Instruction tmp_inst;
                    tmp_inst.instruction = TOT_istructions[i];
                    tmp_inst.pattern = allPssibleCombination.first[i][a][b];
                    tmp_inst.len = allPssibleCombination.second[i][j];
                    Moves.push_back(tmp_inst);                    
                }
            }
        }
    }

    auto start = std::chrono::high_resolution_clock::now();
    Moves = getPossibleINST(TOT_istructions ,V);
    auto result = bfs_algo_program(Moves, 0,  n, voidMat, V, memory, memory_program, map_value);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration_sec = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Time of execution: " << duration_sec.count() << " seconds" << std::endl;

    int res = result.first;
    printf("value Final = %d\n", res);
    printf("______________________________ Program ______________________________\n%s", result.second.c_str());
    printf("\n");
    auto prog_arr = parseString(result.second);

}

