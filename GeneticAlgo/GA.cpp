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
int NOTCOUNT = 100;




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



int FillWithPerfect(std::vector<std::vector<int>> index_nodes, std::vector<int> instruction, std::vector<int> pattern,int len, int id,std::vector<std::vector<int>> & current_state,std::vector<std::vector<int>> & end_state, int n, int m, std::vector<int> & value_index){
    int new_id = id;
    for(int i = 0; i < index_nodes.size(); ++i){
        new_id = executeInstruction(new_id, index_nodes[i][0],index_nodes[i][1],instruction,  len,  pattern, &end_state, &current_state,value_index);
    }
    return new_id;
}


int getFitness(std::vector<std::vector<int>> old_best_node, std::vector<std::vector<int>> fake_hash_old,int minLevel, bool same_level, int current_id, int max_id, int number_inst, std::vector<std::vector<int>> instructions, std::vector<int> lengts, std::vector<std::vector<int>> patterns,std::vector<int> & memory,std::vector<std::vector<int>> & current_state,std::vector<std::vector<int>> & end_state, int n, int m, std::vector<int> & value_index){
    std::pair<std::vector<std::vector<int>> ,std::vector<std::vector<int>>> best_first;
    std::vector<std::vector<int>> node_pos;
    std::vector<std::vector<int>> fake_hash;

    if(!same_level && minLevel > number_inst){
        if(number_inst){
            auto actualProg = buildInstruction({{0,0}},lengts[number_inst],instructions[number_inst],patterns[number_inst]);

        }
        best_first = checkForBESTmove(instructions[number_inst], lengts[number_inst], patterns[number_inst],  &end_state, &current_state);
        node_pos = best_first.first;
        fake_hash = best_first.second;
    }else{
        node_pos = old_best_node;
        fake_hash = fake_hash_old;
    }
    // END THE THIRD MOVE APPLIED
    if(number_inst == minLevel){
        int end_id = current_id;
        auto new_current_state = current_state;
        auto allCopy = checkForCopy(end_id, 0,0, instructions[number_inst], lengts[number_inst], patterns[number_inst], &end_state, &new_current_state, value_index);
        for(int i = 0; i < allCopy.size(); ++i){
            end_id = executeInstruction(end_id, allCopy[i][0],allCopy[i][1],instructions[number_inst],  lengts[number_inst],  patterns[number_inst], &end_state, &new_current_state,value_index);
        }
        if(max_id != end_id){
            return 100;
        }
        return (minLevel+1);
    }


    //CHECK WITH ONLY PERFECT COPY
    int current_best_1 = 100;
    if(!same_level){
        auto new_id_f = current_id;
        auto new_current_state_f = current_state;
        new_id_f = FillWithPerfect(node_pos, instructions[number_inst],  patterns[number_inst],lengts[number_inst], new_id_f,new_current_state_f,end_state,n,m,value_index);
        if(new_id_f == max_id){
            return (number_inst + 1);
        }
        current_best_1 = getFitness({{}},{{}},minLevel, false, new_id_f, max_id, (number_inst+1), instructions, lengts, patterns, memory, new_current_state_f, end_state, n,  m,value_index);
        
        if(number_inst == 0){
            printArray(new_current_state_f);
            printf("\n");
        }
    }
    


    //STAGE RECURSIVE
    int best = current_best_1;
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < m; ++j){
            if(fake_hash[i][j] != -1){
                auto new_current_state = current_state;

                //If move doesn't Color Continue
                int checkColor = checkColorOne(current_id, i, j,  instructions[number_inst], lengts[number_inst], patterns[number_inst], &end_state,  &new_current_state);
                
                
                if(checkColor < 0){
                    continue;
                }


                //Now execute new instruction
                int new_id = executeInstruction(current_id, i,j,instructions[number_inst],  lengts[number_inst],  patterns[number_inst], &end_state, &new_current_state,value_index);
                
                //IF we already see the coloration continue, this branch we have already compute
                


                memory[new_id] = 1;
                auto current_fake_hash = fake_hash;
                current_fake_hash[i][j] = -1;

                //iterate on the same three-level
                int current_best_2 = getFitness(node_pos,current_fake_hash,minLevel, true, new_id, max_id, number_inst, instructions, lengts, patterns, memory, new_current_state, end_state, n,  m,value_index);

                
                //Before start new level of three we need to fill with perfect color (doesn't remove any coloration)
                new_id = FillWithPerfect(node_pos, instructions[number_inst],  patterns[number_inst],lengts[number_inst], new_id,new_current_state,end_state,n,m,value_index);
                if(new_id == max_id){
                    return (number_inst + 1);
                }

                //iterate into new level

                int current_best_1 = getFitness({{}},{{}},minLevel, false, new_id, max_id, (number_inst+1), instructions, lengts, patterns, memory, new_current_state, end_state, n,  m,value_index);

                int current_best = std::min(current_best_1,current_best_2);
                if(best > current_best){
                    best = current_best;
                }
                


            }
        }
    }
    return best;
}



