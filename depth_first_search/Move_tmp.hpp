
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
}


int moveAndColor(int node_i, int node_j, std::vector<int> istruction, int lengthOfInst, std::vector<int> pattern, std::vector<std::vector<int>> * mat, std::vector<std::vector<int>> * actualRes){
    
    //initialization of parameter
    //index to read vector of instruction and pattern
    int idx_pattern = 0;
    int idx_istruct = 0;

    // i and j coordinate to move into the graph
    int i = node_i;
    int j = node_j;

    //check patter end, but not used --> we assume that if a move has length 2 you cant have a pattern of 3 or 4
    int pattern_end = 0;

    //minimal number to color
    int min_to_color = 0;

    //getTypeOfInst
    int type_of_inst = istruction[idx_istruct];
    istruction.erase(istruction.begin());

    if(type_of_inst == 2){
        lengthOfInst = istruction.size()/2+1;
    }

    //run the istruction
    while(true){
        //printf("i = %d,  j = %d , idx_inst = %d, idx_patt = %d, len_inst = %d\n",i,j, idx_istruct, idx_pattern, lengthOfInst);

        //check if a non correct node overlapp on a correct node  
        if((*actualRes)[i][j] == (*mat)[i][j] && pattern[idx_pattern] != (*mat)[i][j]){
            min_to_color -= 1;
        }
        //check if new node is correct and remove an uncorrect/non colored node
        if((*actualRes)[i][j] != (*mat)[i][j] && pattern[idx_pattern] == (*mat)[i][j]){
            min_to_color += 1;

        }

        //update pattern index
        (*actualRes)[i][j] = pattern[idx_pattern];
        idx_pattern += 1;
        if(idx_pattern >= pattern.size()){
            idx_pattern = 0;
        }

        //update coordinate with instruction and update coordinate index
        i += istruction[(idx_istruct)];
        idx_istruct += 1;
        j += istruction[(idx_istruct)];
        idx_istruct += 1;
        if(idx_istruct >= istruction.size()){
            idx_istruct = 0;
        }

        //if the instruction is endend we break
        lengthOfInst -= 1;
        if(lengthOfInst == 0){
            break;
        }

        //check if you can do the move
        if(i >= (*mat).size() || i < 0 || j >= (*mat).size() || j < 0 ){
            return -10000;
        }

    }
    return min_to_color;
}



int moveFinal(int id, int node_i, int node_j, std::vector<int> istruction, int lengthOfInst, std::vector<int> pattern, std::vector<std::vector<int>> * mat, std::vector<std::vector<int>> * actualRes){
    
    //initialization of parameter
    //index to read vector of instruction and pattern
    int idx_pattern = 0;
    int idx_istruct = 0;

    // i and j coordinate to move into the graph
    int i = node_i;
    int j = node_j;

    //check patter end, but not used --> we assume that if a move has length 2 you cant have a pattern of 3 or 4
    int pattern_end = 0;

    //minimal number to color
    int min_to_color = 0;

    //getTypeOfInst
    int type_of_inst = istruction[idx_istruct];
    istruction.erase(istruction.begin());

    if(type_of_inst == 2){
        lengthOfInst = istruction.size()/2+1;
    }

    //run the istruction
    while(true){
        //printf("i = %d,  j = %d , idx_inst = %d, idx_patt = %d, len_inst = %d\n",i,j, idx_istruct, idx_pattern, lengthOfInst);

        //check if a non correct node overlapp on a correct node  
        if((*actualRes)[i][j] == (*mat)[i][j] && pattern[idx_pattern] != (*mat)[i][j]){
            id = id - 2^(mat->size() * i + j);
            min_to_color -= 1;
        }
        //check if new node is correct and remove an uncorrect/non colored node
        if((*actualRes)[i][j] != (*mat)[i][j] && pattern[idx_pattern] == (*mat)[i][j]){
            id = id - 2^(mat->size() * i + j);
            min_to_color += 1;

        }

        //update pattern index
        (*actualRes)[i][j] = pattern[idx_pattern];
        idx_pattern += 1;
        if(idx_pattern >= pattern.size()){
            idx_pattern = 0;
        }

        //update coordinate with instruction and update coordinate index
        i += istruction[(idx_istruct)];
        idx_istruct += 1;
        j += istruction[(idx_istruct)];
        idx_istruct += 1;
        if(idx_istruct >= istruction.size()){
            idx_istruct = 0;
        }

        //if the instruction is endend we break
        lengthOfInst -= 1;
        if(lengthOfInst == 0){
            break;
        }

        //check if you can do the move
        if((*mat)[i][j] == -1 || i >= (*mat).size() || i < 0 || j >= (*mat).size() || j < 0 ){
            return -10000;
        }

    }
    if(min_to_color <= 0){
        id = -10000;
    }
    return min_to_color;
}





