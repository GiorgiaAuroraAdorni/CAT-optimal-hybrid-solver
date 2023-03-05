#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <regex>


std::vector<std::string> parseString(const std::string& inputStr) {
    std::vector<std::string> result;
    std::stringstream ss(inputStr);
    std::string item;
    while (std::getline(ss, item, '\n')) {
        result.push_back(item);
    }
    return result;
}


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
