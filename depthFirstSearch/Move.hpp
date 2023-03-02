
int executeInstruction(int id, int node_i, int node_j, std::vector<int> istruction, int lengthOfInst, std::vector<int> pattern, std::vector<std::vector<int>> * mat, std::vector<std::vector<int>> * actualRes,std::vector<int> & value_index){
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


int executeInstruction_number(int id, int node_i, int node_j, std::vector<int> istruction, int lengthOfInst, std::vector<int> pattern, std::vector<std::vector<int>> * mat, std::vector<std::vector<int>> * actualRes){
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
            id = id - 1;
            min_to_color -= 1;
        }
        //check if new node is correct and remove an uncorrect/non colored node
        if((*actualRes)[i][j] != (*mat)[i][j] && pattern[idx_pattern] == (*mat)[i][j]){
            id = id + 1;
            min_to_color += 1;

        }
        //update pattern index
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




std::vector<std::vector<int>> checkForCopy(int id, int node_i, int node_j, std::vector<int> istruction, int lengthOfInst, std::vector<int> pattern, std::vector<std::vector<int>> * mat, std::vector<std::vector<int>> * actualRes,std::vector<int> & value_index){
    //return {{node_i,node_j}};
    std::vector<std::vector<int>> result;
    std::vector<std::vector<std::vector<int>>> result_tmp;
    int max_len = mat->size() > 5 ? mat->size() : 5;
    for(int i = 0; i <= max_len; ++i){
        result_tmp.push_back({});
    }
    int count = 0;
    for(int i = 0; i < mat->size(); ++i){
        for(int j = 0; j < mat->size(); ++j){
            if((*actualRes)[i][j] == 0){
                if((*mat)[i][j] == pattern[0]){
                    int tmp_res = executeInstruction_number(0,i, j,istruction,lengthOfInst,pattern,mat,actualRes);
                    if(tmp_res > 0){
                        result_tmp[tmp_res-1].push_back({i,j});
                        count += 1;
                    }
                }
            }
        }
    }
    for(int i = result_tmp.size()-1; i >= 0; --i){
        for(int j = 0; j < result_tmp[i].size(); ++j){
            result.push_back(result_tmp[i][j]);
        }
    }
    return result;
}

int checkAllColor(std::vector<std::vector<int>> * mat, std::vector<std::vector<int>> * actualRes){
    int only_color = -1;
    for(int i = 0; i < mat->size(); ++i){
        for(int j = 0; j < mat->size(); ++j){
            if(only_color == -1 && (*mat)[i][j] != (*actualRes)[i][j]){
                only_color = (*mat)[i][j];
            }
            if((*actualRes)[i][j] == 0 && only_color != (*mat)[i][j]){
                return 0;
            }
        }
    }
    return only_color;
}
