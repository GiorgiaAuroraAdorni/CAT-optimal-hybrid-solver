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


std::vector<double> getSimilarityVector(int tot_caselle, int number_inst, std::vector<std::vector<int>> voidMat,std::vector<std::vector<int>> & end_state, std::vector<Instruction> Instructions, int idx_inst, std::vector<int> value_index){

    std::vector<double> result(number_inst, 0); 

    auto fist_state = voidMat;
    int tot_colored = 0;

    auto allCopy = checkForCopy(0, 0,0, Instructions[idx_inst].instruction, Instructions[idx_inst].len, Instructions[idx_inst].pattern, &end_state, &fist_state, value_index);
    for(int i = 0; i < allCopy.size(); ++i){
        executeInstruction(0, allCopy[i][0],allCopy[i][1],Instructions[idx_inst].instruction,Instructions[idx_inst].len, Instructions[idx_inst].pattern, &end_state, &fist_state,value_index);
    }

    for(int i = 0; i < fist_state.size(); ++i){
        for(int j = 0; j < fist_state.size(); ++j){
            if(fist_state[i][j] != 0 && fist_state[i][j] != 5 && fist_state[i][j] != -1){
                tot_colored += 1;
            }
        }
    }

    for(int i = 0; i < number_inst; ++i){
        auto tmp_state = fist_state;
        auto allCopy = checkForCopy(0, 0,0, Instructions[i].instruction, Instructions[i].len, Instructions[i].pattern, &end_state, &tmp_state, value_index);
       
        
        for(int j = 0; j < allCopy.size(); ++j){
            executeInstruction(0, allCopy[j][0],allCopy[j][1],Instructions[i].instruction,Instructions[i].len, Instructions[i].pattern, &end_state, &tmp_state, value_index);
        }
        
       

        int tmp_colored = 0;

        for(int a = 0; a < tmp_state.size(); ++a){
            for(int b= 0; b < tmp_state.size(); ++b){
                if(tmp_state[a][b] != 0 && tmp_state[a][b] != 5 && tmp_state[a][b] != -1){
                    tmp_colored += 1;
                }
            }
        }

        result[i] = 1- ((static_cast<double>(tmp_colored - tot_colored))/(static_cast<double>(tot_caselle - tot_colored)));

    }

    return result;

}


std::vector<int> findTopMIndices(const std::vector<double>& expectationFirst, int m) {
    std::vector<int> nonZeroIndices;
    for (int i = 0; i < expectationFirst.size(); i++) {
        if (expectationFirst[i] != 0) {
            nonZeroIndices.push_back(i);
        }
    }
    
    // Ordinamento degli indici in base ai valori di expectationFirst corrispondenti
    sort(nonZeroIndices.begin(), nonZeroIndices.end(), [&](int a, int b) {
        return expectationFirst[a] < expectationFirst[b];
    });
    
    // Creazione di un nuovo vector contenente i primi m indici dell'array ordinato
    std::vector<int> topMIndices;
    for (int i = 0; i < nonZeroIndices.size(); i++) {
        if (topMIndices.size() == m) {
            break;
        }
        topMIndices.push_back(nonZeroIndices[i]);
    }
    
    return topMIndices;
}


