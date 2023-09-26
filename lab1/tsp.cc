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

    static std::vector<int> solveTSP(ParentOperator prt_opr, CrossoverType crss_tp, const std::vector<int>& distance, 
                                    const double mutationRate, const int generationCount);

private:
    static void createPopulation(const std::vector<int>& distance);
    static int determineFitness(const std::vector<int>& distance, const std::vector<int>& route);
    static Parents selectParentPairInBreeding();
    static std::vector<int> twoPointCrossover(const Parents& parent_pair);
    static bool shouldMutate(const double mutationRate);
    static void mutate(std::vector<int>& route);
    static bool hasDuplicates(const std::vector<int>& route);
    static void fixDuplicatesAndAddMissing(std::vector<int>& route);

    static bool checkMatrixSize(const std::vector<int>& distance);

    static std::vector<Route> population;
    static int num_cities;
};

int TSPGeneticSolver::num_cities = 0;
std::vector<Route> TSPGeneticSolver::population;

std::vector<int> TSPGeneticSolver::solveTSP(ParentOperator prt_opr, CrossoverType crss_tp, const std::vector<int>& distance, 
                                            const double mutationRate, const int generationCount) {

    if (!TSPGeneticSolver::checkMatrixSize(distance)) return {};

    num_cities = std::sqrt(static_cast<double>(distance.size()));

    createPopulation(distance);

    for (int i = 0; i < generationCount; ++i) {
        auto parent_pair = selectParentPairInBreeding();
        auto child_route = twoPointCrossover(parent_pair);

        if(hasDuplicates(child_route)) {
            fixDuplicatesAndAddMissing(child_route);
        }

        if (shouldMutate(mutationRate)) {
            mutate(child_route);
        }

        int fitness = determineFitness(distance, child_route);
        std::cout << "fitness: " << fitness << std::endl;
        population.push_back({fitness, child_route});
    }


    std::sort(population.begin(), population.end());

    std::cout << "answer: " << population[0].first << std::endl;

    for(int i = 0; i < population[0].second.size(); i++) {
        std::cout << population[0].second[i] << " ";
    }

    return population[0].second;
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
        int fitness = determineFitness(distance, random_route);
        population.push_back({fitness, random_route});
    }
}

int TSPGeneticSolver::determineFitness(const std::vector<int>& distance, const std::vector<int>& route) {
    int fitness = 0;

    if (route.empty() || route.size() <= 1) {
        return fitness;
    }

    for (size_t i = 0; i < route.size() - 1; ++i) {
        int current_city = route[i];
        int next_city = route[i + 1];

        int distance_value = distance[current_city * num_cities + next_city];

        fitness += distance_value;
    }

    int last_city = route.back();
    int first_city = route.front();
    int distance_to_start = distance[last_city * num_cities + first_city];
    fitness += distance_to_start;

    return fitness;
}

Parents TSPGeneticSolver::selectParentPairInBreeding() {
    std::sort(population.begin(), population.end());
    
    int min_diff = population[1].first - population[0].first;
    Parents parent_pair{0, 1};

    for(int i = 1; i < population.size() - 1; i++){
        int diff = population[i + 1].first - population[i].first;
        if(diff < min_diff) {
            min_diff = diff;
            parent_pair = {i, i + 1};
        }
    }

    return parent_pair;
}

std::vector<int> TSPGeneticSolver::twoPointCrossover(const Parents& parent_pair) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(1, num_cities - 2);
    
    int point1 = distribution(gen);
    int point2;
    do {
        point2 = distribution(gen);
    } while (point1 == point2);

    if (point1 > point2) {
        std::swap(point1, point2);
    }

    std::vector<int> child_route;
    child_route.resize(num_cities);

    for (int i = point1; i <= point2; ++i) {
        child_route[i] = population[parent_pair.first].second[i];
    }

    for (int i = 0; i < point1; ++i) {
        child_route[i] = population[parent_pair.second].second[i];
    }

    for (int i = point2 + 1; i < num_cities; ++i) {
        child_route[i] = population[parent_pair.second].second[i];
    }

    return child_route;
}

bool TSPGeneticSolver::shouldMutate(const double mutationRate) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    double randomValue = distribution(gen);

    return randomValue < mutationRate;
}

void TSPGeneticSolver::mutate(std::vector<int>& route) {
    int size = route.size();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, size - 1);

    int mutationPoint1 = distribution(gen);
    int mutationPoint2;
    do {
        mutationPoint2 = distribution(gen);
    } while (mutationPoint1 == mutationPoint2);

    std::iter_swap(route.begin() + mutationPoint1, route.begin() + mutationPoint2);
}

bool TSPGeneticSolver::hasDuplicates(const std::vector<int>& route) {
    std::vector<int> routePath = route;
    std::sort(routePath.begin(), routePath.end());
    return std::adjacent_find(routePath.begin(), routePath.end()) != routePath.end();
}

void TSPGeneticSolver::fixDuplicatesAndAddMissing(std::vector<int>& route) {
    std::vector<int> uniqueCities;
    for (int city : route) {
        if (std::find(uniqueCities.begin(), uniqueCities.end(), city) == uniqueCities.end()) {
            uniqueCities.push_back(city);
        }
    }

    for (int city = 0; city < num_cities; ++city) {
        if (std::find(uniqueCities.begin(), uniqueCities.end(), city) == uniqueCities.end()) {
            uniqueCities.push_back(city);
        }
    }

    route = uniqueCities;
}

bool TSPGeneticSolver::checkMatrixSize(const std::vector<int>& distance) {
    double root = std::sqrt(static_cast<double>(distance.size()));
    return root == static_cast<int>(root);
}

int main() {
    TSPGeneticSolver::solveTSP(TSPGeneticSolver::ParentOperator::Outbreeding, 
                               TSPGeneticSolver::CrossoverType::MultiPoint, 
                               {0, 6, 5, 9, 1, 9, 3, 1, 2, 5, 6, 0, 8, 9, 5, 4, 9, 5, 7, 8, 
                                5, 8, 0, 0, 8, 7, 2, 2, 5, 3, 9, 9, 0, 0, 2, 4, 2, 2, 6, 5, 
                                1, 5, 8, 2, 0, 9, 3, 1, 8, 1, 9, 4, 7, 4, 9, 0, 6, 2, 8, 7, 
                                3, 9, 2, 2, 3, 6, 0, 7, 6, 9, 1, 5, 2, 2, 1, 2, 7, 0, 3, 8, 
                                2, 7, 5, 6, 8, 8, 6, 3, 0, 7, 5, 8, 3, 5, 1, 7, 9, 8, 7, 0
                                }, 
                                1.0,
                                10);

    return 0;
}
