// #include "gurobi_c++.h"
// #include "Mod1V1.hpp"



// // i supose everything is well loaded and the graph is kernilised
// Solution Mod1V1::solve(FLPData instance){
    
//     Solution returnSol;

//     try{
//         int CardDU = instance.getGraph().getNbr_demandes_unitaires();
//         int CardV = instance.getGraph().getVertices().size();
//         std::stringstream name;


//         std::cout << "--> Creating the Gurobi environment" << std::endl;
//         GRBEnv env(true);
//         env.start();


//         std::cout << "--> Creating the Gurobi model" << std::endl;
//         GRBModel model(env);


//         std::cout << "--> Creating the Gurobi variables" << std::endl;

//         GRBVar * Omega = new GRBVar[CardDU+2];
//         for (int t=0; t<=CardDU+1; ++t){
//             name << "Omega[" << t << "]";
//             Omega[t] = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_CONTINUOUS, name.str());
//             name.str("");
//         }

//         GRBVar * Delta = new GRBVar[CardDU+1];
//         for (int t=0; t<=CardDU; ++t){
//             name << "Delta[" << t << "]";
//             Delta[t] = model.addVar(0.0, 30.0, 0.0, GRB_CONTINUOUS, name.str());
//             name.str("");
//         }

//         GRBVar *** X = new GRBVar**[CardDU+1];
//         for (int t=0; t<=CardDU; ++t){
//             X[t] = new GRBVar*[CardV];
//             for (int i=0; i<CardV; ++i){
//                 X[t][i] = new GRBVar[CardV+1];
//                 for (int j=0; j<CardV+1; ++j){
//                     if (i!=j){
//                         name << "X[" << t << "][" << i << "][" << j << "]";
//                         X[t][i][j] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, name.str());
//                         name.str("");
//                     }
//                 }
//             }
//         }

//         GRBVar ** Y = new GRBVar*[CardDU+1];
//         for (int t=0; t<=CardDU; ++t){
//             Y[t] = new GRBVar*[CardDU];
//             for (int d=0; d<CardDU; ++d){
//                 name << "Y[" << t << "][" << d << "]";
//                 Y[t][d] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, name.str());
//                 name.str("");
//             }
//         }

//         GRBVar *** Z = new GRBVar***[CardDU+1];
//         for (int t=0; t<=CardDU; ++t){
//             Z[t] = new GRBVar**[2];
//             for (int a=0; a<=1; ++a)
//                 Z[t][a] = new GRBVar[CardDU];
//                 for (int d=0; d<CardDU; ++d){
//                     name << "Z[" << t << "][" << a << "][" << d << "]";
//                     Z[t][a][d] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, name.str());
//                     name.str("");
//                 }
//         }
        

//         std::cout << "--> Creating the Objective" << std::endl;

//         GRBLinExpr obj = Omega[CardDU+1];
//         model.setObjective(obj, GRB_MINIMIZE);


//         std::cout << "--> Creating Constraints" << std::endl;

//         for (int t=0; t<=CardDU-1; ++t){
//             for (int i=0; i<CardV; ++i){
//                 GRBLinExpr exp = 0;
//                 for (int j=0; j<CardV; ++j){
//                     if (i!=j){
//                         exp += X[t][j][i] - X[t+1][i][j];
//                     }
//                 }
//                 exp -= X[t+1][i][CardV]
//                 name << "flot " << t << " , " << i;
//                 model.addConstr(exp==0,name.str());
//                 name.str("");
//             }
//         }

//         GRBLinExpr exp = 0;
//         for (int j=1; j<CardV; ++j){
//             exp += X[0][0][j];
//         }
//         name << "deb flot";
//         model.addConstr(exp==1,name.str());
//         name.str("");

//         GRBLinExpr exp = 0;
//         for (int t=0; t<=CardDU; ++t){
//             for (int i=0; i<CardV; ++i){
//                 exp += X[t][i][CardV];
//             }
//         }
//         name << "fin flot";
//         model.addConstr(exp==1,name.str());
//         name.str("");































        // std::cout <<"----> setting model" << std::endl;
        // model.set(GRB_DoubleParam_TimeLimit, this->timeLimit);

        // std::cout <<"----> model solving"<< std::endl;
        // model.optimize();

        // int status = model.get(GRB_IntAttr_Status);
        // if (status == GRB_OPTIMAL || (status== GRB_TIME_LIMIT && model.get(GRB_IntAttr_SolCount)>0)){
        //     std::cout << "Succes! (Status: " << status << ")" << std::endl;
        //     std::cout<<"--> Printing results "<< std::endl;
        //     std::cout << "Runtime : " << model.get(GRB_DoubleAttr_Runtime) << " seconds"<< std::endl;

        //     model.write("solution.sol");
        //     std::cout << "Objective value = "<< model.get(GRB_DoubleAttr_ObjVal)  << std::endl;

        //     std::vector<std::vector<std::vector<int>>> SX(instance.nbPeriode);
        //     std::vector<std::vector<int>> SY(instance.nbPeriode);
        //     for (int t=0; t<instance.nbPeriode; ++t){
        //         SX[t] = std::vector<std::vector<int>>(instance.nbDepotPotentiel);
        //         SY[t] = std::vector<int>(instance.nbDepotPotentiel);
        //         for (int i=0; i<instance.nbDepotPotentiel; ++i){
        //             if (Y[t][i].get(GRB_DoubleAttr_X)>0){
        //                 std::cout << "Y[" << t << "][" << i << "] = 1" << std::endl;
        //                 SY[t][i] = 1;    
        //             }else{
        //                 SY[t][i] = 0;
        //             }
        //             SX[t][i] = std::vector<int>(instance.nbClient);
        //             for (int j=0; j<instance.nbClient; ++j){
        //                 if (X[t][i][j].get(GRB_DoubleAttr_X)>0){
        //                     std::cout << "X[" << t << "][" << i << "][" << j << "] = 1" << std::endl;
        //                     SX[t][i][j] = 1;
        //                 }else{
        //                     SX[t][i][j] = 0;
        //                 }
        //             }
        //         }
        //     }
        //     returnSol = FLPSolution(SY,SX);
        //     std::cout << "Solution feasability : " << returnSol.check(instance) << std::endl;
        //     // std::cout << "nmax = " << instance.n[instance.nbPeriode-1] << std::endl;
        //     // std::cout << counter << std::endl;
        //     // for (int t=0; t<instance.nbPeriode; ++t){
        //     //     std::cout << "p = " << instance.p[t] << std::endl;
        //     // }
        // }






//     }catch (GRBException e)
//     {
//         std::cout << "Error code = " << e.getErrorCode() << std::endl;
//         std::cout << e.getMessage() << std::endl;
//     }
//     catch (...)
//     {
//         std::cout << "Exception during optimization" << std::endl;
//     } 
//     return returnSol;







// }