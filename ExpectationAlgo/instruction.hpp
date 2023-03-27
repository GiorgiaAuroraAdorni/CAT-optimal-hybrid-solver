class Instruction {
public:
    std::vector<int> instruction;
    std::vector<int> pattern;
    int len;
};


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

