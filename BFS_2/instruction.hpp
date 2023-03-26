class Instruction {
public:
    std::vector<int> instruction;
    std::vector<int> pattern;
    int len;
};


std::pair<Instruction, std::vector<int>> mirrorInst(bool mirror_x, bool mirror_y, Instruction inst, int i, int j, int n){
    std::vector<int> coordinate;
    if(mirror_y){
        coordinate.push_back(n-i-1);
    }else{
        coordinate.push_back(i);
    }

    if(mirror_x){
        coordinate.push_back(n-j-1);
    }else{
        coordinate.push_back(j);
    }

    auto instTmp = inst.instruction;
    int idx = 1;
    while(idx < instTmp.size()){
        if(mirror_y){
            instTmp[idx] = instTmp[idx] * -1;
        }
        idx += 1;

        if(mirror_x){
            instTmp[idx] = instTmp[idx] * -1;
        }
        idx += 1;
    }

    Instruction mirror_inst;
    mirror_inst.instruction = instTmp;
    mirror_inst.pattern = inst.pattern;
    mirror_inst.len = inst.len;

    return std::make_pair(mirror_inst, coordinate);
}
struct VectorHash {
    size_t operator()(const std::vector<int>& vec) const {
        std::hash<int> hasher;
        size_t seed = vec.size();
        for (auto& i : vec) {
            seed ^= hasher(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

std::unordered_map<std::vector<int>, std::string, VectorHash> GET_NAME_INSTRUCTION = {
    {{0,0,1}, "orizontal"},
    {{0,1,0}, "vertical"},
    {{0,-1,1},"diagonal-U-R"},
    {{0,-1,-1}, "diagonal-U-L"},
    {{2,0,1,-1,0,0,-1}, "square"},
    {{2,-1,0,-1,0,0,-1,0,-1}, "L-UP-L"},
    {{2,-1,0,-1,0,0,1,0,1}, "L-UP-R"},
    {{2,0,-1,0,-1,-1,0,-1,0}, "L-R-UP"},
    {{2,0,1,0,1,-1,0,-1,0}, "L-L-UP"},
    {{0,-1,1,1,1}, "ZigZag-Oriz-UP-DOW"},
    {{0,1,1,-1,1}, "ZigZag-Oriz-DOW-UP"},
    {{1,1,1,1,-1}, "ZigZag-Vert-R-L"},
    {{1,1,-1,1,1}, "ZigZag-Vert-L-R"}
};


std::string createInst(std::vector<std::vector<int>> nodes, Instruction instruction) {
    std::string result = "Node {";
    for (int i = 0; i < nodes.size(); i++) {
        result += "{";
        for (int j = 0; j < nodes[i].size(); j++) {
            result += std::to_string(nodes[i][j]);
            if (j < nodes[i].size() - 1) {
                result += ",";
            }
        }
        result += "}";
        if (i < nodes.size() - 1) {
            result += ",";
        }
    }
    std::string instruction_name = GET_NAME_INSTRUCTION[instruction.instruction];
    result += "} - Instruc " + instruction_name + " - Pattern {";
    for (int i = 0; i < instruction.pattern.size(); i++) {
        result += std::to_string(instruction.pattern[i]);
        if (i < instruction.pattern.size() - 1) {
            result += ",";
        }
    }
    result += "} - Length " + std::to_string(instruction.len);
    return result;
}


void printInstruction(const Instruction& ind) {
    std::cout << "Instructions:{";
    for (const auto& instr : ind.instruction) {
        std::cout << instr << ", ";
    }
    std::cout << "}";
    std::cout << " - ";
    std::cout << "Patterns:{";
    for (const auto& patt : ind.pattern) {
        std::cout << patt << ", ";
    }
    std::cout << "}";

    std::cout << " - ";
    std::cout << "Lengths:";
    std::cout << ind.len << "\n";
}

