#include <regex>
#include <iostream>
#include <vector>

int main() {
    std::string input = "Nodes({0,0}{0,2}{1,3}{5,4}){orizontal} len = {3}  Pattern = {3,4}";

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

    // Estrai gli altri valori dalla stringa
    std::string instruct = "orizontal";
    int len = 0;
    std::vector<int> pattern;

    std::regex rgx2("len\\s*=\\s*\\{(\\d+)\\}");
    std::smatch match;
    if (std::regex_search(input, match, rgx2)) {
        len = std::stoi(match[1].str());
    }

    std::regex rgx3("Pattern\\s*=\\s*\\{([^\\}]*)\\}");
    if (std::regex_search(input, match, rgx3)) {
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

    // Stampa i valori estratti
    std::cout << "Nodes: ";
    for (auto node : nodes) {
        std::cout << "{" << node[0] << "," << node[1] << "} ";
    }
    std::cout << std::endl;

    std::cout << "Instruction: " << instruct << std::endl;
    std::cout << "Len: " << len << std::endl;

    std::cout << "Pattern: ";
    for (auto num : pattern) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}