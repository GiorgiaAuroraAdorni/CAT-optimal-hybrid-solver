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


std::unordered_map<std::string, std::vector<int>> GET_INSTRUCTION = {
        {"orizontal", {0,0,1}},
        {"vertical", {0,1,0}},
        {"diagonal-U-R", {0,-1,1}},
        {"diagonal-U-L", {0,-1,-1}},
        {"square", {2,0,1,-1,0,0,-1}},
        {"L-UP-L", {2,-1,0,-1,0,0,-1,0,-1}},
        {"L-UP-R", {2,-1,0,-1,0,0,1,0,1}},
        {"L-R-UP", {2,0,-1,0,-1,-1,0,-1,0}},
        {"L-L-UP", {2,0,1,0,1,-1,0,-1,0}},
        {"ZigZag-Oriz-UP-DOW", {0,-1,1,1,1}},
        {"ZigZag-Oriz-DOW-UP", {0,1,1,-1,1}},
        {"ZigZag-Vert-R-L", {1,1,1,1,-1}},
        {"ZigZag-Vert-L-R", {1,1,-1,1,1}}
};



std::string buildInstruction(std::vector<std::vector<int>> index_node, int len, std::vector<int> instruction, std::vector<int> pattern){
    std::string result = "Nodes(";
    for(int i = 0; i < index_node.size(); i++){
        auto tmp_1 = std::to_string(index_node[i][0]);
        auto tmp_2 = std::to_string(index_node[i][1]);
        result = result+ "{" +tmp_1 + "," + tmp_2 + "}";
    }
    result = result + ")";
    
    if(index_node.size() > 2){
        result = result + " copy inst ";
    }

    result = result + "{"+GET_NAME_INSTRUCTION[instruction]+ "} len = {"+ std::to_string(len) + "}  Pattern = {";
    for(int i = 0; i < pattern.size(); ++i){
        result = result + std::to_string(pattern[i]) + " ";
    }
    result = result + "}";
    return result;
}

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


