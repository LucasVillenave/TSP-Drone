#ifndef Mod1V1
#define Mod1V1

#include <vector>
#include <string>

#include "Solution.hpp"

class Mod1V1{
    public : 
        int timeLimit;

        Mod1V1(int timeout){this->timeLimit = timeout;}

        Solution solve(FLPData instance);
};

#endif
