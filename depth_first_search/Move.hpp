#include <string>
#include <vector>

/**
 * @brief this function try to do a move and try all possible repetition of this move
 * 
 * @param node_i the i position of the start node
 * @param node_j the j position of the start node
 * @param istruction array that  contains the instruction to run
 * @param pattern the pattern
 * @param mat the matrix with the table
 * @return int number of move
 */
int moveNoColor(int node_i, int node_j, std::vector<int> istruction, std::vector<int> pattern, std::vector<std::vector<int>> mat){
    
    //initialization of parameter
    int number_move = 0;
    int idx_pattern = 0;
    int idx_istruct = 0;
    int i = node_i;
    int j = node_j;

    int type_of_inst = istruction[idx_istruct];
    int type_one = false;
    int pattern_end = 0;

    istruction.erase(istruction.begin());


    //run the istruction
    while(mat[i][j] == pattern[idx_pattern]){

        //printf("i = %d,  j = %d,   color = %d,     actual color = %d \n",i,j, pattern[idx_pattern], mat[i][j]);

        //check the variable to transform
        //simple move
        i += istruction[(idx_istruct)];
        idx_istruct += 1;
        j += istruction[(idx_istruct)];
        
        if(idx_pattern == pattern.size() - 1){
            pattern_end = 1;
        }

        //check one move complete
        if(pattern_end && type_of_inst != 2 && (type_one || (idx_istruct == istruction.size() - 1))){
            if(type_of_inst != 1 || (type_of_inst == 1 && type_one == 1)){
                number_move += 1;
            }
        }

        if(j >= mat.size() || j < 0 || i >= mat.size() || i < 0){
                break;
        }
        
        idx_istruct += 1;
        idx_pattern += 1;

        //check if we have complet 1 move
        if(idx_istruct >= istruction.size()){
            if(type_of_inst == 2){ //istruction that must end
                number_move += 1;
                break;
            }
            if(type_of_inst == 1){
                type_one = 1;
            }
            idx_istruct = 0;
        }

        if(idx_pattern >= pattern.size()){
            idx_pattern = 0;
        }
    }
    printf("res = %d\n",number_move );
    return number_move;
    //printf("i = %d,  j = %d,   color = %d,     actual color = %d \n\n",i,j, pattern[idx_pattern], mat[i][j]);
}




/**
 * @brief this function try to do a move and try all possible repetition of this move
 * 
 * @param node_i the i position of the start node
 * @param node_j the j position of the start node
 * @param istruction array that  contains the instruction to run
 * @param pattern the pattern
 * @param mat the matrix with the table
 * @return int number of move
 */
/*
int moveNoPattern(int node_i, int node_j, std::vector<int> istruction, std::vector<std::vector<int>> mat){
    
    //initialization of parameter
    std::vector<int> pattern;
    int number_move = 0;
    int idx_pattern = 0;
    int idx_istruct = 0;
    int i = node_i;
    int j = node_j;

    int type_of_inst = istruction[idx_istruct];
    int type_one = false;

    istruction.erase(istruction.begin());


    //run the istruction
    while(mat[i][j] == pattern[idx_pattern]){

        //check the variable to transform
        //simple move
        i += istruction[(idx_istruct)];
        idx_istruct += 1;
        j += istruction[(idx_istruct)];
        
        if(idx_pattern == pattern.size() - 1){
            pattern_end = 1;
        }

        //check one move complete
        if(pattern_end && type_of_inst != 2 && (type_one || (idx_istruct == istruction.size() - 1))){
            if(type_of_inst != 1 || (type_of_inst == 1 && type_one == 1)){
                number_move += 1;
            }
        }

        if(j >= mat.size() || j < 0 || i >= mat.size() || i < 0){
                break;
        }
        
        idx_istruct += 1;
        idx_pattern += 1;

        //check if we have complet 1 move
        if(idx_istruct >= istruction.size()){
            if(type_of_inst == 2){ //istruction that must end
                number_move += 1;
                break;
            }
            if(type_of_inst == 1){
                type_one = 1;
            }
            idx_istruct = 0;
        }

        if(idx_pattern >= pattern.size()){
            idx_pattern = 0;
        }
    }
    printf("res = %d\n",number_move );
    return number_move;
    //printf("i = %d,  j = %d,   color = %d,     actual color = %d \n\n",i,j, pattern[idx_pattern], mat[i][j]);
}*/



// sovrapposizione
//clang++ -std=c++11 -stdlib=libc++ -g dfs_algo.cpp -o dfs
//./dfs