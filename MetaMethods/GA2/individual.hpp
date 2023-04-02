#include <vector>
#include <random>



class Individual {
public:
    std::vector<Instruction> instruction;
    int fitness;
};

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


Instruction getSingleGene(std::vector<Instruction> Instructions){    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis_1(0, Instructions.size() - 1);
    int random_gene = dis_1(gen);
    return Instructions[random_gene];
}

Individual generateRandom(int number_gene, std::vector<Instruction> & Instructions){
    Individual result;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis_1(0, Instructions.size() - 1);
    for(int i = 0; i < number_gene; ++i){

        auto gene = getSingleGene(Instructions);
        result.instruction.push_back(gene);
        
    }
    return result;
}

void killPopulation(int save_number, std::vector<Individual> & population){
    std::sort(population.begin(), population.end(), [](Individual& a, Individual& b) {
        return a.fitness < b.fitness;
    });
    while(population.size() > save_number){
        population.pop_back();
    }
}

void updateChild(Individual & parent, Individual & child, int idx, std::mt19937 gen,  std::vector<Instruction> & Instructions){
    
    std::uniform_real_distribution<> mutation_dis(0, 1);
    std::uniform_int_distribution<> dis_3(0, Instructions.size() - 1);
    double mutation = mutation_dis(gen);

    if(mutation > 0.005){
        child.instruction[idx] = (parent.instruction[idx]);
    }else{
        auto gene = getSingleGene(Instructions);
        child.instruction[idx] = gene;
    }
}

std::vector<Individual> reproduction(Individual parent1, Individual parent2, std::vector<Instruction> & Instructions) {
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
    int i = 0;
    while(i < parent1.instruction.size()){
        //printf("i = %d, cr %d, len = %lu,%lu,%lu,%lu    chillen = %lu,%lu,%lu,%lu\n", i, crossover_split,parent1.instruction.size(),parent1.pattern.size(),parent2.instruction.size(),parent2.pattern.size(),child1.instruction.size(),child1.pattern.size(),child2.instruction.size(),child2.pattern.size());
        if(i < crossover_split){
            updateChild(parent1, child1, i,gen,Instructions);
            updateChild(parent2, child2, i,gen,Instructions);
        }else{
            updateChild(parent2, child1, i,gen,Instructions);
            updateChild(parent1, child2, i,gen,Instructions);

        }
        ++i;
    }
    return { child1, child2 };
}
