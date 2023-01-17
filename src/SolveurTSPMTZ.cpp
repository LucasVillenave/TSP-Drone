#include "SolveurTSPMTZ.hpp"
#include "gurobi_c++.h"

SolveurTSPMTZ::SolveurTSPMTZ(){}

SolveurTSPMTZ::SolveurTSPMTZ(Instance inst){
    this->inst = inst;
}

Solution SolveurTSPMTZ::solve(){
    try{
        GRBEnv env = GRBEnv(true);
        env.set("LogFile", "mip1.log");
        env.start();

        std::vector<std::vector<double>> distances = inst.getGraph().getTSPKernelDist();
        GRBModel model = GRBModel(env);

        
        GRBVar


    }
}