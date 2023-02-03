#ifndef CONSTRUCTIVESOLVE
#define CONSTRUCTIVESOLVE

#include "Solution/Solution.hpp"
#include "utils.hpp"

class ConstructiveSolve{
    private :
        std::vector<std::vector<double>> droneDists;
        std::vector<int> x; 
        std::vector<std::vector<int>> z;

        //savings are an approximation of wether a node would be worth to add to truck tourney
        std::vector<double> savings;
        int n;
        int m;
        double truckTravelCost = 0;

        std::vector<double> nodeDemandCost;
        std::vector<std::vector<double>> nodeDroneCost;
        double totalCost;

        void updateSavings(const Instance& instance);
        void greedyDemandAffectation(const Instance& instance);
        void calculateTotalCosts(const Instance& instance);
        int getCost(std::vector<std::vector<std::vector<int>>> x, std::vector<std::vector<std::vector<int>>> z, const Instance& instance);

        Solution SolveCase1(Instance instance);
        Solution SolveCase2(Instance instance);
        Solution SolveCase3(Instance instance);

        Solution convertToSolutionCase1(Instance instance);

    public :
        ConstructiveSolve();
        Solution Solve(int scenario, Instance instance);
};

#endif