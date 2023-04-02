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
#include <algorithm>
#include <unordered_set>
#include <thread>


#include "./FileReader.hpp"
#include "./instruction.hpp"
#include "./Move.hpp"
#include "./Patterns.hpp"
#include "./HashFile.hpp"
#include "./ProgramString.hpp"
#include "./K-means.hpp"



double MIN_SIMILARITY = 0.75;

void print_vector(const std::vector<int>& v) {
    std::cout << "[";
    for (int i = 0; i < v.size(); i++) {
        std::cout << v[i];
        if (i < v.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
}


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

void print_colorful_vector(const std::vector<int>& v) {
    for (int i = 0; i < v.size(); i++) {
        switch (v[i]) {
            case 1:
                std::cout << "\033[31m" << v[i] << "\033[0m ";
                break;
            case 2:
                std::cout << "\033[34m" << v[i] << "\033[0m ";
                break;
            case 3:
                std::cout << "\033[33m" << v[i] << "\033[0m ";
                break;
            case 4:
                std::cout << "\033[32m" << v[i] << "\033[0m ";
                break;
            default:
                std::cout << v[i] << " ";
        }
    }
    std::cout << std::endl;
}


int FillWithPerfect(std::vector<std::vector<int>> index_nodes, std::vector<int> instruction, std::vector<int> pattern,int len, int id,std::vector<std::vector<int>> & current_state,std::vector<std::vector<int>> & end_state, int n, int m, std::vector<int> & value_index){
    int new_id = id;
    for(int i = 0; i < index_nodes.size(); ++i){
        new_id = executeInstruction(new_id, index_nodes[i][0],index_nodes[i][1],instruction,  len,  pattern, &end_state, &current_state,value_index);
    }
    return new_id;
}

int SECONDTOBREAK = 15;

int getFitness(std::time_t start_time, std::vector<std::vector<int>> old_best_node, std::vector<std::vector<int>> fake_hash_old,int minLevel, bool same_level, int current_id, int max_id, int number_inst, std::vector<std::vector<int>> instructions, std::vector<int> lengts, std::vector<std::vector<int>> patterns,std::vector<int> & memory,std::vector<std::vector<int>> & current_state,std::vector<std::vector<int>> & end_state, int n, int m, std::vector<int> & value_index){
    std::time_t current_time = std::time(nullptr);
    std::time_t elapsed_time = current_time - start_time;
    if (elapsed_time >= SECONDTOBREAK) {
        return 100;
    } 

    int current_best_1 = 1000;

    if(number_inst > 0){
        bool flag_max = false;
        auto current_state_a = current_state;
        auto current_state_b = current_state;
        auto current_state_c = current_state;
        auto current_state_d = current_state;

        auto mvl = getMirrorVeritcalLeft(current_id,  end_state,  current_state_a,value_index);
        flag_max = flag_max || (mvl.first == max_id);
        
        auto mvr = getMirrorVeritcalRig(current_id,  end_state,  current_state_b,value_index);
        flag_max = flag_max || (mvr.first == max_id);
        
        auto mht = getMirrorHorizontalTop(current_id,  end_state,  current_state_c,value_index);
        flag_max = flag_max || (mht.first == max_id);
        
        auto mhd = getMirrorHorizontalDown(current_id,  end_state,  current_state_d,value_index);
        flag_max = flag_max || (mhd.first == max_id);
        
        if(flag_max){
            return number_inst+1;
        }
        if(number_inst < minLevel){
            if(current_id != mvl.first){
                int mlv_res = getFitness(start_time,{{}},{{}},minLevel, false, mvl.first, max_id, (number_inst+1), instructions, lengts, patterns, memory, current_state_a, end_state, n,  m,value_index);
                current_best_1 = std::min(current_best_1, mlv_res);
            }
            
            if(current_id != mvr.first){
                int mlr_res = getFitness(start_time,{{}},{{}},minLevel, false, mvr.first, max_id, (number_inst+1), instructions, lengts, patterns, memory, current_state_b, end_state, n,  m,value_index);
                current_best_1 = std::min(current_best_1, mlr_res);
            }
            
            if(current_id != mht.first){
                int mht_res = getFitness(start_time,{{}},{{}},minLevel, false, mht.first, max_id, (number_inst+1), instructions, lengts, patterns, memory, current_state_c, end_state, n,  m,value_index);
                current_best_1 = std::min(current_best_1, mht_res);
            }
            

            if(current_id != mhd.first){
                int mhd_res = getFitness(start_time,{{}},{{}},minLevel, false, mhd.first, max_id, (number_inst+1), instructions, lengts, patterns, memory, current_state_d, end_state, n,  m,value_index);
                current_best_1 = std::min(current_best_1, mhd_res);
            }
            
        
        }
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





int getFitnessPseudo(std::time_t start_time, std::vector<std::vector<int>> old_best_node, std::vector<std::vector<int>> fake_hash_old,int minLevel, bool same_level, int current_id, int max_id, int number_inst, std::vector<std::vector<int>> instructions, std::vector<int> lengts, std::vector<std::vector<int>> patterns,std::vector<int> & memory,std::vector<std::vector<int>> & current_state,std::vector<std::vector<int>> & end_state, int n, int m, std::vector<int> & value_index){

    int end_id = current_id;
    auto new_current_state = current_state;
    auto allCopy = checkForCopy(end_id, 0,0, instructions[number_inst], lengts[number_inst], patterns[number_inst], &end_state, &new_current_state, value_index);
    
    
    for(int i = 0; i < allCopy.size(); ++i){
        end_id = executeInstruction(end_id, allCopy[i][0],allCopy[i][1],instructions[number_inst],  lengts[number_inst],  patterns[number_inst], &end_state, &new_current_state,value_index);
    }
    
    if(number_inst != minLevel){
        return getFitnessPseudo(start_time,{{}},{{}},minLevel, false, end_id, max_id, (number_inst+1), instructions, lengts, patterns, memory, new_current_state, end_state, n,  m,value_index);
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


int bfs_spec(std::time_t start_time, std::vector<std::vector<int>> old_best_node, std::vector<std::vector<int>> fake_hash_old,int minLevel, bool same_level, int current_id, int max_id, int number_inst, std::vector<Instruction> Moves,std::vector<int> memory,std::vector<std::vector<int>> current_state,std::vector<std::vector<int>> end_state, int n, int m, std::vector<int> value_index){
    std::vector<std::vector<int> > instructions;
    std::vector<std::vector<int> > patterns;
    std::vector<int> lengs;

    for(int i = 0; i < Moves.size(); ++i){
        instructions.push_back(Moves[i].instruction);
        patterns.push_back(Moves[i].pattern);
        lengs.push_back(Moves[i].len);
    }

    int res = getFitnessPseudo(start_time, old_best_node,  fake_hash_old, minLevel,  same_level,  current_id,  max_id,  number_inst,instructions,lengs,  patterns, memory,current_state,end_state,  n,  m,value_index);
    return res;
}


int bfs_best(int minLevel, int max_id, std::vector<Instruction> Moves,std::vector<int> memory,std::vector<std::vector<int>> current_state,std::vector<std::vector<int>> end_state, int n, int m, std::vector<int> value_index){
    std::vector<std::vector<int> > instructions;
    std::vector<std::vector<int> > patterns;
    std::vector<int> lengs;

    for(int i = 0; i < Moves.size(); ++i){
        instructions.push_back(Moves[i].instruction);
        patterns.push_back(Moves[i].pattern);
        lengs.push_back(Moves[i].len);
    }
    std::time_t start_time = std::time(nullptr);
    int res = getFitness(start_time, {{}}, {{}}, minLevel,  false, 0,  max_id,  0,instructions,lengs,  patterns, memory,current_state,end_state,  n,  m,value_index);
    return res;
}




int bfs_best_2(int minLevel, int max_id, std::vector<std::vector<Instruction>> Moves,std::vector<int> memory,std::vector<std::vector<int>> current_state,std::vector<std::vector<int>> end_state, int n, int m, std::vector<int> value_index){
    int res = 10;
    for(int i = 0; i < Moves.size(); ++i){
        int res_2 = bfs_best(minLevel,max_id,Moves[i],memory, current_state,end_state,n,m,value_index);
        res = std::min(res, res_2);
    }
    return res;
}





std::vector<std::vector<int>> find_bottom_k_indices(const std::vector<std::vector<double>>& matrix, int k) {
    std::vector<std::vector<int>> result;
    for (const auto& row : matrix) {
        std::vector<int> indices(row.size());
        std::iota(indices.begin(), indices.end(), 0);
        std::partial_sort(indices.begin(), indices.begin() + k, indices.end(), [&](int i, int j) {
            return row[i] < row[j];
        });
        result.push_back(std::vector<int>(indices.begin(), indices.begin() + k));
    }
    return result;
}


std::vector<std::pair<int, int>>  find_k_lowest_indices(const std::vector<std::vector<double>>& matrix, int k) {
    int n_rows = matrix.size();
    int n_cols = matrix[0].size();

    // Crea un vettore di tuple (valore, riga, colonna) per tutti i valori della matrice
    std::vector<std::tuple<double, int, int>> values;
    for (int i = 0; i < n_rows; i++) {
        for (int j = 0; j < n_cols; j++) {
            values.emplace_back(matrix[i][j], i, j);
        }
    }

    // Ordina il vettore di tuple in base al valore (dal più piccolo al più grande)
    std::sort(values.begin(), values.end());

    // Seleziona le prime k tuple (corrispondenti ai k valori più bassi) e ottieni le coppie di indici
    std::vector<std::pair<int, int>> indices;
    for (int i = 0; i < k && i < values.size(); i++) {
        indices.emplace_back(std::get<1>(values[i]), std::get<2>(values[i]));
    }

    return indices;
}

int countDistinctNumbers(const std::vector<std::vector<int>> matrix) {
    std::unordered_set<int> distinctNumbers;
    for (const auto& row : matrix) {
        for (int num : row) {
            if(num != -1){
                distinctNumbers.insert(num);
            }
        }
    }
    return distinctNumbers.size();
}


std::vector<std::vector<Instruction>> all_combinations(const std::vector<std::vector<Instruction>>& matrix) {
    std::vector<std::vector<Instruction>> result;
    int n = matrix.size();
    std::vector<int> indices(n, 0);
    while (indices[0] < matrix[0].size()) {
        std::vector<Instruction> combination;
        for (int i = 0; i < n; i++) {
            combination.push_back(matrix[i][indices[i]]);
        }
        result.push_back(combination);
        indices[n-1]++;
        for (int i = n - 1; i > 0; i--) {
            if (indices[i] == matrix[i].size()) {
                indices[i-1]++;
                indices[i] = 0;
            }
        }
    }
    return result;
}


std::vector<std::vector<std::vector<Instruction>>> splitVectorInst(const std::vector<std::vector<Instruction>>& vec, const int K) {
    const int N = vec.size();
    const int chunkSize = std::ceil(static_cast<double>(N) / K);
    std::vector<std::vector<std::vector<Instruction>>> result(K);
    int index = 0;

    for (int i = 0; i < K; i++) {
        const int remaining = N - index;
        const int currentChunkSize = std::min(chunkSize, remaining);
        result[i].resize(currentChunkSize);

        for (int j = 0; j < currentChunkSize; j++) {
            result[i][j] = vec[index++];
        }
    }

    return result;
}

std::vector<double> computeEXPcluster(int i, std::vector<double> expect_matrix, int MIN_LEVEL, int max_id,std::vector<int> const memory,std::vector<std::vector<int>> voidMat,std::vector<std::vector<int>> const & V,std::vector<int> const & map_value, std::vector<int> hash_fake_i, std::vector<std::vector<int>> const & clusters, int EXPLOR_EX,std::uniform_int_distribution<int> distr_cls,std::mt19937 gen,std::vector<Instruction> Moves,std::vector<std::vector<double>> exp_inver_mat){
    int n = voidMat.size();
    for(int j = 0; j < clusters[i].size(); ++j){
        int explor = EXPLOR_EX;
        int sumexp = 0;
        while(explor > 0){
            int tmp_idx = distr_cls(gen);
            auto hash_fake_i_tmp = hash_fake_i;
            std::vector<Instruction> inst;
            inst.push_back(Moves[clusters[i][j]]);

            while(hash_fake_i_tmp.size() > 0){
                

                std::uniform_int_distribution<int> distr_tmp_1(0, hash_fake_i_tmp.size()-1);
                int ll = distr_tmp_1(gen);
                int l = hash_fake_i_tmp[ll];

                std::discrete_distribution<> weighted_distrib(exp_inver_mat[l].begin(),(exp_inver_mat[l].begin()+ clusters[l].size()));
                int k = weighted_distrib(gen);
                inst.push_back(Moves[clusters[l][k]]);
                hash_fake_i_tmp.erase(hash_fake_i_tmp.begin() + ll);
            }

            auto memory_tmp = memory;
            auto void_mat_tmp = voidMat;
            std::time_t start_time = std::time(nullptr);
            int current_sol = bfs_spec(start_time,{{}}, {{}},MIN_LEVEL, false, 0,  max_id, 0, inst, memory_tmp, void_mat_tmp, V, n, n, map_value);

            sumexp += current_sol;
            explor--;
            //printf("curr = %d, level = %d\n\n", current_sol,MIN_LEVEL);
        }
        auto tmp_exp = expect_matrix[j];
        auto new_exp = static_cast<double>(sumexp)/(static_cast<double>(EXPLOR_EX));
        if(tmp_exp == 100){
            tmp_exp = new_exp;
        }
        expect_matrix[j] = (tmp_exp + new_exp)/2;
    }
    return expect_matrix;
}

int TOT_COLOR = 0;

int main(int argc, char *argv[])
{   
    //TODO 4
    std::time_t start_algo = std::time(nullptr);
    
    std::ios_base::sync_with_stdio(false);

    //3 solo 3 e non 2 PERCHE la sol non ha i primi numeri :)
    //2 troppo lento
    //std::string path = "./Graph/miniGraph_3.txt";
    std::string path = "./Graph/TestGraph_5.txt";
    //read file and convert information into a matrix
    auto V = file_reader(path);
    int n = V.size();

    TOT_COLOR = countDistinctNumbers(V);
    printf("we have a total of %d different colors\n", TOT_COLOR);
    
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
    std::vector<Instruction> Moves;

    Moves = getPossibleINST(TOT_istructions ,V);

//DEBUG
    // Instruction A;
    // Instruction B;
    // Instruction C;


    // A.instruction = {2, 0, 1, -1, 0, 0, -1} ;
    // B.instruction = {0, 1, 0};
    // C.instruction = {0, 0, 1};
    // A.pattern = {1,1,2,3};
    // B.pattern = {3,3,4 };
    // C.pattern = {1,2,3};
    // A.len = 1;
    // B.len = 3;
    // C.len = 3;
    
    // // int idaskd = 0;
    // // idaskd = executeInstruction(idaskd, 3,0,A.instruction, A.len, A.pattern, &V, &voidMat,map_value);
    // // idaskd = executeInstruction(idaskd, 3,3,A.instruction, A.len, A.pattern, &V, &voidMat,map_value);
    // // idaskd = executeInstruction(idaskd, 4,3,B.instruction, B.len, B.pattern, &V, &voidMat,map_value);
    // // idaskd = executeInstruction(idaskd, 4,2,B.instruction, B.len, B.pattern, &V, &voidMat,map_value);
    // // idaskd = getMirrorHorizontalDown(idaskd,  V,  voidMat,map_value).first;

    // int fit = bfs_best(2, max_id, {B,A,C}, memory, voidMat, V, n, n, map_value);
    // printf("FIT = %d\n",fit);
    // printArray(voidMat);
    // return 1;
    ///////////


    printf("start do create clusters\n");
    int k = TOT_COLOR - 1;

    if(k == 0){
        return 1;
    }

    //auto clusters = KmeansPlusPlus(k, Moves, V,voidMat,map_value);
    auto clusters = K_means_cos(k, Moves, V,voidMat,map_value);
    printf("end do create clusters\n");

    // for(int i = 0; i < clusters.size(); ++i){
    //         for(int j = 0; j < clusters[i].size(); ++j){
    //             print_colorful_vector(Moves[clusters[i][j]].pattern);
    //         }
    //         printf("\n");
    // }
    // printf("end to create clusters\n");

    
    int max_cols = 0;
    for (const auto& row : clusters) {
        if (row.size() > max_cols) {
            max_cols = row.size();
        }
    }
    std::vector<std::vector<double>> expect_matrix(clusters.size(), std::vector<double>(max_cols, 100));
    std::vector<std::vector<double>> exp_inver_mat(clusters.size(), std::vector<double>(max_cols, 1));
    
    int TIME = 5;
    int EXPLOR_EX = 20;
    int EXPLORE_B = 50;
    int MIN_LEVEL = TOT_COLOR-2;
    int BEST_CHOOS = 4;
    int TOT_B_CHOO = 5;
    int min = TOT_COLOR;
    int old_min = TOT_COLOR;
    std::random_device rd;
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<int> distr_cls(0, clusters.size()-1);
    std::mutex mutex;

    unsigned int MAX_THREAD = std::thread::hardware_concurrency();

    std::vector<int> hash_fake(clusters.size());
    for (int i = 0; i < clusters.size(); i++) {
        hash_fake[i] = i;
    }

    while(TIME > 0){
        printf("start compute Expectation with %lu Thread\n",clusters.size());

        
        int thread_i_exp = 0;
        std::vector<std::thread> threads_expect(clusters.size());
        std::vector<std::vector<double>> results_exp(clusters.size());


        for(int i = 0; i < clusters.size(); ++i){
            std::vector<double> exp_mat_i = expect_matrix[i];
            auto hash_fake_i = hash_fake;
            hash_fake_i.erase(hash_fake_i.begin() + i);
            auto hash_fake_i_tmp = hash_fake_i;
            auto expect_cluster_val = expect_matrix[i];
            threads_expect[i] = std::thread([&results_exp,i, expect_cluster_val,  MIN_LEVEL,  max_id,memory, voidMat,V,map_value,hash_fake_i, clusters,  EXPLOR_EX, distr_cls,gen, Moves, exp_inver_mat]() {
                results_exp[i] =  computeEXPcluster( i, expect_cluster_val,  MIN_LEVEL,  max_id,memory, voidMat,V,map_value,hash_fake_i, clusters,  EXPLOR_EX, distr_cls,gen, Moves, exp_inver_mat);;

            });
        }


        for (auto& thread : threads_expect) {
            thread.join();
        }
        printf("inverse of expectation\n");
        for(int i = 0; i < results_exp.size(); ++i){
            expect_matrix[i] = results_exp[i];
            for(int j = 0; j < expect_matrix[i].size(); ++j){
                exp_inver_mat[i][j] = 100/expect_matrix[i][j];
            }
        }

        printf("Get best in clusters\n");


        auto best_value = find_bottom_k_indices(expect_matrix, BEST_CHOOS);

        /*
        for(int i = BEST_CHOOS; i <= TOT_B_CHOO; ++i){
            for(int j = 0; j < best_value.size();++j){
                std::discrete_distribution<> weighted_distrib(exp_inver_mat[j].begin(),(exp_inver_mat[j].begin()+ clusters[j].size()));
                int k = weighted_distrib(gen);
                best_value[j].push_back(clusters[j][k]);
            }
        }*/

        // for(int in = 0; in < best_value.size(); in++){
        //     for(int jn = 0; jn < best_value[in].size(); ++jn){
        //         printInstruction(best_value[in][jn]);
        //     }
        // }

        std::vector<std::vector<Instruction>> allInstruction;
        
        for(int i = 0; i < best_value.size(); ++i){
            allInstruction.push_back({});
            for(int j = 0; j < best_value[i].size(); ++j){
                allInstruction[i].push_back(Moves[clusters[i][best_value[i][j]]]);
            }
        }

        auto allCombination = all_combinations(allInstruction);
        auto allCombination_K = splitVectorInst(allCombination, MAX_THREAD);
        //printf("number of combination %lu, number of thread %d \n",allCombination_K.size(),MAX_THREAD);

        printf("START THREAD\n");
        int thread_i = 0;
        std::vector<std::thread> threads(MAX_THREAD);
        std::vector<int> results(MAX_THREAD);

        for (int j = 0; j < MAX_THREAD; j++) {
            auto memory_tmp = memory;
            auto void_mat_tmp = voidMat;
            std::time_t start_time = std::time(nullptr);
            threads[j] = std::thread([&results,j, allCombination_K, MIN_LEVEL, max_id, memory_tmp, void_mat_tmp, V, n, map_value]() {
                results[j] = bfs_best_2(MIN_LEVEL, max_id, allCombination_K[j], memory_tmp, void_mat_tmp, V, n, n, map_value);
            });
        }



        for (auto& thread : threads) {
            thread.join();
        }
        

        for(int j = 0; j < results.size(); ++j){
            auto current_sol = results[j];
            
            min = std::min(min, current_sol);
            printf("New sol = %d\n",current_sol);

        }
        
        if(min <= 2){
            break;
        }

        MIN_LEVEL = min - 2;

        if(old_min != min){
            printf("recompute Clusters for new depth\n");
            k = min - 1;
            auto clusters_new = KmeansPlusPlus(k, Moves, V,voidMat,map_value);
            clusters = clusters_new;
            int max_cols = 0;
            for (const auto& row : clusters) {
                if (row.size() > max_cols) {
                    max_cols = row.size();
                }
            }

            std::vector<std::vector<double>> new_expect_matrix(clusters.size(), std::vector<double>(max_cols, 100));
            std::vector<std::vector<double>> new_exp_inver_mat(clusters.size(), std::vector<double>(max_cols, 1));
            expect_matrix = new_expect_matrix;
            exp_inver_mat = new_exp_inver_mat;

            std::vector<int> hash_fake_2(clusters.size());
            for (int i = 0; i < clusters.size(); i++) {
                hash_fake_2[i] = i;
            }

            hash_fake = hash_fake_2;
            printf("%lu, %d\n", clusters.size(), MIN_LEVEL);
        }

        old_min = min;
        TIME -= 1;
    }
    printf("MIN = %d\n", min);
}

