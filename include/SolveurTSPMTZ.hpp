#ifndef SOLVEURTSPMTZ
#define SOLVEURTSPMTZ

#include "Solution/Solution.hpp"

class SolveurTSPMTZ{

    private :
    Instance inst;

    public :
    SolveurTSPMTZ();
    SolveurTSPMTZ(Instance instance);
    Solution solve();

};


#endif