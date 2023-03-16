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






std::unordered_map<std::string, std::vector<int>> GET_INSTRUCTION = {
        {"orizontal", {0,0,1}},
        {"vertical", {0,1,0}},
        {"diagonal-U-R", {0,-1,1}},
        {"diagonal-U-L", {0,-1,-1}},
        {"square", {2,0,1,-1,0,0,-1}},
        {"L-UP-L", {2,-1,0,-1,0,0,-1,0,-1}},
        {"L-UP-R", {2,-1,0,-1,0,0,1,0,1}},
        {"L-R-UP", {2,0,-1,0,-1,-1,0,-1,0}},
        {"L-L-UP", {2,0,1,0,1,-1,0,-1,0}},
        {"ZigZag-Oriz-UP-DOW", {0,-1,1,1,1}},
        {"ZigZag-Oriz-DOW-UP", {0,1,1,-1,1}},
        {"ZigZag-Vert-R-L", {1,1,1,1,-1}},
        {"ZigZag-Vert-L-R", {1,1,-1,1,1}}
};
