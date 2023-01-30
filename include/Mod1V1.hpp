#ifndef MOD1V1
#define MOD1V1

#include <vector>
#include <string>

#include "Solution/Solution.hpp"
#include "utils.hpp"

class Mod1V1{
    public : 
        int timeLimit;

        Mod1V1(int timeout){this->timeLimit = timeout;}

        Solution solve(Instance instance);
};

#endif
