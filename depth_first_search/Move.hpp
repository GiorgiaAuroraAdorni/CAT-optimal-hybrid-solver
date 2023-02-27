#include <string>
#include <vector>

/**
 * @brief               Work one istruction
 * 
 * @param n             Size of matrix
 * @param m             Size of matrix
 * @param id            Actual Id of actualRes
 * @param node_i        Coordinate i of start node
 * @param node_j        Coordinate j of start node
 * @param istruction    Array that rapresent the instruction
 * @param lengthOfInst  Length of the instruction/ colered pixels
 * @param pattern       The pattern for the instruction
 * @param mat           The final array result
 * @param actualRes     The actual colored result
 * @param value_index   Map to have in O(1) for i and j the corresponding encripted value
 * @return int          the new Id
 */
int oneStep(int n, int m, int id, int node_i, int node_j, std::vector<int> istruction, int lengthOfInst, std::vector<int> pattern, std::vector<std::vector<int>> * mat, std::vector<std::vector<int>> * actualRes,std::vector<int> & value_index){
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

    // if the instruction have a fixed length we adapt
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