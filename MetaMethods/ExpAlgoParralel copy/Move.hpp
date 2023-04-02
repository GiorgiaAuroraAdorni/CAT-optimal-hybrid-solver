
int WRONG_COLOR = 5;
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
        }else{
            (*actualRes)[i][j] = WRONG_COLOR;
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
    if(min_to_color < 0){
        id = -10000;
    }
    //printf("end MOve\n");

    return id;
}

int executeInstruction_special(int id, int node_i, int node_j, std::vector<int> istruction, int lengthOfInst, std::vector<int> pattern, std::vector<std::vector<int>> * mat, std::vector<std::vector<int>> * actualRes,std::vector<int> & value_index){
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
        }else{
            (*actualRes)[i][j] = WRONG_COLOR;
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

    //check if color at leart one pixel
    bool flag_check = false;
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
            flag_check = true;

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
    if(min_to_color < 0){
        id = -10000;
    }
    //printf("end MOve\n");
    if(flag_check == false){
        return -1000;
    }
    return id;
}




std::vector<std::vector<int>> checkForCopy(int id, int node_i, int node_j, std::vector<int> istruction, int lengthOfInst, std::vector<int> pattern, std::vector<std::vector<int>> * mat, std::vector<std::vector<int>> * actualRes,std::vector<int> & value_index){
    //return {{node_i,node_j}};
    std::vector<std::vector<int>> result;
    std::vector<std::vector<std::vector<int>>> result_tmp;
    int max_len = mat->size() > 5 ? mat->size() : 5;
    for(int i = 0; i <= (max_len); ++i){
        result_tmp.push_back({});
    }
    int count = 0;
    for(int i = 0; i < mat->size(); ++i){
        for(int j = 0; j < mat->size(); ++j){
            if((*actualRes)[i][j] == 0 || (*actualRes)[i][j] == 5){
                
                int tmp_res = executeInstruction_number(0,i, j,istruction,lengthOfInst,pattern,mat,actualRes);
                if(tmp_res > 0){
                    result_tmp[tmp_res].push_back({i,j});
                    count += 1;
                }
                
            }
        }
    }

    for(int i = 0; i < result_tmp.size(); ++i){
        auto tmp_result_to_order = result_tmp[i];
        for(int a = 0; a < tmp_result_to_order.size(); ++a){
            for(int b = a+1; b < tmp_result_to_order.size(); b++){
                auto Res_tmp = (*actualRes);
                int tmp_numb_1 = executeInstruction(0, tmp_result_to_order[a][0],  tmp_result_to_order[a][1],istruction, lengthOfInst,  pattern, mat, &Res_tmp,value_index);
                int tmp_numb_2 = executeInstruction_number(0, tmp_result_to_order[b][0],  tmp_result_to_order[b][1],istruction, lengthOfInst,  pattern, mat, &Res_tmp);

                if(tmp_numb_2 <= 0){
                    auto tmp = tmp_result_to_order[a];
                    tmp_result_to_order[a] = tmp_result_to_order[b];
                    tmp_result_to_order[b] = tmp;
                }
            }
        }
        for(int j = 0; j < tmp_result_to_order.size(); ++j){
            result.push_back(tmp_result_to_order[j]);
        }
    }


    return result;
}









int checkCancelMove(int id, int node_i, int node_j, std::vector<int> istruction, int lengthOfInst, std::vector<int> pattern, std::vector<std::vector<int>> * mat, std::vector<std::vector<int>> * actualRes){
    int n = mat->size();
    int idx_pattern = 0;
    int idx_istruct = 0;
    int i = node_i;
    int j = node_j;
    int pattern_end = 0;
    int type_of_inst = istruction[idx_istruct];
    istruction.erase(istruction.begin());
    if(type_of_inst == 2){
        lengthOfInst = istruction.size()/2+1;
    }
    while(true){
        if(pattern[idx_pattern] != (*mat)[i][j]){
            return -1;
        }
        idx_pattern += 1;
        if(idx_pattern >= pattern.size()){
            idx_pattern = 0;
        }

        i += istruction[(idx_istruct)];
        idx_istruct += 1;
        j += istruction[(idx_istruct)];
        idx_istruct += 1;
        if(idx_istruct >= istruction.size()){
            idx_istruct = 0;
        }

        lengthOfInst -= 1;
        if(lengthOfInst == 0){
            break;
        }

        if(i >= (*mat).size() || i < 0 || j >= (*mat).size() || j < 0 || (*mat)[i][j] == -1){
            return -1;
        }

    }

    return 1;
}

 


