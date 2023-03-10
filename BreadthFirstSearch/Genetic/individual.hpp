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


void KillUselessPeople(std::vector<Individual> & population){
    for (auto i = population.begin(); i != population.end();) {
        if(i->fitness == 10000) {
            i = population.erase(i);
        } else {
            ++i;
        }
    }
}

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
        return a.fitness < b.fitness;
    });

    //Selection of the inverse of fitness for roulett
    std::vector<double> inv_fitnesses;
    double inv_fitness_sum = 0.0;
    for (auto& ind : population) {
        inv_fitness_sum += 1.0 / ind.fitness;
        inv_fitnesses.push_back(inv_fitness_sum);
    }

    //roulett
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, inv_fitness_sum);
    while (parents.size() < num_parents) {
        double rand_fit = dis(gen);
        int i = 0;
        while (inv_fitnesses[i] < rand_fit) {
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
        result.pattern.push_back(Patterns[pat]);
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

void updateChild(Individual & parent, Individual & child, int idx, std::mt19937 gen, std::vector<std::vector<int>> Patterns){
    
    std::uniform_real_distribution<> mutation_dis(0, 1);
    std::uniform_int_distribution<> dis_3(0, MAX_INSTRUCTION.size() - 1);
    std::uniform_int_distribution<> dis_4(0, Patterns.size() - 1);
    double mutation = mutation_dis(gen);

    if(mutation > 0.005){
        child.instruction[idx] = (parent.instruction[idx]);
        child.pattern[idx] = parent.pattern[idx];
    }else{
        double new_inst = dis_3(gen);
        double new_patt = dis_4(gen);
        child.instruction[idx] = MAX_INSTRUCTION[new_inst];
        child.pattern[idx] = Patterns[new_patt];
    }
}

std::vector<Individual> reproduction(Individual parent1, Individual parent2, std::vector<std::vector<int>> Patterns) {
    auto child1 = parent1;
    auto child2 = parent2;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis_1(0, parent1.instruction.size() - 1);
    int crossover_split = dis_1(gen);

    //case we have a clone of parent
    if(crossover_split == 0){
        return {child1, child2};
    }

    std::uniform_real_distribution<> mutation_dis(0, 1);
    std::uniform_int_distribution<> dis_3(0, MAX_INSTRUCTION.size() - 1);
    std::uniform_int_distribution<> dis_4(0, Patterns.size() - 1);
    int i = 0;
    while(i < parent1.instruction.size()){
        //printf("i = %d, cr %d, len = %lu,%lu,%lu,%lu    chillen = %lu,%lu,%lu,%lu\n", i, crossover_split,parent1.instruction.size(),parent1.pattern.size(),parent2.instruction.size(),parent2.pattern.size(),child1.instruction.size(),child1.pattern.size(),child2.instruction.size(),child2.pattern.size());
        if(i < crossover_split){
            updateChild(parent1, child1, i,gen,Patterns);
            updateChild(parent2, child2, i,gen,Patterns);
        }else{
            updateChild(parent2, child1, i,gen,Patterns);
            updateChild(parent1, child2, i,gen,Patterns);
        }
        ++i;
    }
    return { child1, child2 };
}
