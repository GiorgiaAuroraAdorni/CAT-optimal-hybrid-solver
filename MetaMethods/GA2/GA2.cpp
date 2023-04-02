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

#include "./FileReader.hpp"
#include "./instruction.hpp"
#include "./individual.hpp"
#include "./Move.hpp"
#include "./Patterns.hpp"
#include "./HashFile.hpp"
#include "./ProgramString.hpp"


double MIN_SIMILARITY = 0.75;



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

int SECONDTOBREAK = 10;

int getFitness(std::time_t start_time, std::vector<std::vector<int>> old_best_node, std::vector<std::vector<int>> fake_hash_old,int minLevel, bool same_level, int current_id, int max_id, int number_inst, std::vector<std::vector<int>> instructions, std::vector<int> lengts, std::vector<std::vector<int>> patterns,std::vector<int> & memory,std::vector<std::vector<int>> & current_state,std::vector<std::vector<int>> & end_state, int n, int m, std::vector<int> & value_index){
    std::time_t current_time = std::time(nullptr);
    std::time_t elapsed_time = current_time - start_time;
    if (elapsed_time >= SECONDTOBREAK) {
        return 100;
    } 

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
        current_best_1 = getFitness(start_time,{{}},{{}},minLevel, false, new_id_f, max_id, (number_inst+1), instructions, lengts, patterns, memory, new_current_state_f, end_state, n,  m,value_index);
        
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
                
                
                if(new_id == current_id){
                    continue;
                }
                if(new_id < 0){
                    continue;
                }

                
                //IF we already see the coloration continue, this branch we have already compute
                

                memory[new_id] = 1;
                auto current_fake_hash = fake_hash;
                current_fake_hash[i][j] = -1;

                //iterate on the same three-level
                int current_best_2 = getFitness(start_time,node_pos,current_fake_hash,minLevel, true, new_id, max_id, number_inst, instructions, lengts, patterns, memory, new_current_state, end_state, n,  m,value_index);

                
                //Before start new level of three we need to fill with perfect color (doesn't remove any coloration)
                new_id = FillWithPerfect(node_pos, instructions[number_inst],  patterns[number_inst],lengts[number_inst], new_id,new_current_state,end_state,n,m,value_index);
                if(new_id == max_id){
                    return (number_inst + 1);
                }


                //iterate into new level

                int current_best_1 = (memory[new_id] != -1) ? 100 : getFitness(start_time,{{}},{{}},minLevel, false, new_id, max_id, (number_inst+1), instructions, lengts, patterns, memory, new_current_state, end_state, n,  m,value_index);

                int current_best = std::min(current_best_1,current_best_2);
                if(best > current_best){
                    best = current_best;
                }
                


            }else{
            }
        }
    }
    return best;
}