std::pair<std::vector<std::vector<int>>,std::vector<std::vector<int>>> checkForBESTmove(std::vector<int> istruction, int lengthOfInst, std::vector<int> pattern, std::vector<std::vector<int>> * mat, std::vector<std::vector<int>> * actualRes){
    std::vector<std::vector<int>> result;
    std::vector<std::vector<int>> fakehash(actualRes->size(), std::vector<int>((*actualRes)[0].size(), 1));

    int count = 0;
    for(int i = 0; i < mat->size(); ++i){
        for(int j = 0; j < mat->size(); ++j){
            if((*actualRes)[i][j] == 0 || (*actualRes)[i][j] == 5){
                int tmp_res = checkCancelMove(0,i, j,istruction,lengthOfInst,pattern,mat,actualRes);
                if(tmp_res > 0){
                    result.push_back({i,j});
                    fakehash[i][j] = -1;
                }
                
            }
        }
    }
    return std::make_pair(result,fakehash);
}



int checkAllColor(std::vector<std::vector<int>> * mat, std::vector<std::vector<int>> * actualRes){
    int only_color = -1;
    for(int i = 0; i < mat->size(); ++i){
        for(int j = 0; j < mat->size(); ++j){
            if(only_color == -1 && (*mat)[i][j] != (*actualRes)[i][j]){
                only_color = (*mat)[i][j];
            }
            if((*actualRes)[i][j] == WRONG_COLOR ||(((*actualRes)[i][j] == 0) && only_color != (*mat)[i][j])){
                return 0;
            }
        }
    }
    return only_color;
}








int checkColorOne(int id, int node_i, int node_j, std::vector<int> istruction, int lengthOfInst, std::vector<int> pattern, std::vector<std::vector<int>> * mat, std::vector<std::vector<int>> * actualRes){
    int n = mat->size();
    int idx_pattern = 0;
    int idx_istruct = 0;
    int i = node_i;
    int j = node_j;
    int pattern_end = 0;
    int min_to_color = 0;
    bool flag_check = false;
    //getTypeOfInst
    int type_of_inst = istruction[idx_istruct];
    istruction.erase(istruction.begin());

    if(type_of_inst == 2){
        lengthOfInst = istruction.size()/2+1;
    }

    //run the istruction
    while(true){
        if(pattern[idx_pattern] == (*mat)[i][j]){
            return 1;
        }
        idx_pattern += 1;
        if(idx_pattern >= pattern.size()){
            idx_pattern = 0;
        }

        i += istruction[(idx_istruct)];
        idx_istruct += 1;
        j += istruction[(idx_istruct)];
        idx_istruct += 1;
        if(idx_istruct >= istruction.size()){
            idx_istruct = 0;
        }

        lengthOfInst -= 1;
        if(lengthOfInst == 0){
            break;
        }

        if(i >= (*mat).size() || i < 0 || j >= (*mat).size() || j < 0 || (*mat)[i][j] == -1){
            //printf("end MOve\n");
            return -1;
        }
        
    }
    return -1;
}


int getPattern(std::vector<int> & pattern_res, int node_i, int node_j, std::vector<int> istruction, int lengthOfInst, std::vector<std::vector<int>> * mat){
    int n = mat->size();
    int idx_istruct = 0;
    int i = node_i;
    int j = node_j;
    int min_to_color = 0;
    bool flag_check = false;
    //getTypeOfInst
    int type_of_inst = istruction[idx_istruct];
    istruction.erase(istruction.begin());

    if(type_of_inst == 2){
        lengthOfInst = istruction.size()/2+1;
    }

    int pattern_idx = 0;

    //run the istruction
    while(true){
        if((*mat)[i][j] == -1){
            return -1;
        }
        if(pattern_res.size() < 4){
            pattern_res.push_back((*mat)[i][j]);
        }else{
            if(pattern_res[pattern_idx] != (*mat)[i][j]){
                return -1;
            }
            pattern_idx += 1;
            if(pattern_idx >= pattern_res.size()){
                pattern_idx = 0;
            }
        }
        

        i += istruction[(idx_istruct)];
        idx_istruct += 1;
        j += istruction[(idx_istruct)];
        idx_istruct += 1;
        if(idx_istruct >= istruction.size()){
            idx_istruct = 0;
        }

        lengthOfInst -= 1;
        if(lengthOfInst == 0){
            break;
        }

        if(i >= (*mat).size() || i < 0 || j >= (*mat).size() || j < 0 || (*mat)[i][j] == -1){
            //printf("end MOve\n");
            return -1;
        }
        
    }
    return 1;
}


