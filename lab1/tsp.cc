#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <random>
#include <algorithm>

using Route = std::pair<int, std::vector<int>>;
using Parents = std::pair<int, int>;

class TSPGeneticSolver {
public:
    enum ParentOperator {
        Panmixia,
        Inbreeding,
        Outbreeding
    };

    enum CrossoverType {
        SinglePoint,
        TwoPoint,
        MultiPoint,
        CycleSinglePoint,
        CycleTwoPoint
    };

    static std::vector<int> solveTSP(ParentOperator prt_opr, CrossoverType crss_tp, const std::vector<int>& distance);

private:
    static void createPopulation(const std::vector<int>& distance);
    static int determineFitness(const std::vector<int>& distance, const std::vector<int>& route);
    static Parents selectParentPairOutbreeding();

    static bool checkMatrixSize(const std::vector<int>& distance);

    static std::vector<Route> population;
    static int num_cities;
};

int TSPGeneticSolver::num_cities = 0;
std::vector<Route> TSPGeneticSolver::population;

std::vector<int> TSPGeneticSolver::solveTSP(ParentOperator prt_opr, CrossoverType crss_tp, const std::vector<int>& distance) {

    if (!TSPGeneticSolver::checkMatrixSize(distance)) return {};

    num_cities = std::sqrt(static_cast<double>(distance.size()));

    createPopulation(distance);

    return {3, 4};
}

void TSPGeneticSolver::createPopulation(const std::vector<int>& distance) {
    std::vector<int> cities;

    for(int i = 0; i < num_cities; i++) {
        cities.push_back(i);
    }

    int num_routes = 10;
    
    std::random_device rd;
    std::mt19937 gen(rd());

    for (int i = 0; i < num_routes; ++i) {
        std::vector<int> random_route = cities;
        std::shuffle(random_route.begin(), random_route.end(), gen);
        
        std::cout << "fitness: " << determineFitness(distance, random_route) << std::endl;

        int fitness = determineFitness(distance, random_route);

        TSPGeneticSolver::population.push_back({fitness, random_route});

        std::cout << "size: " << population.size() << std::endl;
    }
}

int TSPGeneticSolver::determineFitness(const std::vector<int>& distance, const std::vector<int>& route) {
    int fitness = 0;
    
    for(size_t i = 0; i < route.size(); ++i) {
        int current_city = route[i];
        int next_city = route[(i + 1) % route.size()];
        
        fitness += distance[current_city * num_cities + next_city];
    }
    
    return fitness;
}

Parents selectParentPairOutbreeding() {
    return {0, 0};
}

bool TSPGeneticSolver::checkMatrixSize(const std::vector<int>& distance) {
    double root = std::sqrt(static_cast<double>(distance.size()));
    return root == static_cast<int>(root);
}

int main() {
    TSPGeneticSolver::solveTSP(TSPGeneticSolver::ParentOperator::Outbreeding, 
                               TSPGeneticSolver::CrossoverType::MultiPoint, 
                               {0, 10, 15, 20, 10, 0, 35, 25, 15, 35, 0, 30, 20, 25, 30, 0});
    return 0;
}