int main(int argc, char *argv[])
{   
    //TODO 4
    std::ios_base::sync_with_stdio(false);

    //3 solo 3 e non 2
    //2 troppo lento
    //std::string path = "./Graph/miniGraph_2.txt";
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


    

    
    //TODO NON VEDO L
    std::vector<std::vector<int>> TOT_istructions = {{0,0,1},{0,1,0},{0,-1,1},{0,-1,-1},{2,0,1,-1,0,0,-1},{2,-1,0,-1,0,0,-1,0,-1},{2,-1,0,-1,0,0,1,0,1}, {2,0,-1,0,-1,-1,0,-1,0},{2,0,1,0,1,-1,0,-1,0},{0,-1,1,1,1},{0,1,1,-1,1},{1,1,1,1,-1},{1,1,-1,1,1}};
    std::string start_prog = "";
    
    auto Moves = getPossibleINST(TOT_istructions ,V);


    int NUMBER_INDIVIDUAL = 100;
    int NUMBER_PARENT = 80;
    int NUMBER_GENE = 3;
    int TIME = 0;
    int min_layer = 2;
    std::vector<Individual> Population;
    Individual best_individuo;

    int min_best = 1000000;

    std::time_t start_algo = std::time(nullptr);

    for(int i = 0; i < NUMBER_INDIVIDUAL; i++){
        Individual indiv_tmp = generateRandom(NUMBER_GENE, Moves);
        
        std::vector<std::vector<int> > instructions = {indiv_tmp.instruction[0].instruction, indiv_tmp.instruction[1].instruction, indiv_tmp.instruction[2].instruction};
        std::vector<std::vector<int> > patterns = {indiv_tmp.instruction[0].pattern, indiv_tmp.instruction[1].pattern, indiv_tmp.instruction[2].pattern};
        std::vector<int> lengs = {indiv_tmp.instruction[0].len, indiv_tmp.instruction[1].len, indiv_tmp.instruction[2].len};
        
        auto memory_tmp = memory;
        auto void_mat_tmp = voidMat;
        

        std::time_t start_time = std::time(nullptr);
        int fit = getFitness(start_time,{{}}, {{}},min_layer, false, 0,  max_id, 0, instructions, lengs, patterns, memory_tmp, void_mat_tmp, V, n, n, map_value);
                

        indiv_tmp.fitness = fit;

        
        if(fit == 3){
            min_layer = 1;
            best_individuo = indiv_tmp;
        }
        if(fit == 2){
            min_layer = 0;
            TIME = 100;
            best_individuo = indiv_tmp;
            break;
        }          

        min_best = std::min(min_best, fit);
        Population.push_back(indiv_tmp);
    }

    
    while(TIME < 100){
        
        auto Parents = selectINdividuals(Population, NUMBER_PARENT);
        std::vector<Individual> Childs;
        while(Parents.size() > 0){
            auto p1 = Parents[Parents.size() - 1];
            Parents.pop_back();
            auto p2 = Parents[Parents.size() - 1];
            Parents.pop_back();
            
            auto child12 = reproduction(p1, p2, Moves);

            Childs.push_back(child12[0]);
            Childs.push_back(child12[1]);
        }

        killPopulation((NUMBER_INDIVIDUAL-NUMBER_PARENT), Population);

        int min = 10000;

        for(int i = 0; i < Childs.size(); ++i){
            

            Individual indiv_tmp = Childs[i];
        
            std::vector<std::vector<int> > instructions = {indiv_tmp.instruction[0].instruction, indiv_tmp.instruction[1].instruction, indiv_tmp.instruction[2].instruction};
            std::vector<std::vector<int> > patterns = {indiv_tmp.instruction[0].pattern, indiv_tmp.instruction[1].pattern, indiv_tmp.instruction[2].pattern};
            std::vector<int> lengs = {indiv_tmp.instruction[0].len, indiv_tmp.instruction[1].len, indiv_tmp.instruction[2].len};
        
            auto memory_tmp = memory;
            auto void_mat_tmp = voidMat;
            std::time_t start_time = std::time(nullptr);
            int fit = getFitness(start_time, {{}}, {{}},min_layer, false, 0,  max_id, 0, instructions, lengs, patterns, memory_tmp, void_mat_tmp, V, n, n, map_value);
          

            indiv_tmp.fitness = fit;

            
            if(fit == 3){
                min_layer = 1;
                best_individuo = indiv_tmp;
            }
            if(fit <= 2){
                min_layer = 0;
                TIME = 100;
                best_individuo = indiv_tmp;
                break;
            }



            
            min_best = std::min(min_best, fit);
            
            Population.push_back(indiv_tmp);
        }
        
        
        std::sort(Population.begin(), Population.end(), [](Individual& a, Individual& b) {
            return a.fitness < b.fitness;
        });

        printf("Time = %d MIN = %d, TOT_IND = %lu\n",TIME, Population[0].fitness, Population.size());

        TIME += 1;
    }

    
    // std::sort(Population.begin(), Population.end(), [](Individual& a, Individual& b) {
    //     return a.fitness < b.fitness;
    // });


    for(int i = 0; i < Moves.size(); ++i){
        printf("find new Best\n");
        int end_id = 0;
        auto new_current_state = voidMat;
        auto allCopy = checkForCopy(end_id, 0,0, Moves[i].instruction, Moves[i].len, Moves[i].pattern, &V, &new_current_state, map_value);
        
        for(int i = 0; i < allCopy.size(); ++i){
            end_id = executeInstruction(end_id, allCopy[i][0],allCopy[i][1], Moves[i].instruction, Moves[i].len, Moves[i].pattern, &V, &new_current_state,map_value);
        }

        if(end_id == max_id){
            best_individuo.instruction[0] = Moves[i];
            best_individuo.fitness = 1;
            break;
        }
    }

    std::time_t current_algo = std::time(nullptr);
    std::time_t elapsed_algo = current_algo - start_algo;

    printf("BEST = %d, Time = %ld \n", best_individuo.fitness,elapsed_algo);
    printInstruction(best_individuo.instruction[0]);
    printInstruction(best_individuo.instruction[1]);
    printInstruction(best_individuo.instruction[2]);

    //printf("%f\n", (2 + )/10.0);
    
}