//clang++ -std=c++11 -stdlib=libc++ -g dfs_algo.cpp -o dfs
//./dfs




int moveFinal_2(int id, int node_i, int node_j, std::vector<int> istruction, int lengthOfInst, std::vector<int> pattern, std::vector<std::vector<int>> * mat, std::vector<std::vector<int>> * actualRes,std::vector<int> & value_index){
    //printf("start MOve\n");
    int n = mat->size();
    //initialization of parameter
    //index to read vector of instruction and pattern
    int idx_pattern = 0;
    int idx_istruct = 0;

    // i and j coordinate to move into the graph
    int i = node_i;
    int j = node_j;

    //check patter end, but not used --> we assume that if a move has length 2 you cant have a pattern of 3 or 4
    int pattern_end = 0;

    //minimal number to color
    int min_to_color = 0;

    //getTypeOfInst
    int type_of_inst = istruction[idx_istruct];
    istruction.erase(istruction.begin());

    if(type_of_inst == 2){
        lengthOfInst = istruction.size()/2+1;
    }

    //run the istruction
    while(true){
        //printf("i = %d,  j = %d , idx_inst = %d, idx_patt = %d, len_inst = %d\n",i,j, idx_istruct, idx_pattern, lengthOfInst);
        //check if a non correct node overlapp on a correct node  
        if((*actualRes)[i][j] == (*mat)[i][j] && pattern[idx_pattern] != (*mat)[i][j]){
            id = id - std::pow(2,value_index[i*n+j]);
            min_to_color -= 1;
        }
        //check if new node is correct and remove an uncorrect/non colored node
        if((*actualRes)[i][j] != (*mat)[i][j] && pattern[idx_pattern] == (*mat)[i][j]){
            id = id + std::pow(2,value_index[i*n+j]);
            min_to_color += 1;

        }
        //update pattern index
        if((*mat)[i][j] == pattern[idx_pattern]){
            (*actualRes)[i][j] = pattern[idx_pattern];
        }
        idx_pattern += 1;
        if(idx_pattern >= pattern.size()){
            idx_pattern = 0;
        }

        //update coordinate with instruction and update coordinate index
        i += istruction[(idx_istruct)];
        idx_istruct += 1;
        j += istruction[(idx_istruct)];
        idx_istruct += 1;
        if(idx_istruct >= istruction.size()){
            idx_istruct = 0;
        }

        //if the instruction is endend we break
        lengthOfInst -= 1;
        if(lengthOfInst == 0){
            break;
        }

        //check if you can do the move
        if(i >= (*mat).size() || i < 0 || j >= (*mat).size() || j < 0 || (*mat)[i][j] == -1){
            //printf("end MOve\n");
            return -10000;
        }

    }
    if(min_to_color <= 0){
        id = -10000;
    }
    //printf("end MOve\n");

    return id;
}





//clang++ -std=c++11 -stdlib=libc++ -g dfs_algo.cpp -o dfs
//./dfs