struct VectorHash2 {
  size_t operator()(const std::vector<int>& v) const {
    std::hash<int> hasher;
    size_t seed = 0;
    for (int elem : v) {
      seed ^= hasher(elem) + 0x9e3779b9 + (seed<<6) + (seed>>2);
    }
    return seed;
  }
};


std::unordered_map<std::vector<int>, int, VectorHash2> LMOVE = {
    {{2,-1,0,-1,0,0,-1,0,-1}, 1},
    {{2,-1,0,-1,0,0,1,0,1}, 1},
    {{2,0,-1,0,-1,-1,0,-1,0}, 1},
    {{2,0,1,0,1,-1,0,-1,0}, 1},
    {{0,-1,1,1,1}, 0},
    {{0,1,1,-1,1}, 0},
    {{1,1,1,1,-1}, 0},
    {{1,1,-1,1,1}, 0},
    {{0,0,1}, 0},
    {{0,1,0}, 0},
    {{0,-1,1},0},
    {{0,-1,-1}, 0},
    {{2,0,1,-1,0,0,-1}, 1}
};




std::pair<std::vector<std::vector<std::vector<std::vector<int>>>>, std::vector<std::vector<int>>> getPossibleMove(std::vector<std::vector<int>> instructions,std::vector<std::vector<int>> & mat){
    std::vector<std::vector<std::vector<std::vector<int>>>> pattern_move = {};
    std::vector<std::vector<int>> len_move = {};

    for(int inst = 0; inst < instructions.size(); ++inst){

        pattern_move.push_back({});
        len_move.push_back({});

        int min_len = instructions[inst][0] == 2 ? 1 : 1;
        int max_len = instructions[inst][0] == 2 ? 1 : 6;
        int len_idx = 0;
        for(int len = min_len; len <= max_len; ++len){

            pattern_move[inst].push_back({});
            len_move[inst].push_back(len);
            
            std::unordered_map<std::vector<int>, int, VectorHash2> checkArr;

            for(int i = 0; i < mat.size(); ++i){
                for(int j = 0; j < mat[0].size(); ++j){

                    

                    std::vector<int> pattern_res;
                    int flag = getPattern(pattern_res,i, j, instructions[inst], len, &mat);

                    

                    if(flag == 1 && instructions[inst][0] == 2 && instructions.size() == 9){
                        if(pattern_res[0] != pattern_res[4]){
                            flag = -1;    
                        }
                    }

                    if(flag == 1 && checkArr[pattern_res] != 1){
                        pattern_move[inst][len_idx].push_back(pattern_res);
                        checkArr[pattern_res] = 1;
                    }
                }
            }
            len_idx += 1;
        }
        
    }

    return std::make_pair(pattern_move,len_move);   
}




std::vector<Instruction> getPossibleINST(std::vector<std::vector<int>> instructions,std::vector<std::vector<int>> & mat){
    std::vector<Instruction> res;

    for(int inst = 0; inst < instructions.size(); ++inst){
        Instruction tmpINST1;
        tmpINST1.instruction = instructions[inst];

        int min_len = instructions[inst][0] == 2 ? 1 : 1;
        int max_len = instructions[inst][0] == 2 ? 1 : 6;
        int len_idx = 0;
        for(int len = min_len; len <= max_len; ++len){
            
            Instruction tmpINST2 = tmpINST1;
            tmpINST2.len = len;

            if(inst != 0 &&  tmpINST1.instruction[0] != 2 && len == 1){
                continue;
            }
            
            std::unordered_map<std::vector<int>, int, VectorHash2> checkArr;

            for(int i = 0; i < mat.size(); ++i){
                for(int j = 0; j < mat[0].size(); ++j){
                    auto tmpINST3 = tmpINST2;
                    

                    std::vector<int> pattern_res;
                    int flag = getPattern(pattern_res,i, j, instructions[inst], len, &mat);

                    

                    if(flag == 1 && checkArr[pattern_res] != 1){
                        tmpINST3.pattern = pattern_res;
                        res.push_back(tmpINST3);
                        //printInstruction(tmpINST3);
                        checkArr[pattern_res] = 1;
                    }
                }
            }
            len_idx += 1;
        }
        
    }

    return res;   
}


