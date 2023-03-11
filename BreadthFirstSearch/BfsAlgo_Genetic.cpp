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

#include "./Genetic/individual.hpp"

int NUMBER_INDIVIDUAL = 100;
int NUMBER_PARENT = 80;
int NUMBER_GENE = 4;

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

//std::vector<std::vector<int>> istructions = {{0,0,1},{0,1,0},{0,-1,1},{0,-1,-1},{2,0,1,-1,0,0,-1},{2,-1,0,-1,0,0,-1,0,-1},{2,-1,0,-1,0,0,1,0,1}, {2,0,-1,0,-1,-1,0,-1,0},{2,0,1,0,1,-1,0,-1,0},{0,-1,1,1,1},{0,1,1,-1,1},{1,1,1,1,-1},{1,1,-1,1,1}};
//std::vector<std::vector<int>> patterns = {{4},{3},{4,3},{4,4,3},{4,3,3,3},{3,4},{3,3,4,4},{3,4,3,3},{3,4,3},{3,4,4,4}};


std::pair<int, std::string>  bfs_algo_program(std::vector<std::vector<int>> istructions,std::vector<std::vector<int>> patterns, std::vector<std::string> & memory_program, int id, std::vector<int> & memory, int n, int m, std::vector<std::vector<int>> & current_state, std::vector<std::vector<int>> * end_solution, int n_iter, std::vector<int> & value_index){
    if(id < 0){
        printf("BUG\n");
        return std::make_pair(10000, "no correct program");
    }
    if(memory[id] != -1){                               // BASE CASE --> We have alsa visited this case
        return std::make_pair(memory[id], memory_program[id]);                              // return the value to reach the id-th state
    }                                           // ELSE CASE --> We need to compute the move to reach the state
    int check_color = checkAllColor(end_solution, &current_state);
    if(check_color > 0){
        return std::make_pair(1, ("Nodes({0,0}){fill} len = {1}  Pattern = {"+std::to_string(check_color)+"}\n"));
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
                if(instruction[0] == 0 &&  pattern.size() > len){
                    continue;
                }
                bool copy_instruction = true;
                for(int i = 0; i < n; ++i){
                    for(int j = 0; j < m; ++j){
                        if(current_state[i][j] == 0 && pattern[0] == (*end_solution)[i][j]){
                            auto new_current_state = current_state;         //Duplicate the state to avoid to modify the object for other iteration
                            int number_new = executeInstruction_number(0, i,  j,instruction,  len,  pattern, end_solution, &new_current_state);
                            

                            if(number_new > 0){//If the result is accepted --> the function doesen't color outside the matrix
                                               //AND the instruction which non removes more colors than it adds

                                //check if we can do a copy and repeat of this move
                                //but in all case we remove the last move for add into check_copy array

                                std::vector<std::vector<int>> check_copy = {{i,j}};       
                                //auto check_copy = checkForCopy(id, i,  j,instruction,  len,  pattern, end_solution, &new_current_state,value_index);
                                int new_id = id;
                                for(int ind_n = 0; ind_n < check_copy.size(); ++ind_n){
                                    int prev_id = new_id;
                                    new_id = executeInstruction(new_id, check_copy[ind_n][0],check_copy[ind_n][1],instruction,  len,  pattern, end_solution, &new_current_state,value_index);
                                }
                                

                                //now we can call the recursion
                                auto pair_rec = bfs_algo_program(istructions,patterns, memory_program, new_id, memory, n,m,new_current_state,end_solution,(n_iter+1),value_index);   
                                int a = pair_rec.first;
                                if(min_value > (1+a)){
                                    auto actualProg = buildInstruction(check_copy,len,instruction,pattern);
                                    best_prog =  actualProg + "\n" + pair_rec.second;
                                }  
                                    
                                    
                                min_value = std::min(min_value, (1+a));     //check if is better than actual min

                                
                                if(copy_instruction){
                                    new_current_state = current_state; 

                                    auto check_copy = checkForCopy(id, i,  j,instruction,  len,  pattern, end_solution, &new_current_state,value_index);                                    
                                    new_id = id;
                                    for(int ind_n = 0; ind_n < check_copy.size(); ++ind_n){
                                        i = check_copy[ind_n][0];
                                        j = check_copy[ind_n][1];
                                        
                                        new_id = executeInstruction(new_id, check_copy[ind_n][0],check_copy[ind_n][1],instruction,  len,  pattern, end_solution, &new_current_state,value_index);


                                        int prev_id = new_id;

                                        auto pair_rec = bfs_algo_program(istructions,patterns,memory_program, new_id, memory, n,m,new_current_state,end_solution,(n_iter+1),value_index);   
                                        int a = pair_rec.first;
                                        if(min_value > (1+a)){
                                            auto actualProg = buildInstruction(check_copy,len,instruction,pattern);
                                            best_prog =  actualProg + "\n" + pair_rec.second;
                                        } 

                                        if(n_iter == 1 && ind_n == check_copy.size()-1 && id == 1023 ){
                                            printArray(new_current_state);
                                            printf("num iter = %d\n",n_iter);
                                        }

                                        min_value = std::min(min_value, (1+a));
                                    }
                                    copy_instruction = false;
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
    //std::string path = "./Graph/miniGraph_6.txt";
    std::string path = "./Graph/TestGraph.txt";
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



    
    
    std::vector<std::vector<int>> TOT_istructions = {{0,0,1},{0,1,0},{0,-1,1},{0,-1,-1},{2,0,1,-1,0,0,-1},{2,-1,0,-1,0,0,-1,0,-1},{2,-1,0,-1,0,0,1,0,1}, {2,0,-1,0,-1,-1,0,-1,0},{2,0,1,0,1,-1,0,-1,0},{0,-1,1,1,1},{0,1,1,-1,1},{1,1,1,1,-1},{1,1,-1,1,1}};
    std::vector<std::vector<int>> TOT_patterns = generatePatterns(4);
    std::string start_prog = "";


    int time = 0;
    std::vector<Individual> Population;

    printf("Initialization of individuals START\n");

    for(int i = 0; i < NUMBER_INDIVIDUAL; i++){
        Individual indiv_tmp = generateRandom(NUMBER_GENE, TOT_patterns);
        auto memory_program_tmp = memory_program;
        auto memory_tmp = memory;
        auto voidMat_tmp = voidMat;
        int fit = bfs_algo_program(indiv_tmp.instruction, indiv_tmp.pattern,memory_program_tmp, 0, memory_tmp,  n,  n, voidMat_tmp, &V, 0,map_value).first;
        printf("%d  ", fit);
        indiv_tmp.fitness = fit;
        Population.push_back(indiv_tmp);
    }
    printf("Initialization of individuals COMPLETE\n");



    // Individual solutionIndividual;
    // solutionIndividual.instruction = {{0,0,1},{0,1,0},{0,-1,1},{0,-1,-1},{2,-1,0,-1,0,0,-1,0,-1},{2,-1,0,-1,0,0,1,0,1}};
    // solutionIndividual.pattern = {{1,2},{1},{2},{1,1,2},{3},{4}};
    // auto memory_program_tmp = memory_program;
    // auto memory_tmp = memory;
    // auto voidMat_tmp = voidMat;
    // int fit = bfs_algo_program(solutionIndividual.instruction, solutionIndividual.pattern,memory_program_tmp, 0, memory_tmp,  n,  n, voidMat_tmp, &V, 0,map_value).first;
    // solutionIndividual.fitness = fit;
    // printf("fit = %d, fitness %d\n",fit,solutionIndividual.fitness );
    // return 1;

    printf("Genetic Algo START\n");
    auto start = std::chrono::high_resolution_clock::now();

    while(time < 5){
        
        
        auto Parents = selectINdividuals(Population, NUMBER_PARENT);
        std::vector<Individual> Childs;
        while(Parents.size() > 0){
            auto p1 = Parents[Parents.size() - 1];
            Parents.pop_back();
            auto p2 = Parents[Parents.size() - 1];
            Parents.pop_back();
            
            auto child12 = reproduction(p1, p2, TOT_patterns);
            Childs.push_back(child12[0]);
            Childs.push_back(child12[1]);
        }

        killPopulation((NUMBER_INDIVIDUAL-NUMBER_PARENT), Population);

        int min = 10000;

        for(int i = 0; i < Childs.size(); ++i){
            auto memory_program_tmp = memory_program;
            auto memory_tmp = memory;
            auto voidMat_tmp = voidMat;
            int fit = bfs_algo_program(Childs[i].instruction, Childs[i].pattern,memory_program_tmp, 0, memory_tmp,  n,  n, voidMat_tmp, &V, 0,map_value).first;
            min = std::min(min, fit);
            Childs[i].fitness = fit;
            Population.push_back(Childs[i]);
        }

        //second kill for useless individual with no fitness
        /*
        KillUselessPeople(Population);
        printf("Population size %lu\n",Population.size());
        while(Population.size() < NUMBER_INDIVIDUAL){
            Individual indiv_tmp = generateRandom(NUMBER_GENE, TOT_patterns);
            int fit = bfs_algo_program(indiv_tmp.instruction, indiv_tmp.pattern,memory_program, 0, memory,  n,  n, voidMat, &V, 0,map_value).first;
            indiv_tmp.fitness = fit;
            Population.push_back(indiv_tmp);
        }*/

        printf("Time = %d MIN = %d, TOT_IND = %lu\n",time, min, Population.size());

        time += 1;
    }

    printf("Genetic Algo END\n");
    
    std::sort(Population.begin(), Population.end(), [](Individual& a, Individual& b) {
        return a.fitness < b.fitness;
    });


    //Population[1].instruction
    //Population[0].pattern
    auto res_pair = bfs_algo_program(Population[1].instruction,Population[0].pattern, memory_program,0, memory,  n,  n, voidMat, &V, 0,map_value);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration_sec = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Tempo di esecuzione: " << duration_sec.count() << " secondi" << std::endl;


    int res = res_pair.first;
    printf("value Final = %d\n", res);
    printf("______________________________ Program ______________________________\n%s", res_pair.second.c_str());
    printf("\n");
    auto prog_arr = parseString(res_pair.second);
    
    
        
}

//TODO bug square prima