// #include "gurobi_c++.h"
// #include "Mod1V1.hpp"


// Solution Mod1V1::solve(FLPData instance){
    
//     Solution returnSol;

//     try{
//         std::cout << "--> Creating the Gurobi environment" << std::endl;
//         GRBEnv env(true);
//         env.start();

//         std::cout << "--> Creating the Gurobi model" << std::endl;
//         GRBModel model(env);

//         std::cout << "--> Creating the Gurobi variables" << std::endl;

//         std::stringstream name;






//         GRBVar ** Y = new GRBVar*[instance.nbPeriode];
//         GRBVar *** X = new GRBVar**[instance.nbPeriode];
//         for (int t=0; t<instance.nbPeriode; ++t){
//             Y[t] = new GRBVar[instance.nbDepotPotentiel];
//             X[t] = new GRBVar*[instance.nbDepotPotentiel];
//             for (int i=0; i<instance.nbDepotPotentiel; ++i){
//                 name << "Y[" << t << "][" << i << "]";
//                 Y[t][i] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, name.str());
//                 name.str("");

//                 X[t][i] = new GRBVar[instance.nbClient];
//                 for (int j=0; j<instance.nbClient; ++j){
//                     name << "X[" << t << "][" << i << "][" << j << "]";
//                     X[t][i][j] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, name.str());
//                     name.str("");
//                 }
//             }
//         }

        











































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