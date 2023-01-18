#include "SolveurTSPMTZ.hpp"
#include "gurobi_c++.h" 

SolveurTSPMTZ::SolveurTSPMTZ(){}

SolveurTSPMTZ::SolveurTSPMTZ(Instance inst){
    this->inst = inst;
}

void SolveurTSPMTZ::solve(){
    //Get shortest distances between clients
    Graph graph = inst.getGraph();
    std::vector<std::vector<double>> distances = graph.getTSPKernelDist();
    std::vector<Demand> demands = graph.getDemands();
    int nb_clients = 0;
    std::vector<std::vector<double>> costs;
    for(int i = 0; i < distances.size(); i++){
        if(demands[i].getAmount() <= 0){
            continue;
        }
        std::vector<double> costs_i;
        nb_clients ++;
        for(int j = 0; j < distances.size(); j++){
            if(demands[j].getAmount() <= 0){
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
            x[i][j] = model.addVar(0.0, 1.0, costs[i][j], GRB_BINARY);
        }
    }

    GRBVar *u = new GRBVar [nb_clients];
    for(int i = 0; i < nb_clients; i++){
        u[i] = model.addVar(0.0, 2 * nb_clients, 0.0, GRB_CONTINUOUS);
    }

    //(1) et (2) Contraintes de flot
    for(int i = 0; i < nb_clients; i++) {
        GRBLinExpr expr1 = 0;
        GRBLinExpr expr2 = 0;
        for (int j = 0; j < nb_clients; j++) {
            expr1 += x[i][j];
            expr2 += x[j][i];
        std::string s1 = "(1)" + std::to_string(i) + "_" + std::to_string(j);
        std::string s2 = "(2)" + std::to_string(i) + "_" + std::to_string(j);
        model.addConstr(expr1, GRB_EQUAL, 1.0, s1);
        model.addConstr(expr2, GRB_EQUAL, 1.0, s2);
        }
    }

    //(3) Contraintes de sous-tours
    for(int i = 0; i < nb_clients; i++){
        for(int j = 1; j < nb_clients; j++){
            std::string s = "(3)" + std::to_string(i) + "_" + std::to_string(j);
            model.addConstr(u[i] + x[i][j], GRB_LESS_EQUAL, u[j] + (nb_clients - 1) * (1 - x[i][j]), s);
        }
    }

    //Objective
    GRBLinExpr obj = 0;
    for(int i = 0; i < nb_clients; i++){
        for(int j = 0; j < nb_clients; j++){
            obj += x[i][j] * costs[i][j];
        }
    }
    model.setObjective(obj, GRB_MINIMIZE);

    model.optimize();

}