#include "SolveurTSPMTZ.hpp"
#include "gurobi_c++.h"

SolveurTSPMTZ::SolveurTSPMTZ(){}

SolveurTSPMTZ::SolveurTSPMTZ(Instance inst){
    this->inst = inst;
}

void SolveurTSPMTZ::solve(){
    try{
        //Get shortest distances between clients
        std::vector<std::vector<int>> distances = inst.getGraph().getTSPKernelDist();
        std::vector<int> demands = inst.getGraph().getDemands();
        int nb_clients = 0;
        std::vector<std::vector<double>> costs;
        for(int i = 0; i < distances.size(); i++){
            if(demands[i] == 0){
                continue;
            }
            std::vector<double> costs_i;
            nb_clients ++;
            for(int j = 0; j < distances.size(); j++){
                if(demands[j] == 0){
                    continue;
                }
                costs_i.push_back(distances[i][j]);
            }
            costs.push_back(costs_i);
        }

        //Model definition
        GRBEnv env = GRBEnv(true);
        env.set("LogFile", "mip1.log");
        env.start();


        GRBModel model = GRBModel(env);

        GRBVar **x = new GRBVar *[nb_clients];

        for (int i = 0; i < nb_clients; i++) {
            x[i] = new GRBVar[nb_clients];
            for (int j = 0; j < nb_clients; j++) {
                x[i][j] = model.addVar(0.0, 1.0, 1.0, GRB_BINARY);
            }
        }

        GRBVar *u = new GRBVar [nb_clients];
        for(int i = 0; i < nb_clients, i++){
            u[i] = model.addVar(lb = 0.0, vtype = GRB.CONITNUOUS);
        }

        //(1) et (2) Contraintes de flot
        for(int i = 0; i < n; i++) {
            GRBLinExpr expr1 = 0;
            GRBLinExpr expr2 = 0;
            for (j = 0; j < n; j++) {
                expr1 += x[i][j];
                expr2 += x[j][i];
            string s1 = "(1)" + itos(i) + "_" + itos(j);
            string s2 = "(2)" + itos(i) + "_" + itos(j);
            model.addConstr(expr1, GRB.EQUAL, 1.0, s1);
            model.addConstr(expr2, GRB.EQUAL, 1.0, s2);
            }
        }

        //(3) Contraintes de sous-tours
        for(int i = 0; i < nb_clients; i++){
            for(int j = 1; j < nb_clients; j++){
                String s = "(3)" + itos(i) + "_" + itos(j);
                model.addConstr(u[i] + x[i][j], GRB.LESS_EQUAL, u[j] + (nb_clients - 1) * (1 - x[i][j]), s);
            }
        }

        //Objective
        GRBLinExpr obj = 0;
        for(int i = 0; i < nb_clients; i++){
            for(int j = 0; j < nb_clients; j++){
                obj += x[i][j] * costs[i][j];
            }
        }
        model.setObjective(obj, GRB.MINIMIZE);

        model.optimize();

    }
}