std::vector<std::vector<int>> combinations(int n) {
    std::vector<std::vector<int>> result;
    std::vector<int> temp(n);
    for (int i = 0; i < n; i++) {
        temp[i] = i;
    }
    result.push_back(temp);
    while (next_permutation(temp.begin(), temp.end())) {
        result.push_back(temp);
    }
    return result;
}


int main(int argc, char *argv[])
{   
    //TODO 4

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

    //DEBUG TODO
  
    
    std::vector<std::vector<int>>  inst = {{0,0,1},{2,0,1,-1,0,0,-1}};
    std::vector<std::vector<int>>  patt = {{4,3},{2,1}};


    std::vector<std::vector<int>>  inst2 = {{0,0,1},{0,0,1},{0,0,1}};
    std::vector<std::vector<int>>  patt2 = {{4,3},{2,1},{3}};
    std::vector<int> len2 = {2,2,1};

    auto start_p = std::chrono::high_resolution_clock::now();

    auto fit =  getFitness({{}}, {{}},1, false, 0,  max_id, 0, inst2, len2, patt2, memory, voidMat, V, n, n, map_value);

    

    auto end_p = std::chrono::high_resolution_clock::now();
    auto duration_sec_p = std::chrono::duration_cast<std::chrono::seconds>(end_p - start_p);
    std::cout << "Tempo di esecuzione: " << duration_sec_p.count() << " secondi" << std::endl;

    printf("%d fit\n", fit);
    //printf("______________________________ Program ______________________________\n%s", fit.second.c_str());
    return 1;
    
    /*
    int len_t = 3;
    auto check_copy = checkForCopy(0, 0,  0,inst[1],  len_t,  patt[1], &V, &voidMat,map_value);
    for(int i = 0; i < check_copy.size(); i++){
        printf("{%d,%d} ", check_copy[i][0],check_copy[i][1]);
    }
    printf("\n");
    std::vector<std::vector<int>> new_current_state = voidMat; 
    int new_id = 0;
    int i = 0;
    int j = 0;
    for(int ind_n = 0; ind_n < check_copy.size(); ++ind_n){
        i = check_copy[ind_n][0];
        j = check_copy[ind_n][1];
        int number_new = executeInstruction_number(0, i,  j,inst[1],  len_t,  patt[1], &V, &new_current_state);

        if(number_new <= 0){
            continue;
        }

        int prev_id = new_id;
        new_id = executeInstruction(new_id, check_copy[ind_n][0],check_copy[ind_n][1],inst[1],  len_t,  patt[1], &V, &new_current_state,map_value);
    }
    printf("id = %d\n",new_id);
    new_id = executeInstruction(new_id, 1,0,inst[0],  3,  patt[0], &V, &new_current_state,map_value);
    
    printArray(new_current_state);
    



    //int fit = bfs_algo_program(inst, patt,memory_program, 0, memory,  n,  n, voidMat, &V, 0,map_value).first;
    //printf("%d fit\n", fit);
    return 1;
    //END DEBUG 


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
        */
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
        /*
        printf("Time = %d MIN = %d, TOT_IND = %lu\n",time, min, Population.size());

        time += 1;
    }

    printf("Genetic Algo END\n");
    
    std::sort(Population.begin(), Population.end(), [](Individual& a, Individual& b) {
        return a.fitness < b.fitness;
    });


    auto res_pair = bfs_algo_program(Population[1].instruction,Population[0].pattern, memory_program,0, memory,  n,  n, voidMat, &V, 0,map_value);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration_sec = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "Tempo di esecuzione: " << duration_sec.count() << " secondi" << std::endl;


    int res = res_pair.first;
    printf("value Final = %d\n", res);
    printf("______________________________ Program ______________________________\n%s", res_pair.second.c_str());
    printf("\n");
    auto prog_arr = parseString(res_pair.second);
    
    */
        
}

//TODO bug square prima