std::pair<int, std::string>  bfs_algo_program(std::vector<std::string> & memory_program, int id, std::vector<int> & memory, int n, int m, std::vector<std::vector<int>> & current_state, std::vector<std::vector<int>> * end_solution, int n_iter, std::vector<int> & value_index){
    if(id < 0){
        return std::make_pair(10000000, "no correct program");
    }
    if(n_iter > 4){
        return std::make_pair(10000000, "no correct program");
    }
    if(memory[id] != -1){                               // BASE CASE --> We have alsa visited this case
        return std::make_pair(memory[id], memory_program[id]);                              // return the value to reach the id-th state
    }                                           // ELSE CASE --> We need to compute the move to reach the state
    int check_color = checkAllColor(end_solution, &current_state);
    if(check_color > 0){
        return std::make_pair(10, ("Nodes({0,0}){fill} len = {1}  Pattern = {"+std::to_string(check_color)+"}\n"));
    }
    int min_value = 10000;//INFINITY; 
    std::string best_prog = "";
    // We iterate, for every move, for every length of this move , for every patters, For every non colored Node
    for (int inst = 0; inst < istructions.size(); ++inst) {
        const auto instruction = istructions[inst];
        //If the move have a prefix length we set the length 1 else we need to iterate for every possibility
        int max_len_move = instruction[0] == 2 ? 1: n;
        int min_len_mov = ((instruction.size() == 3 && instruction[1] == 1 && instruction[1] == 0)|| instruction[0] == 2)? 1 : 2;
        // TODO: int min_len_move to have only orizontal can have len 1
        for(int len = max_len_move; len >= min_len_mov; --len){
            for(int patt = 0; patt < patterns.size();++patt){
                const auto pattern = patterns[patt];
                for(int i = 0; i < n; ++i){
                    for(int j = 0; j < m; ++j){
                        if(current_state[i][j] == 0 && pattern[0] == (*end_solution)[i][j]){
                            auto new_current_state = current_state;         //Duplicate the state to avoid to modify the object for other iteration
                            int number_new = executeInstruction_number(0, i,  j,instruction,  len,  pattern, end_solution, &new_current_state);
                            

                            if(number_new > 0){                                 //If the result is accepted --> the function doesen't color outside the matrix
                                                                            //AND the instruction which non removes more colors than it adds
                                
                                
                                //check if we can do a copy and repeat of this move
                                //but in all case we remove the last move for add into check_copy array
                                auto check_copy = checkForCopy(id, i,  j,instruction,  len,  pattern, end_solution, &new_current_state,value_index);
                                //std::vector<std::vector<int>> check_copy = {{i,j}};                                    
                                int new_id = id;
                                for(int ind_n = 0; ind_n < check_copy.size(); ++ind_n){
                                    int prev_id = new_id;
                                    new_id = executeInstruction(new_id, check_copy[ind_n][0],check_copy[ind_n][1],instruction,  len,  pattern, end_solution, &new_current_state,value_index);
                                }
                                
                                
                                if(new_id < 0){
                                    printArray(current_state);
                                    printf("ID = %d , Instruction %s\n",new_id, (buildInstruction(check_copy,len,instruction,pattern)).c_str());
                                    printArray(new_current_state);
                                }
                                    

                                //now we can call the recursion
                                auto pair_rec = bfs_algo_program(memory_program, new_id, memory, n,m,new_current_state,end_solution,(n_iter+1),value_index);   
                                int a = pair_rec.first;
                                if(min_value > (1+a)){
                                    auto actualProg = buildInstruction(check_copy,len,instruction,pattern);
                                    best_prog =  actualProg + "\n" + pair_rec.second;
                                }  
                                    
                                    
                                min_value = std::min(min_value, (1+a));     //check if is better than actual min
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
    std::string path = "./Graph/miniGraph.txt";
    //std::string path = "./Graph/TestGraph.txt";
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



    
   
    /*
    int len_mov = 2;
    int id_2 = executeInstruction(0, 0,  0,{0,0,1},  len_mov,  {3}, &V, &voidMat,map_value);
    printf("check colore = %d\n",checkAllColor(&V, &voidMat));
    auto copy_tmp = checkForCopy(0, 0,  0,{0,0,1},  len_mov,  {3}, &V, &voidMat,map_value);
    int prev_id = id_2;
    for(int i = 0; i < copy_tmp.size(); ++i){
        printf("copy into node %d, %d\n",copy_tmp[i][0],copy_tmp[i][1]);
        id_2 = executeInstruction(id_2, copy_tmp[i][0],copy_tmp[i][1],{0,0,1},  len_mov,  {3}, &V, &voidMat,map_value);
        if(id_2 < prev_id){
            printArray(voidMat);
            printf(" WOOOO = %d, %d\n", copy_tmp[i][0],copy_tmp[i][1]);
        }
        prev_id = id_2;
    }
    printf("check colore = %d\n",checkAllColor(&V, &voidMat));
    printArray(voidMat);

    auto result = buildInstruction({{0,0},{2,0},{3,0}}, len_mov,{0,0,1},{3} );
    printf("instruction: \n%s\n, ID = %d\n", result.c_str(),id_2);
    */
    

   /*
    auto tmp_voidMat = voidMat;
    int id_1 = executeInstruction(0, 2,  0,{2,-1,0,-1,0,0,1,0,1},  1,  {3,4,3}, &V, &tmp_voidMat,map_value);
    id_1 = executeInstruction(id_1, 2,  1,{2,0,1,-1,0,0,-1},  1,  {3,4,4,4}, &V, &tmp_voidMat,map_value);
    printf("ID = %d \n", id_1);
    printArray(tmp_voidMat);
    */

    /*
    auto tmp_voidMat = voidMat;
    int id_1 = executeInstruction(0, 0,  0,{0,0,1},  3,  {3, 1}, &V, &tmp_voidMat,map_value);
    id_1 = executeInstruction(id_1, 2,  0,{0,0,1},  3,  {3, 1}, &V, &tmp_voidMat,map_value);
    id_1 = executeInstruction(id_1, 1,  0,{2,0,1,-1,0,0,-1},  1,  {3, 1}, &V, &tmp_voidMat,map_value);
    id_1 = executeInstruction(id_1, 1,  1,{2,0,1,-1,0,0,-1},  1,  {3,1}, &V, &tmp_voidMat,map_value);
    printf("ID = %d \n", id_1);
    printArray(tmp_voidMat);
    return 1;*/
    
    patterns = generatePatterns(4);
    std::string start_prog = "";


    auto start = std::chrono::high_resolution_clock::now();
    printArray(V);
    auto res_pair = bfs_algo_program(memory_program, 0, memory,  n,  n, voidMat, &V, 0,map_value);
    printf("end\n");

    auto end = std::chrono::high_resolution_clock::now();
    auto duration_sec = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Tempo di esecuzione: " << duration_sec.count() << " secondi" << std::endl;


    int res = res_pair.first;
    printf("value Final = %d\n", res);
    printf("progr Final = \n%s", res_pair.second.c_str());
    
    
}