int main(int argc, char *argv[])
{   
    //TODO 4
    std::ios_base::sync_with_stdio(false);

    //3 solo 3 e non 2
    //2 troppo lento
    //std::string path = "./Graph/miniGraph_1.txt";
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

    auto allPssibleCombination = getPossibleMove(TOT_istructions ,V);

    
/*
    //DEBUG
    Instruction A;
    Instruction B;
    Instruction C;

    A.instruction = {0, 0,1};
    B.instruction = {0, 0,1};
    C.instruction = {0, 0,1};
    A.pattern = {3,4};
    B.pattern = {4};
    C.pattern = {4,3};
    A.len = 2;
    B.len = 1;
    C.len = 2;
    

    std::vector<std::vector<int> > instructions = {A.instruction, B.instruction, C.instruction};
    std::vector<std::vector<int> > patterns = {A.pattern, B.pattern, C.pattern};
    std::vector<int> lengs = {A.len, B.len, C.len};

    int current_sol = getFitness({{}}, {{}},0, false, 0,  max_id, 0, instructions, lengs, patterns, memory, voidMat, V, n, n, map_value);
    printf("%d\n",current_sol);

    return 1;*/





    
    printf("all move are initialized\n");

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
                    //printInstruction(tmp_inst);
                    
                }
            }
        }
    }

    Moves = getPossibleINST(TOT_istructions ,V);

    printf("start do create clusters\n");

    int number_instruction = Moves.size();
    
    auto similaVec1 = getSimilarityVector(total_colored,number_instruction , voidMat, V,  Moves,  0, map_value);

        
    int idxMin = -1;
    int min_prob = 100;
    for(int i = 0; i < similaVec1.size(); ++i){
        if(similaVec1[i] < min_prob){
            idxMin = i;
            min_prob = similaVec1[i];
        }
    }

    auto similaVec2 = getSimilarityVector(total_colored,number_instruction, voidMat, V,  Moves,  idxMin, map_value);

    auto moves_tmp = voidMat;
    std::vector<int> First_cluster;
    std::vector<int> Secon_cluster;
    std::vector<int> Third_cluster;


    for(int i = 0; i < similaVec2.size(); ++i){
        bool flag = true;

        if(similaVec1[i] >= MIN_SIMILARITY){
            First_cluster.push_back(i);
            flag = false;
        }
        if(similaVec2[i] >= MIN_SIMILARITY){
            Secon_cluster.push_back(i);
            flag = false;
        }
        if(flag){
            Third_cluster.push_back(i);
        }

    }

    if(Secon_cluster.size() == 0){
        Secon_cluster = First_cluster;
    }
    if(Third_cluster.size() == 0){
        Third_cluster = Secon_cluster;
    }
    
    printf("Start Expectation Algo\n");

    int TIME = 50;
    int MAXFIRST = 100 > First_cluster.size() ? First_cluster.size() : 100;
    int TRIAL = 15;
    int TRIAL2 = 50;
    double EXPLORATION = 0.75;
    int min = 100;
    int min_layer = 2;

    std::random_device rd;
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<int> distr1(0, First_cluster.size()-1);
    std::uniform_int_distribution<int> distr2(0, Secon_cluster.size()-1);
    std::uniform_int_distribution<int> distr3(0, Third_cluster.size()-1);
    std::uniform_int_distribution<int> distr4(0, Moves.size()-1);

    std::uniform_real_distribution<double> distr_ex(0.0, 1.0); 
    

    //TODO GESTIRE MENO LIVELLI
    while(TIME > 0){
        std::vector<int> bestNode;
        std::vector<double> expectationFirst(First_cluster.size(), 0);


        for(int f = 0; f < MAXFIRST; ++f){

            int i = distr1(gen);

            if(expectationFirst[i] != 0.0){
                while(expectationFirst[i] == 0.0){
                    i += 1;
                    if(i >= First_cluster.size()){
                        i = 0;
                    }
                }
            }
            int sum_tmp = 0;

                

            for(int t = 0; t < TRIAL; ++t){
                int random_one = distr2(gen);
                int random_two = distr3(gen);
                Instruction A = Moves[First_cluster[i]];
                Instruction B = Moves[Secon_cluster[random_one]];
                Instruction C = Moves[Third_cluster[random_two]];

                std::vector<std::vector<int> > instructions = {A.instruction, B.instruction, C.instruction};
                std::vector<std::vector<int> > patterns = {A.pattern, B.pattern, C.pattern};
                std::vector<int> lengs = {A.len, B.len, C.len};


                
                auto memory_tmp = memory;
                auto void_mat_tmp = voidMat;

                int current_fit = getFitness({{}}, {{}},min_layer, false, 0,  max_id, 0, instructions, lengs, patterns, memory_tmp, void_mat_tmp, V, n, n, map_value);
                
                

                if(min > current_fit){
                    min = current_fit;
                    if(min < 4){
                        if(min == 1){
                            break;
                        }
                        min_layer = current_fit - 2;
                    }
                }
                sum_tmp += current_fit;
                
                
            }

            expectationFirst[i] = static_cast<double>(sum_tmp)/(static_cast<double>(TRIAL));
        }

        

        auto top_int = findTopMIndices(expectationFirst, 5);

        for(int i = 0; i < TRIAL2; ++i){
             for(int j = 0; j < top_int.size(); ++j){
                int random_one = distr2(gen);
                int random_two = distr3(gen);
                double exploration = distr_ex(gen);

                Instruction A = Moves[First_cluster[top_int[j]]];
                Instruction B = Moves[Secon_cluster[random_one]];
                Instruction C = Moves[Third_cluster[random_two]];

                if(exploration >= EXPLORATION){
                    int explor_move = distr4(gen);
                    B = Moves[explor_move];
                    
                    int explor_move_2 = distr4(gen);
                    C = Moves[explor_move_2];
                }

                std::vector<std::vector<int> > instructions = {A.instruction, B.instruction, C.instruction};
                std::vector<std::vector<int> > patterns = {A.pattern, B.pattern, C.pattern};
                std::vector<int> lengs = {A.len, B.len, C.len};
                
                auto memory_tmp = memory;
                auto void_mat_tmp = voidMat;
                int current_sol = getFitness({{}}, {{}},min_layer, false, 0,  max_id, 0, instructions, lengs, patterns, memory_tmp, void_mat_tmp, V, n, n, map_value);
                
                if(min > current_sol){
                    min = current_sol;
                    if(min == 1){
                        break;
                    }
                    min_layer = current_sol - 2;
                }
            }
        }

        printf("min = %d\n", min);

        TIME -= 1;
    }


    return 1;

    printf("FIRST CLUSTER \n");
    for(int i = 0; i < First_cluster.size(); ++i){
        printInstruction(Moves[First_cluster[i]]); 
        printf("prob = %f\n", similaVec1[First_cluster[i]]);
    }
    printf("\n\n");
    printf("SECOND CLUSTER \n");
    for(int i = 0; i < Secon_cluster.size(); ++i){
        printInstruction(Moves[Secon_cluster[i]]); 
        printf("prob = %f\n", similaVec2[Secon_cluster[i]]);
    }
    printf("\n\n");
    printf("THIRD CLUSTER \n");
    for(int i = 0; i < Third_cluster.size(); ++i){
        printInstruction(Moves[Third_cluster[i]]); 
        printf("prob = %f\n", similaVec1[Third_cluster[i]]);
        printf("prob = %f\n", similaVec2[Third_cluster[i]]);
    }
    printf("\n\n");

    /*

     std::vector<Instruction> Moves2;
    Moves2.push_back(Moves[0]);
    Moves2.push_back(Moves[1]);
    Moves2.push_back(Moves[3]);
    Moves2.push_back(Moves[12]);
    Moves2.push_back(Moves[5]);
    Moves2.push_back(Moves[20]);

    auto similaVec1 = getSimilarityVector(total_colored,Moves2.size() , voidMat, V,  Moves2,  0, map_value);
    
    int idxMin = -1;
    int min_prob = 100;
    for(int i = 0; i < similaVec1.size(); ++i){
        if(similaVec1[i] < min_prob){
            idxMin = i;
            min_prob = similaVec1[i];
        }
    }

    auto similaVec2 = getSimilarityVector(total_colored,Moves2.size() , voidMat, V,  Moves2,  idxMin, map_value);

    auto moves_tmp = voidMat;
    std::vector<int> First_cluster;
    std::vector<int> Secon_cluster;
    std::vector<int> Third_cluster;

    for(int i = 0; i < similaVec2.size(); ++i){
        bool flag = true;

        if(similaVec1[i] >= MIN_SIMILARITY){
            First_cluster.push_back(i);
            flag = false;
        }
        if(similaVec2[i] >= MIN_SIMILARITY){
            Secon_cluster.push_back(i);
            flag = false;
        }
        if(flag){
            Third_cluster.push_back(i);
        }

    }

    printf("FIRST CLUSTER \n");
    for(int i = 0; i < First_cluster.size(); ++i){
        printInstruction(Moves2[First_cluster[i]]); 
        printf("prob = %f\n", similaVec1[First_cluster[i]]);
    }
    printf("\n\n");
    printf("SECOND CLUSTER \n");
    for(int i = 0; i < Secon_cluster.size(); ++i){
        printInstruction(Moves2[Secon_cluster[i]]); 
        printf("prob = %f\n", similaVec2[Secon_cluster[i]]);
    }
    printf("\n\n");
    printf("THIRD CLUSTER \n");
    for(int i = 0; i < Third_cluster.size(); ++i){
        printInstruction(Moves2[Third_cluster[i]]); 
        printf("prob = %f\n", similaVec1[Third_cluster[i]]);
        printf("prob = %f\n", similaVec2[Third_cluster[i]]);
    }
    printf("\n\n");

*/

    
    //auto similaVec1 = getSimilarityVector(total_colored,number_instruction , voidMat, V,  Moves,  0, map_value);

    
}

