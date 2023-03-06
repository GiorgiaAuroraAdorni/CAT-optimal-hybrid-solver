#include <vector>
#include <random>


std::vector<std::vector<int>> MAX_INSTRUCTION = {{0,0,1},{0,1,0},{0,-1,1},{0,-1,-1},{2,0,1,-1,0,0,-1},{2,-1,0,-1,0,0,-1,0,-1},{2,-1,0,-1,0,0,1,0,1}, {2,0,-1,0,-1,-1,0,-1,0},{2,0,1,0,1,-1,0,-1,0},{0,-1,1,1,1},{0,1,1,-1,1},{1,1,1,1,-1},{1,1,-1,1,1}};

class Individual {
public:
    std::vector<std::vector<int>> instruction;
    std::vector<std::vector<int>> pattern;
    int fitness;
    
    Individual() : fitness(1000), instruction{}, pattern{} {}
};


void printIndividual(const Individual& ind) {
    std::cout << "Fitness: " << ind.fitness << "\n";
    std::cout << "Instructions:\n";
    for (const auto& instr : ind.instruction) {
        std::cout << "[";
        for (const auto& i : instr) {
            std::cout << i << ", ";
        }
        std::cout << "]\n";
    }
    std::cout << "Patterns:\n";
    for (const auto& patt : ind.pattern) {
        std::cout << "[";
        for (const auto& p : patt) {
            std::cout << p << ", ";
        }
        std::cout << "]\n";
    }
}


std::vector<Individual> selectINdividuals(std::vector<Individual> population, int num_parents) {
    
    std::vector<Individual> parents;

    //Sort population by fitness
    std::sort(population.begin(), population.end(), [](Individual& a, Individual& b) {
        return a.fitness > b.fitness;
    });

    //Selection of the fitness for roulett
    std::vector<int> fitnesses;
    int fitness_sum = 0.0;
    for (auto& ind : population) {
        fitness_sum += ind.fitness;
        fitnesses.push_back(fitness_sum);
    }

    //roulett
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, fitness_sum);
    while (parents.size() < num_parents) {
        double rand_fit = dis(gen);
        int i = 0;
        while (fitnesses[i] < rand_fit) {
            ++i;
        }
        parents.push_back(population[i]);
    }

    return parents;
}


Individual generateRandom(int number_gene, std::vector<std::vector<int>> Patterns){
    Individual result;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis_1(0, MAX_INSTRUCTION.size() - 1);
    std::uniform_int_distribution<> dis_2(0, Patterns.size() - 1);
    for(int i = 0; i < number_gene; ++i){
        int ins = dis_1(gen);
        int pat = dis_2(gen);
        result.instruction.push_back(MAX_INSTRUCTION[ins]);
        result.instruction.push_back(Patterns[pat]);
    }
    return result;
}

void killPopulation(int save_number, std::vector<Individual> & population){
    std::sort(population.begin(), population.end(), [](Individual& a, Individual& b) {
        return a.fitness > b.fitness;
    });
    while(population.size() > save_number){
        population.pop_back();
    }
}

std::vector<Individual> reproduction(const Individual& parent1, const Individual& parent2, std::vector<std::vector<int>> Patterns) {
    Individual child1;
    Individual child2;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis_1(0, parent1.instruction.size() - 1);
    int crossover_split = dis_1(gen);


    std::uniform_real_distribution<> dis_2(0, 1);
    std::uniform_int_distribution<> dis_3(0, MAX_INSTRUCTION.size() - 1);
    std::uniform_int_distribution<> dis_4(0, Patterns.size() - 1);

    for(int i = 0; i < crossover_split; i++){
        //CHILD 1
        double mutation_1 = dis_2(gen);
        if(mutation_1 > 0.005){
            child1.instruction.push_back(parent1.instruction[i]);
            child1.pattern.push_back(parent1.pattern[i]);
        }else{
            double new_inst = dis_3(gen);
            double new_patt = dis_3(gen);
            child1.instruction.push_back(MAX_INSTRUCTION[new_inst]);
            child1.pattern.push_back(Patterns[new_patt]);
        }
        printf("I = %d\n",i);
        //CHILD 2
        double mutation_2 = dis_2(gen);
        if(mutation_2 > 0.005){
            child2.instruction.push_back(parent2.instruction[i]);
            child2.pattern.push_back(parent2.pattern[i]);
        }else{
            double new_inst = dis_3(gen);
            double new_patt = dis_3(gen);
            child2.instruction.push_back(MAX_INSTRUCTION[new_inst]);
            child2.pattern.push_back(Patterns[new_patt]);
        }
    }
    printf("second part\n");

    for (int i = crossover_split; i < parent1.instruction.size(); i++) {
        //CHILD 1
        double mutation_1 = dis_2(gen);
        if(mutation_1 > 0.005){
            child1.instruction.push_back(parent2.instruction[i]);
            child1.pattern.push_back(parent2.pattern[i]);
        }else{
            double new_inst = dis_3(gen);
            double new_patt = dis_3(gen);
            child1.instruction.push_back(MAX_INSTRUCTION[new_inst]);
            child1.pattern.push_back(Patterns[new_patt]);
        }
        //CHILD 2
        double mutation_2 = dis_2(gen);
        if(mutation_2 > 0.005){
            child2.instruction.push_back(parent1.instruction[i]);
            child2.pattern.push_back(parent1.pattern[i]);
        }else{
            double new_inst = dis_3(gen);
            double new_patt = dis_3(gen);
            child2.instruction.push_back(MAX_INSTRUCTION[new_inst]);
            child2.pattern.push_back(Patterns[new_patt]);
        }
    }

   
    return { child1, child2 };
}
