#include <vector>
#include <iostream>


void print2DVector(const std::vector<std::vector<int>>& v) {
    for (const auto& row : v) {
        for (const auto& element : row) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
    }
}

std::vector<std::vector<int>> generatePatterns(int n_patt){
    std::vector<std::vector<int>> result;


    

    
    //push base case: {1}, {2}, ...
    for(int i = 1; i <= n_patt; ++i){
        result.push_back({i});

        //push second case: {1,2}, {1,3}, ...
        for(int j = 1; j <=n_patt; ++j){
            if(i != j){
                result.push_back({i,j});
            }

            //push third case: {1,2,2}, {1,2,3}, ...
            for(int k = 1; k <= n_patt; ++k){
                // no case 111 == 1  121 == 12
                if((i != j && i != k) && (i != k)){
                    result.push_back({i,j,k});
                }

                //
                for(int l = 1; l <= n_patt; ++l){
                    // no case 1231 == 123  1212 == 12
                    if((i != l) && ((i != j) ^ (i != k || j != l))){
                         result.push_back({i,j,k,l});
                    }
                }
            }
        }
    }
    
    //print2DVector(result);
    //printf("%lu\n", result.size());
    return result;
}