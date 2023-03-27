
std::vector<std::string> parse_string(const std::string& str) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, ' ')) {
        tokens.push_back(token);
    }

    return tokens;
}

std::vector<std::vector<int>> file_reader(std::string fileName){
    // line_file    --> the line for all file
    // x AND y      --> two vector that rapresent city x[0] and y[0] 
    //                  --> are coordinate of city 1 
    // flag         --> if true that means the line_file rapresent point not descriptions
    // best         --> solution

    std::string line_file;
    std::vector<float> x;
    std::vector<float> y;
    double best = 0.0;
    bool flag = false;
    // START read File
    std::ifstream MyReadFile(fileName);
    std::vector<std::vector<int> > Problem;
    int n = 0;
    int r = 0;
    while (getline(MyReadFile, line_file))
    {
        size_t firstNonSpace = line_file.find_first_not_of(' ');
        //Stop read because file is end
        if (line_file == "EOF")
        {
            break;
        }
        std::string inputString = line_file;
        std::vector<std::string> parsedString = parse_string(inputString);


        if (parsedString.size() == 1){
            n = std::stoi(parsedString[0]);
            std::vector<std::vector<int> > mat(n, std::vector<int>(n, 0));
            Problem = mat;
            //printf("The problem has length %lu x %lu \n", Problem.size(), Problem.size());

        }else{
            for(int i = 0; i < n; i++){
                int value_tmp = std::stoi(parsedString[i]);
                if(value_tmp == 0){
                    Problem[r][i] = -1;
                }else{
                    Problem[r][i] = value_tmp;
                }
            }
            r += 1;
        }
    }

    MyReadFile.close();
    return Problem;
}


std::vector<std::vector<int>> file_reader_test(std::string fileName){
    // line_file    --> the line for all file
    // x AND y      --> two vector that rapresent city x[0] and y[0] 
    //                  --> are coordinate of city 1 
    // flag         --> if true that means the line_file rapresent point not descriptions
    // best         --> solution

    std::string line_file;
    std::vector<float> x;
    std::vector<float> y;
    double best = 0.0;
    bool flag = false;
    // START read File
    std::ifstream MyReadFile(fileName);
    std::vector<std::vector<int> > Problem;
    int n = 0;
    int r = 0;
    while (getline(MyReadFile, line_file))
    {
        size_t firstNonSpace = line_file.find_first_not_of(' ');
        //Stop read because file is end
        if (line_file == "EOF")
        {
            break;
        }
        std::string inputString = line_file;
        std::vector<std::string> parsedString = parse_string(inputString);


        if (parsedString.size() == 1){
            n = std::stoi(parsedString[0]);
            std::vector<std::vector<int> > mat(n, std::vector<int>(n, 0));
            Problem = mat;
            //printf("The problem has length %lu x %lu \n", Problem.size(), Problem.size());

        }else{
            for(int i = 0; i < n; i++){
                int value_tmp = std::stoi(parsedString[i]);
                Problem[r][i] = value_tmp;

            }
            r += 1;
        }
    }

    MyReadFile.close();
    return Problem;
}