#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <regex>
std::string removeFirst12Chars(const std::string& str) {
    if (str.length() <= 12) {
        return "";
    }
    return str.substr(12);
}

std::string buildInstruction(std::vector<std::vector<int>> index_node, int len, std::vector<int> instruction, std::vector<int> pattern){
    std::string result = "Nodes(";
    for(int i = 0; i < index_node.size(); i++){
        auto tmp_1 = std::to_string(index_node[i][0]);
        auto tmp_2 = std::to_string(index_node[i][1]);
        result = result+ "{" +tmp_1 + "," + tmp_2 + "}";
    }
    result = result + ")";

    result = result + "Instruction{"+GET_NAME_INSTRUCTION[instruction]+ "} len = {"+ std::to_string(len) + "}  Pattern = {";
    for(int i = 0; i < pattern.size(); ++i){
        if(i != 0){
            result = result + ",";
        }
        result = result + std::to_string(pattern[i]);
    }
    result = result + "}";
    return result;
}

std::vector<std::string> parseString(const std::string& inputStr) {
    std::vector<std::string> result;
    std::stringstream ss(inputStr);
    std::string item;
    while (std::getline(ss, item, '\n')) {
        result.push_back(item);
    }
    return result;
}

//FATTO DA CHATGPT3

void getProg(std::string input, std::string * instruction_in, int * len_in, std::vector<int> & patterns_in, std::vector<std::vector<int>> & nodes_in) {
    // Cerca tutti i numeri tra parentesi graffe e aggiungili alla lista di nodi
    std::vector<std::vector<int>> nodes;
    std::regex rgx("\\{(\\d+),(\\d+)\\}");
    std::sregex_iterator iter(input.begin(), input.end(), rgx);
    std::sregex_iterator end;
    for (; iter != end; ++iter) {
        int x = std::stoi((*iter)[1].str());
        int y = std::stoi((*iter)[2].str());
        nodes.push_back({x, y});
    }
    //nodes.pop_back();
    // Estrai l'istruzione dalla stringa
    std::regex rgx2("Instruction\\{([^\\}]*)\\}");
    std::smatch match;
    if (std::regex_search(input, match, rgx2)) {
        *instruction_in = match[1].str();
    }

    // Estrai gli altri valori dalla stringa
    int len = 0;
    std::vector<int> pattern;

    std::regex rgx3("len\\s*=\\s*\\{(\\d+)\\}");
    if (std::regex_search(input, match, rgx3)) {
        len = std::stoi(match[1].str());
    }

    std::regex rgx4("Pattern\\s*=\\s*\\{([^\\}]*)\\}");
    if (std::regex_search(input, match, rgx4)) {
        std::string pattern_str = match[1].str();
        std::stringstream ss(pattern_str);
        int num;
        while (ss >> num) {
            pattern.push_back(num);
            if (ss.peek() == ',') {
                ss.ignore();
            }
        }
    }

    nodes_in = nodes;
    *len_in = len;
    patterns_in = pattern;
}