//TypeOFmirrir  --> 1 verticale e siamo a sinistra
//              --> 2 verticale e siamo a destra
//              --> 3 vorizzontale e siamo a sotto
//              --> 4 vorizzontale e siamo a sopra

std::pair<int, std::vector<std::vector<int>>> getMirrorVeritcalLeft(int old_id,std::vector<std::vector<int>> & resMat,std::vector<std::vector<int>> & currMat,std::vector<int> & value_index){
    std::vector<std::vector<int>> res;
    int new_id = old_id;
    int n = resMat.size();
    int m = n - 1;
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n/2; ++j){
            if(currMat[i][j] == -1 || resMat[i][m-j] == -1){
                continue;
            }
            if(resMat[i][m-j] == currMat[i][j]){
                if(currMat[i][m-j] == 0 || currMat[i][m-j] == WRONG_COLOR){
                    new_id += std::pow(2,value_index[i*n+(m-j)]);
                }
                currMat[i][m-j] = currMat[i][j];
                res.push_back({i,j});
            }
        }
    }
    return std::make_pair(new_id,res);
}


std::pair<int, std::vector<std::vector<int>>> getMirrorVeritcalRig(int old_id,std::vector<std::vector<int>> & resMat,std::vector<std::vector<int>> & currMat,std::vector<int> & value_index){
    std::vector<std::vector<int>> res;
    int new_id = old_id;
    int n = resMat.size();
    int m = n - 1;
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n/2; ++j){
            if(resMat[i][j] == -1 || currMat[i][m-j] == -1){
                continue;
            }
            if(resMat[i][j] == currMat[i][m-j]){
                if(currMat[i][j] == 0 || currMat[i][j] == WRONG_COLOR){
                    new_id += std::pow(2,value_index[i*n+j]);
                }
                currMat[i][j] = currMat[i][m-j];
                res.push_back({i,(m-j)});
            }
        }
    }
    return std::make_pair(new_id,res);
}


std::pair<int, std::vector<std::vector<int>>> getMirrorHorizontalTop(int old_id,std::vector<std::vector<int>> & resMat,std::vector<std::vector<int>> & currMat,std::vector<int> & value_index){
    std::vector<std::vector<int>> res;
    int new_id = old_id;
    int n = resMat.size();
    int m = n - 1;
    for(int i = 0; i < n/2; ++i){
        for(int j = 0; j < n; ++j){
            if(currMat[i][j] == -1 || resMat[m-i][j] == -1){
                continue;
            }
            if(resMat[m-i][j] == currMat[i][j]){
                if(currMat[m-i][j] == 0 || currMat[m-i][j] == WRONG_COLOR){
                    new_id += std::pow(2,value_index[(m-i)*n+j]);
                }
                currMat[m-i][j] = currMat[i][j];
                res.push_back({i,j});
            }
        }
    }
    return std::make_pair(new_id,res);
}



std::pair<int, std::vector<std::vector<int>>> getMirrorHorizontalDown(int old_id,std::vector<std::vector<int>> & resMat,std::vector<std::vector<int>> & currMat,std::vector<int> & value_index){
    std::vector<std::vector<int>> res;
    int new_id = old_id;
    int n = resMat.size();
    int m = n - 1;
    for(int i = 0; i < n/2; ++i){
        for(int j = 0; j < n; ++j){
            if(resMat[i][j] == -1 || currMat[m-i][j] == -1){
                continue;
            }
            if(currMat[m-i][j] == resMat[i][j]){
                if(currMat[i][j] == 0 || currMat[i][j] == WRONG_COLOR){
                    new_id += std::pow(2,value_index[i*n+j]);
                }
                currMat[i][j] = currMat[m-i][j];
                res.push_back({(m-i),j});
            }
        }
    }
    return std::make_pair(new_id,res);
}