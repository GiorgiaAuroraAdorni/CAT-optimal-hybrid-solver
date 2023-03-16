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
int NUMBER_GENE = 5;
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
            int check_max_col = 100;
            for(int i = 0; i < n; ++i){
                for(int j = 0; j < m; ++j){
                    if(new_current_state[i][j] != 5 && new_current_state[i][j] != 0){
                        check_max_col -= 1;
                    }
                }
            }
            return check_max_col;
        }
        return (minLevel+1);
    }


    //CHECK WITH ONLY PERFECT COPY
    int current_best_1 = 1000;
    if(!same_level){
        auto new_id_f = current_id;
        auto new_current_state_f = current_state;
        new_id_f = FillWithPerfect(node_pos, instructions[number_inst],  patterns[number_inst],lengts[number_inst], new_id_f,new_current_state_f,end_state,n,m,value_index);
        if(new_id_f == max_id){
            return (number_inst + 1);
        }
        current_best_1 = getFitness({{}},{{}},minLevel, false, new_id_f, max_id, (number_inst+1), instructions, lengts, patterns, memory, new_current_state_f, end_state, n,  m,value_index);
        
    }
    

    //STAGE RECURSIVE
    int best = current_best_1;
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < m; ++j){
            if(current_state[i][j] == -1){
                continue;
            }
            if(fake_hash.size()> 0 && fake_hash[i][j] != -1){
                auto new_current_state = current_state;

                //If move doesn't Color Continue
                int checkColor = checkColorOne(current_id, i, j,  instructions[number_inst], lengts[number_inst], patterns[number_inst], &end_state,  &new_current_state);
                
                
                if(checkColor < 0){
                    continue;
                }


                //Now execute new instruction
                int new_id = executeInstruction(current_id, i,j,instructions[number_inst],  lengts[number_inst],  patterns[number_inst], &end_state, &new_current_state,value_index);
                if(new_id < 0){
                    continue;
                }
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

                int current_best_1 = (memory[new_id] != -1) ? 100 : getFitness({{}},{{}},minLevel, false, new_id, max_id, (number_inst+1), instructions, lengts, patterns, memory, new_current_state, end_state, n,  m,value_index);

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
    std::ios_base::sync_with_stdio(false);

    std::string path = "./Graph/miniGraph_1.txt";
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
    
    std::vector<std::vector<int>>  inst2 = {{2, 0, 1, -1, 0, 0, -1, },{0, 0, 1, },{1, 1, 1, 1, -1, },};
    std::vector<std::vector<int>>  patt2 = {{4, 4, 3, 3, },{3, },{3, },};
    std::vector<int> len2 = {1,1,1};

    auto start_p = std::chrono::high_resolution_clock::now();
    printf("GAY\n");
    auto fit =  getFitness({{}}, {{}},2, false, 0,  max_id, 0, inst2, len2, patt2, memory, voidMat, V, n, n, map_value);

    

    auto end_p = std::chrono::high_resolution_clock::now();
    auto duration_sec_p = std::chrono::duration_cast<std::chrono::seconds>(end_p - start_p);
    std::cout << "Tempo di esecuzione: " << duration_sec_p.count() << " secondi" << std::endl;

    printf("%d fit\n", fit);
    //printf("______________________________ Program ______________________________\n%s", fit.second.c_str());
    return 1;*/
    
    





    
    
    std::vector<std::vector<int>> TOT_istructions = {{0,0,1},{0,1,0},{0,-1,1},{0,-1,-1},{2,0,1,-1,0,0,-1},{2,-1,0,-1,0,0,-1,0,-1},{2,-1,0,-1,0,0,1,0,1}, {2,0,-1,0,-1,-1,0,-1,0},{2,0,1,0,1,-1,0,-1,0},{0,-1,1,1,1},{0,1,1,-1,1},{1,1,1,1,-1},{1,1,-1,1,1}};
    //std::vector<std::vector<int>> TOT_patterns = generatePatterns(4);
    std::string start_prog = "";


    
    auto allPssibleCombination = getPossibleMove(TOT_istructions ,V);

    
    int time = 0;
    int DEPTH = NUMBER_GENE - 1;
    std::vector<Individual> Population;

    printf("Initialization of individuals START\n");
    int min_best = 1000000;

    for(int i = 0; i < NUMBER_INDIVIDUAL; i++){
        Individual indiv_tmp = generateRandom(NUMBER_GENE, allPssibleCombination.first, TOT_istructions, allPssibleCombination.second);
        auto memory_program_tmp = memory_program;
        auto memory_tmp = memory;
        auto voidMat_tmp = voidMat;
        int fit = getFitness({{}}, {{}},DEPTH, false, 0,  max_id, 0, indiv_tmp.instruction, indiv_tmp.lens, indiv_tmp.pattern, memory, voidMat, V, n, n, map_value);
        indiv_tmp.fitness = fit;
        min_best = std::min(min_best, fit);
        Population.push_back(indiv_tmp);
    }

    printf("Initialization of individuals COMPLETE\n");


    printf("Genetic Algo START\n");
    auto start = std::chrono::high_resolution_clock::now();
    
    while(time < 100){
        
        
        auto Parents = selectINdividuals(Population, NUMBER_PARENT);
        std::vector<Individual> Childs;
        while(Parents.size() > 0){
            auto p1 = Parents[Parents.size() - 1];
            Parents.pop_back();
            auto p2 = Parents[Parents.size() - 1];
            Parents.pop_back();
            
            auto child12 = reproduction(p1, p2, allPssibleCombination.first, TOT_istructions, allPssibleCombination.second);

            Childs.push_back(child12[0]);
            Childs.push_back(child12[1]);
        }

        killPopulation((NUMBER_INDIVIDUAL-NUMBER_PARENT), Population);

        int min = 10000;

        for(int i = 0; i < Childs.size(); ++i){
            auto memory_program_tmp = memory_program;
            auto memory_tmp = memory;
            auto voidMat_tmp = voidMat;
            int fit = getFitness({{}}, {{}},DEPTH, false, 0,  max_id, 0, Childs[i].instruction, Childs[i].lens, Childs[i].pattern, memory, voidMat, V, n, n, map_value);
            min = std::min(min, fit);
            Childs[i].fitness = fit;
            Population.push_back(Childs[i]);
        }

        int killed = 0;
        bool kill_mod = false;
        if(false){
            for(int i = 0; i < NUMBER_INDIVIDUAL; i++){
                killed += 1;
                Individual indiv_tmp = generateRandom(NUMBER_GENE, allPssibleCombination.first, TOT_istructions, allPssibleCombination.second);
                auto memory_program_tmp = memory_program;
                auto memory_tmp = memory;
                auto voidMat_tmp = voidMat;
                int fit = getFitness({{}}, {{}},DEPTH, false, 0,  max_id, 0, indiv_tmp.instruction, indiv_tmp.lens, indiv_tmp.pattern, memory, voidMat, V, n, n, map_value);
                indiv_tmp.fitness = fit;
                Population[i] = indiv_tmp;
            }
        }
        
        
        std::sort(Population.begin(), Population.end(), [](Individual& a, Individual& b) {
            return a.fitness < b.fitness;
        });

        printf("Time = %d MIN = %d,Kill = %d, TOT_IND = %lu\n",time, Population[0].fitness,killed, Population.size());

        time += 1;
    }

    printf("Genetic Algo END\n");
    
    std::sort(Population.begin(), Population.end(), [](Individual& a, Individual& b) {
        return a.fitness < b.fitness;
    });

    printf("BEST = %d\n", Population[0].fitness);
    printIndividual(Population[0]);
    /*
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
   return 1;
    








    /*
    std::vector<std::vector<int>> instuhda = {{0,1,0},{2,0,1,-1,0,0,-1}};
    auto aslkdj = getPossibleMove(instuhda ,V);
    
    for(int i = 0; i < instuhda.size(); ++i ){
        printf("MOVE %d \n\n", i);
        for(int j = 0; j < aslkdj.second.size(); ++j){
            printf("LEN %d \n", aslkdj.second[i][j]);
            for(int a = 0; a < aslkdj.first[i].size(); ++a){
                for(int b = 0; b < aslkdj.first[i][a].size(); ++b){
                    for(int c = 0; c < aslkdj.first[i][a][b].size(); ++c){
                        printf("%d ", aslkdj.first[i][a][b][c]);
                    }
                    printf("\n");   
                }
                printf("\n");
            }
            printf("\n");
        }
        printf("\n");
    }

    return 1;
    */



   //DEBUG TODO
  
    /*
    
    std::vector<std::vector<int>>  inst = {{0,0,1},{2,0,1,-1,0,0,-1}};
    std::vector<std::vector<int>>  patt = {{4,3},{2,1}};


    std::vector<std::vector<int>>  inst2 = {{0,0,1},{0,0,1},{0,0,1}};
    std::vector<std::vector<int>>  patt2 = {{4,3},{2,1},{3}};
    std::vector<int> len2 = {2,2,1};

    auto start_p = std::chrono::high_resolution_clock::now();

    auto fit =  getFitness({{}}, {{}},2, false, 0,  max_id, 0, inst2, len2, patt2, memory, voidMat, V, n, n, map_value);

    

    auto end_p = std::chrono::high_resolution_clock::now();
    auto duration_sec_p = std::chrono::duration_cast<std::chrono::seconds>(end_p - start_p);
    std::cout << "Tempo di esecuzione: " << duration_sec_p.count() << " secondi" << std::endl;

    printf("%d fit\n", fit);
    //printf("______________________________ Program ______________________________\n%s", fit.second.c_str());
    return 1;

    */
        
}

//TODO bug square prima