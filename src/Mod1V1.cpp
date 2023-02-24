// #include "gurobi_c++.h"
// #include "Mod1V1.hpp"



// // i supose everything is well loaded and the graph is kernilised
// Solution Mod1V1::solve(Instance t_instance){
    
//     Solution returnSol(t_instance,{});

//     try{

//         // int CardDU = 10;
//         int CardD = t_instance.getGraph().getDemands().size();

//         int CardV = t_instance.getGraph().getVertices().size();
//         std::stringstream name;

//         int nbr_dep_autor = 3;
//         // int nbr_dep_autor = CardDU;

//         /////////////////////////////////////////////////////////////////////////////
//         std::vector<int> m_ordre_clients;
//         m_ordre_clients.resize(CardD);
//         for (int d=0; d<CardD; ++d){
//             m_ordre_clients[d] = d;
//         }
//         int demande_deb_actuelle = 0;
//         int id_node_deb_actuelle = 0;
//         int demande_fin_actuelle = CardD;
//         int id_node_fin_actuelle = 0;
//         std::vector<int> sommets_actuels;
//         sommets_actuels.resize(CardV);
//         for (int i=0; i<CardV; ++i){
//             sommets_actuels[i] = i;
//         }
//         std::vector<double> variation_temps_cam_d;
//         variation_temps_cam_d.resize(2);
//         for (int a=0; a<=1; ++a){
//             variation_temps_cam_d[a] = 0;
//         }
//         std::vector<bool> client_satisfait;
//         client_satisfait.resize(CardD);
//         for (int d=0; d<client_satisfait.size(); ++d){
//             client_satisfait[d] = false;
//         }
//         std::vector<int> quantite_client_satisfait;
//         quantite_client_satisfait.resize(CardD);
//         for (int d=0; d<quantite_client_satisfait.size(); ++d){
//             quantite_client_satisfait[d] = 0;
//         }
//         std::vector<int> demandes_actuelles;
//         demandes_actuelles.resize(0);
//         for (int d=0; d<demande_fin_actuelle; ++d){
//             if (!client_satisfait[m_ordre_clients[d]]){
//                 demandes_actuelles.push_back(d);
//             }
//         }
//         /////////////////////////////////////////////////////////////////////////////


//         std::cout << "--> Creating the Gurobi environment" << std::endl;
//         GRBEnv env(true);
//         env.start();


//         std::cout << "--> Creating the Gurobi model" << std::endl;
//         GRBModel model(env);


//         // création variables
//         std::cout << "--> Creating the Gurobi variables" << std::endl;
//         GRBVar * Omega = new GRBVar[nbr_dep_autor+1];
//         for (int t=0; t<nbr_dep_autor+1; ++t){
//             name << "Omega[" << t << "]";
//             Omega[t] = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_CONTINUOUS, name.str());
//             name.str("");
//         }

//         GRBVar ** Delta = new GRBVar*[nbr_dep_autor];
//         for (int t=0; t<nbr_dep_autor; ++t){
//             Delta[t] = new GRBVar[2];
//             for (int a=0; a<=1; ++a){
//                 name << "Delta[" << t << "][" << a << "]";
//                 Delta[t][a] = model.addVar(0.0, 30.0, 0.0, GRB_CONTINUOUS, name.str());
//                 name.str("");
//             }
//         }

//         GRBVar *** X = new GRBVar**[nbr_dep_autor];
//         X[0] = new GRBVar*[1];
//         X[0][0] = new GRBVar[sommets_actuels.size()+1];
//         for (int j=1; j<sommets_actuels.size()+1; ++j){
//             name << "X[" << 0 << "][" << 0 << "][" << j << "]";
//             X[0][0][j] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, name.str());
//             name.str("");
//         }
//         for (int t=1; t<nbr_dep_autor-1; ++t){
//             X[t] = new GRBVar*[sommets_actuels.size()+1];
//             for (int i=0; i<sommets_actuels.size(); ++i){
//                 X[t][i] = new GRBVar[sommets_actuels.size()+1];
//                 for (int j=0; j<sommets_actuels.size()+1; ++j){
//                     if (i!=j){
//                         name << "X[" << t << "][" << i << "][" << j << "]";
//                         X[t][i][j] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, name.str());
//                         name.str("");
//                     }
//                 }
//             }
//             X[t][sommets_actuels.size()] = new GRBVar[1];
//             name << "X[" << t << "][" << sommets_actuels.size() << "][" << sommets_actuels.size() << "]";
//             X[t][sommets_actuels.size()][0] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, name.str());
//             name.str("");
//         }
//         X[nbr_dep_autor-1] = new GRBVar*[sommets_actuels.size()+1];
//         for (int i=0; i<sommets_actuels.size()+1; ++i){
//             X[nbr_dep_autor-1][i] = new GRBVar[1];
//             name << "X[" << nbr_dep_autor-1 << "][" << i << "][" << sommets_actuels.size() << "]";
//             X[nbr_dep_autor-1][i][0] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, name.str());
//             name.str("");
//         }

//         GRBVar ** Y = new GRBVar*[nbr_dep_autor];
//         for (int t=0; t<nbr_dep_autor; ++t){
//             Y[t] = new GRBVar[demandes_actuelles.size()];
//             for (int d=0; d<demandes_actuelles.size(); ++d){
//                 name << "Y[" << t << "][" << d << "]";
//                 Y[t][d] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, name.str());
//                 name.str("");
//             }
//         }

//         GRBVar ***** Z = new GRBVar****[nbr_dep_autor];
//         Z[0] = new GRBVar***[2];
//         for (int a=0; a<=1; ++a){
//             Z[0][a] = new GRBVar**[demandes_actuelles.size()];
//             for (int d=0; d<demandes_actuelles.size(); ++d){
//                 Z[0][a][d] = new GRBVar*[t_instance.getGraph().getDemand(demandes_actuelles[d]).getAmount() - quantite_client_satisfait[demandes_actuelles[d]]];
//                 for (int eps=0; eps<t_instance.getGraph().getDemand(demandes_actuelles[d]).getAmount() - quantite_client_satisfait[demandes_actuelles[d]]; ++eps){
//                     Z[0][a][d][eps] = new GRBVar[1];
//                     name << "Z[" << 0 << "][" << a << "][" << d << "][" << eps << "][" << 0 << "]";
//                     Z[0][a][d][eps][0] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, name.str());
//                     name.str("");
//                 }
//             }
//         }
//         for (int t=1; t<nbr_dep_autor; ++t){
//             Z[t] = new GRBVar***[2];
//             for (int a=0; a<=1; ++a){
//                 Z[t][a] = new GRBVar**[demandes_actuelles.size()];
//                 for (int d=0; d<demandes_actuelles.size(); ++d){
//                     Z[t][a][d] = new GRBVar*[t_instance.getGraph().getDemand(demandes_actuelles[d]).getAmount() - quantite_client_satisfait[demandes_actuelles[d]]];
//                     for (int eps=0; eps<t_instance.getGraph().getDemand(demandes_actuelles[d]).getAmount() - quantite_client_satisfait[demandes_actuelles[d]]; ++eps){
//                         Z[t][a][d][eps] = new GRBVar[sommets_actuels.size()];
//                         for (int i=0; i<sommets_actuels.size(); ++i){
//                             name << "Z[" << t << "][" << a << "][" << d << "][" << eps << "][" << i << "]";
//                             Z[t][a][d][eps][i] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, name.str());
//                             name.str("");
//                         }
//                     }
//                 }
//             }
//         }

//         GRBVar ** Zeta = new GRBVar*[nbr_dep_autor];
//         for (int t=0; t<nbr_dep_autor; ++t){
//             Zeta[t] = new GRBVar[2];
//             for (int a=0; a<=1; ++a){
//                 name << "Zeta[" << t << "][" << a << "]";
//                 Zeta[t][a] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, name.str());
//                 name.str("");
//             }
//         }

//         // création objectif
//         std::cout << "--> Creating the Objective" << std::endl;
//         GRBLinExpr obj = Omega[nbr_dep_autor];
//         model.setObjective(obj, GRB_MINIMIZE);


//         // création contraintes
//         std::cout << "--> Creating Constraints : " << std::endl;

//         // -> flot
//         std::cout << "                           - flot" << std::endl;
//         // debut
//         GRBLinExpr expdu = 0;
//         for (int j=1; j<sommets_actuels.size()+1; ++j){
//             expdu += X[0][0][j];
//         }
//         name << "deb flot";
//         model.addConstr(expdu==1,name.str());
//         name.str("");
//         // consevation temps 1
//         for (int i=1; i<sommets_actuels.size(); ++i){
//             GRBLinExpr exp = 0;
//             exp += X[0][0][i];
//             for (int j=0; j<sommets_actuels.size(); ++j){
//                 if (i!=j){
//                     exp -= X[1][i][j];
//                 }
//             }
//             exp -= X[1][i][sommets_actuels.size()];
//             name << "flot " << 0 << " , " << i;
//             model.addConstr(exp==0,name.str());
//             name.str("");
//         }
//         GRBLinExpr expdc = 0;
//         expdc += X[0][0][sommets_actuels.size()];
//         expdc -= X[1][sommets_actuels.size()][0];
//         name << "flot " << 0 << " , " << sommets_actuels.size();
//         model.addConstr(expdc==0,name.str());
//         name.str("");
//         // consevation temps 2 <-> nbr_dep_autor-2
//         for (int t=1; t<nbr_dep_autor-2; ++t){
//             for (int i=0; i<sommets_actuels.size(); ++i){
//                 GRBLinExpr exp = 0;
//                 for (int j=0; j<sommets_actuels.size(); ++j){
//                     if (i!=j){
//                         exp += X[t][j][i] - X[t+1][i][j];
//                     }
//                 }
//                 exp -= X[t+1][i][sommets_actuels.size()];
//                 name << "flot " << t << " , " << i;
//                 model.addConstr(exp==0,name.str());
//                 name.str("");
//             }
//             GRBLinExpr exp = 0;
//             for (int j=0; j<sommets_actuels.size(); ++j){
//                 exp += X[t][j][sommets_actuels.size()];
//             }
//             exp += X[t][sommets_actuels.size()][0];
//             exp -= X[t+1][sommets_actuels.size()][0];
//             name << "flot " << t << " , " << sommets_actuels.size();
//             model.addConstr(exp==0,name.str());
//             name.str("");
//         }
//         // consevation temps nbr_dep_autor-1
//         for (int i=0; i<sommets_actuels.size(); ++i){
//             GRBLinExpr exp = 0;
//             exp += X[nbr_dep_autor-1][i][0];
//             for (int j=0; j<sommets_actuels.size(); ++j){
//                 if (i!=j){
//                     exp -= X[nbr_dep_autor-2][j][i];
//                 }
//             }
//             name << "flot " << 0 << " , " << i;
//             model.addConstr(exp==0,name.str());
//             name.str("");
//         }
//         GRBLinExpr expfc = 0;
//         expfc += X[nbr_dep_autor-2][sommets_actuels.size()][0];
//         for (int i=0; i<sommets_actuels.size(); ++i){
//             expfc += X[nbr_dep_autor-2][i][sommets_actuels.size()];
//         }
//         expfc -= X[nbr_dep_autor-1][sommets_actuels.size()][0];
//         name << "flot " << 0 << " , " << sommets_actuels.size();
//         model.addConstr(expfc==0,name.str());
//         name.str("");
//         // fin
//         GRBLinExpr expf = 0;
//         for (int i=0; i<sommets_actuels.size()+1; ++i){
//             expf += X[nbr_dep_autor-1][i][0];
//         }
//         name << "fin flot";
//         model.addConstr(expf==1,name.str());
//         name.str("");


//         // X / Y
//         std::cout << "                           - X / Y" << std::endl;
//         for (int d=0; d<demandes_actuelles.size(); ++d){
//             if (id_node_deb_actuelle != t_instance.getGraph().getDemand(demandes_actuelles[d]).getNodeGraphID()){
//                 GRBLinExpr exp = 0;
//                 exp += Y[0][d];
//                 name << "X / Y " << 0 << " , " << d;
//                 model.addConstr(exp<=0,name.str());
//                 name.str("");
//             }
//         }
//         for (int t=1; t<nbr_dep_autor-1; ++t){
//             for (int d=0; d<demandes_actuelles.size(); ++d){
//                 GRBLinExpr exp = 0;
//                 exp += Y[t][d];
//                 for (int i=0; i<sommets_actuels.size(); ++i){
//                     if (sommets_actuels[i] != t_instance.getGraph().getDemand(demandes_actuelles[d]).getNodeGraphID()){
//                         for (int j=0; j<sommets_actuels.size(); ++j){
//                             if (i != j){
//                                 exp += X[t][i][j];
//                             }
//                         }
//                     }
//                 }
//                 name << "X / Y " << t << " , " << d;
//                 model.addConstr(exp<=1,name.str());
//                 name.str("");
//             }
//         }
//         for (int d=0; d<demandes_actuelles.size(); ++d){
//             GRBLinExpr exp = 0;
//             exp += Y[nbr_dep_autor-1][d];
//             for (int i=0; i<sommets_actuels.size(); ++i){
//                 if (sommets_actuels[i] != t_instance.getGraph().getDemand(demandes_actuelles[d]).getNodeGraphID()){
//                     exp += X[nbr_dep_autor-1][i][0];
//                 }
//             }
//             name << "X / Y " << nbr_dep_autor-1 << " , " << d;
//             model.addConstr(exp<=1,name.str());
//             name.str("");
//         }


//         // X / Z
//         std::cout << "                           - X / Z" << std::endl;
//         for (int a=0; a<=1; ++a){
//             for (int i=0; i<sommets_actuels.size(); ++i){
//                 for (int t=1; t<nbr_dep_autor-1; ++t){
//                     for (int d=0; d<demandes_actuelles.size(); ++d){
//                         for (int eps=0; eps<t_instance.getGraph().getDemand(demandes_actuelles[d]).getAmount() - quantite_client_satisfait[demandes_actuelles[d]]; ++eps){
//                             GRBLinExpr exp = 0;
//                             exp += Z[t][a][d][eps][i];
//                             for (int j=0; j<sommets_actuels.size(); ++j){
//                                 if (i != j){
//                                     exp -= X[t][i][j];
//                                 }
//                             }
//                             name << "X / Z " << t << " , " << a << " , " << d << " , " << eps << " , " << i;
//                             model.addConstr(exp<=0,name.str());
//                             name.str("");
//                         }
//                     }
//                 }
//                 for (int d=0; d<demandes_actuelles.size(); ++d){
//                     for (int eps=0; eps<t_instance.getGraph().getDemand(demandes_actuelles[d]).getAmount() - quantite_client_satisfait[demandes_actuelles[d]]; ++eps){
//                         GRBLinExpr exp = 0;
//                         exp += Z[nbr_dep_autor-1][a][d][eps][i];
//                         exp -= X[nbr_dep_autor-1][i][0];
//                         name << "X / Z " << nbr_dep_autor-1 << " , " << a << " , " << d << " , " << eps << " , " << i;
//                         model.addConstr(exp<=0,name.str());
//                         name.str("");
//                     }
//                 }
//             }
//         }

//         // Z / Zeta
//         std::cout << "                           - Z / Zeta" << std::endl;
//         for (int a=0; a<=1; ++a){
//             for (int d=0; d<demandes_actuelles.size(); ++d){
//                 for (int eps=0; eps<t_instance.getGraph().getDemand(demandes_actuelles[d]).getAmount() - quantite_client_satisfait[demandes_actuelles[d]]; ++eps){
//                     GRBLinExpr exp = 0;
//                     exp += Z[0][a][d][eps][0];
//                     exp -= Zeta[0][a];
//                     name << "Z / Zeta " << 0 << " , " << a << " , " << d << " , " << eps << " , " << 0;
//                     model.addConstr(exp<=0,name.str());
//                     name.str("");
//                 }
//             }
//         }
//         for (int t=1; t<nbr_dep_autor; ++t){
//             for (int a=0; a<=1; ++a){
//                 for (int d=0; d<demandes_actuelles.size(); ++d){
//                     for (int eps=0; eps<t_instance.getGraph().getDemand(demandes_actuelles[d]).getAmount() - quantite_client_satisfait[demandes_actuelles[d]]; ++eps){
//                         for (int i=0; i<sommets_actuels.size(); ++i){
//                             GRBLinExpr exp = 0;
//                             exp += Z[t][a][d][eps][i];
//                             exp -= Zeta[t][a];
//                             name << "Z / Zeta " << t << " , " << a << " , " << d << " , " << eps << " , " << i;
//                             model.addConstr(exp<=0,name.str());
//                             name.str("");
//                         }
//                     }
//                 }
//             }
//         }

//         // Omega / Y
//         std::cout << "                           - Omega / Y" << std::endl;
//         GRBLinExpr expOY0 = 0;
//         expOY0 += Omega[0];
//         expOY0 -= Omega[1];
//         for (int j=1; j<sommets_actuels.size(); ++j){
//             expOY0 += X[0][0][j] * t_instance.getGraph().getTSPKernelTime(id_node_deb_actuelle,sommets_actuels[j]);
//         }
//         expOY0 += X[0][0][sommets_actuels.size()] * t_instance.getGraph().getTSPKernelTime(id_node_deb_actuelle,id_node_fin_actuelle);
//         for (int d=0; d<demandes_actuelles.size(); ++d){
//             expOY0 += Y[0][d] * t_instance.getTruckDeliveryTime();
//         }
//         name << "Omega / Y " << 0;
//         model.addConstr(expOY0<=0,name.str());
//         name.str("");
//         for (int t=1; t<nbr_dep_autor-1; ++t){
//             GRBLinExpr exp = 0;
//             exp += Omega[t];
//             exp -= Omega[t+1];
//             for (int i=0; i<sommets_actuels.size(); ++i){
//                 for (int j=0; j<sommets_actuels.size(); ++j){
//                     if (i!=j){
//                         exp += X[t][i][j] * t_instance.getGraph().getTSPKernelTime(sommets_actuels[i],sommets_actuels[j]);
//                     }
//                 }
//                 exp += X[t][i][sommets_actuels.size()] * t_instance.getGraph().getTSPKernelTime(sommets_actuels[i],id_node_fin_actuelle);
//             }
//             for (int d=0; d<demandes_actuelles.size(); ++d){
//                 exp += Y[t][d] * t_instance.getTruckDeliveryTime();
//             } 
//             name << "Omega / Y " << t;
//             model.addConstr(exp<=0,name.str());
//             name.str("");
//         }
//         GRBLinExpr expOYf = 0;
//         expOYf += Omega[nbr_dep_autor-1];
//         expOYf -= Omega[nbr_dep_autor];
//         for (int i=0; i<sommets_actuels.size(); ++i){
//             expOYf += X[nbr_dep_autor-1][i][0] * t_instance.getGraph().getTSPKernelTime(sommets_actuels[i],id_node_fin_actuelle);
//         }
//         for (int d=0; d<demandes_actuelles.size(); ++d){
//             expOYf += Y[nbr_dep_autor-1][d] * t_instance.getTruckDeliveryTime();
//         } 
//         name << "Omega / Y " << nbr_dep_autor-1;
//         model.addConstr(expOYf<=0,name.str());
//         name.str("");

//         // Omega / Z
//         std::cout << "                           - Omega / Z" << std::endl;
//         for (int a=0; a<=1; ++a){
//             GRBLinExpr exp = 0;
//             exp += Omega[0];
//             exp -= Omega[1];
//             for (int j=1; j<sommets_actuels.size(); ++j){
//                 exp += X[0][0][j] * t_instance.getGraph().getTSPKernelTime(id_node_deb_actuelle,sommets_actuels[j]);
//             }
//             exp += X[0][0][sommets_actuels.size()] * t_instance.getGraph().getTSPKernelTime(id_node_deb_actuelle,id_node_fin_actuelle);
//             for (int d=0; d<demandes_actuelles.size(); ++d){
//                 for (int eps=0; eps<t_instance.getGraph().getDemand(demandes_actuelles[d]).getAmount() - quantite_client_satisfait[demandes_actuelles[d]]; ++eps){
//                     exp += Z[0][a][d][eps][0] * ( 2 * euclidianDistance(t_instance.getGraph().getVertice(id_node_deb_actuelle).getPos(), t_instance.getGraph().getVertice(t_instance.getGraph().getDemand(demandes_actuelles[d]).getNodeGraphID()).getPos()) / t_instance.getDroneSpeed() );
//                     exp += Z[0][a][d][eps][0] * 30;
//                 }
//             }
//             exp += Delta[0][a];
//             name << "Omega / Z " << 0 << " , " << a;
//             model.addConstr(exp<=30,name.str());
//             name.str("");
//         }
//         for (int t=1; t<nbr_dep_autor-1; ++t){
//             for (int a=0; a<=1; ++a){
//                 GRBLinExpr exp = 0;
//                 exp += Omega[t];
//                 exp -= Omega[t+1];
//                 for (int i=0; i<sommets_actuels.size(); ++i){
//                     for (int j=0; j<sommets_actuels.size(); ++j){
//                         if (i!=j){
//                             exp += X[t][i][j] * t_instance.getGraph().getTSPKernelTime(sommets_actuels[i],sommets_actuels[j]);
//                         }
//                     }
//                     exp += X[t][i][sommets_actuels.size()] * t_instance.getGraph().getTSPKernelTime(sommets_actuels[i],id_node_fin_actuelle);
//                 }
//                 for (int d=0; d<demandes_actuelles.size(); ++d){
//                     for (int eps=0; eps<t_instance.getGraph().getDemand(demandes_actuelles[d]).getAmount() - quantite_client_satisfait[demandes_actuelles[d]]; ++eps){
//                         for (int i=0; i<sommets_actuels.size(); ++i){
//                             exp += Z[t][a][d][eps][i] * ( 2 * euclidianDistance(t_instance.getGraph().getVertice(sommets_actuels[i]).getPos(), t_instance.getGraph().getVertice(t_instance.getGraph().getDemand(demandes_actuelles[d]).getNodeGraphID()).getPos()) / t_instance.getDroneSpeed() );
//                             exp += Z[t][a][d][eps][i] * 30;
//                         }
//                     }
//                 }
//                 exp += Delta[t][a];
//                 name << "Omega / Z " << t << " , " << a;
//                 model.addConstr(exp<=30,name.str());
//                 name.str("");
//             }
//         }
//         for (int a=0; a<=1; ++a){
//             GRBLinExpr exp = 0;
//             exp += Omega[nbr_dep_autor-1];
//             exp -= Omega[nbr_dep_autor];
//             for (int i=0; i<sommets_actuels.size(); ++i){
//                 exp += X[nbr_dep_autor-1][i][0] * t_instance.getGraph().getTSPKernelTime(sommets_actuels[i],id_node_fin_actuelle);
//             }
//             for (int d=0; d<demandes_actuelles.size(); ++d){
//                 for (int eps=0; eps<t_instance.getGraph().getDemand(demandes_actuelles[d]).getAmount() - quantite_client_satisfait[demandes_actuelles[d]]; ++eps){
//                     for (int i=0; i<sommets_actuels.size(); ++i){
//                         exp += Z[nbr_dep_autor-1][a][d][eps][i] * ( 2 * euclidianDistance(t_instance.getGraph().getVertice(sommets_actuels[i]).getPos(), t_instance.getGraph().getVertice(t_instance.getGraph().getDemand(demandes_actuelles[d]).getNodeGraphID()).getPos()) / t_instance.getDroneSpeed() );
//                         exp += Z[nbr_dep_autor-1][a][d][eps][i] * 30;
//                     }
//                 }
//             }
//             exp += Delta[nbr_dep_autor-1][a];
//             name << "Omega / Z " << 0 << " , " << a;
//             model.addConstr(exp<=30,name.str());
//             name.str("");
//         }

//         // Delta / Zeta
//         std::cout << "                           - Delta / Zeta" << std::endl;
//         for (int a=0; a<=1; ++a){
//             for (int t_plus=0; t_plus<nbr_dep_autor; ++t_plus){
//                 GRBLinExpr exp = 0;
//                 exp += Zeta[t_plus][a] * variation_temps_cam_d[a];
//                 if (t_plus > 0){
//                     for (int j=1; j<sommets_actuels.size(); ++j){
//                         exp -= X[0][0][j] * t_instance.getGraph().getTSPKernelTime(id_node_deb_actuelle,sommets_actuels[j]);
//                     }
//                 }
//                 for (int t_prime=1; t_prime<t_plus; ++t_prime){
//                     for (int i=0; i<sommets_actuels.size(); ++i){
//                         for (int j=0; j<sommets_actuels.size(); ++j){
//                             if (i!=j){
//                                 exp -= X[t_prime][i][j] * t_instance.getGraph().getTSPKernelTime(sommets_actuels[i],sommets_actuels[j]);
//                             }
//                         }
//                     }
//                 }
//                 exp -= Delta[t_plus][a];
//                 name << "Delta / Zeta " << -1 << " , " << t_plus << " , " << a;
//                 model.addConstr(exp<=0,name.str());
//                 name.str("");
//                 for (int t_moins=0; t_moins<t_plus; ++t_moins){
//                     GRBLinExpr exp = 0;
//                     exp += (Zeta[t_plus][a] + Zeta[t_moins][a] - 1 ) * 30;
//                     for (int t_prime=t_moins; t_prime<t_plus; ++t_prime){
//                         if (t_prime == 0){
//                             for (int j=1; j<sommets_actuels.size(); ++j){
//                                 exp -= X[0][0][j] * t_instance.getGraph().getTSPKernelTime(id_node_deb_actuelle,sommets_actuels[j]);
//                             }
//                         }
//                         else {
//                             for (int i=0; i<sommets_actuels.size(); ++i){
//                                 for (int j=0; j<sommets_actuels.size(); ++j){
//                                     if (i!=j){
//                                         exp -= X[t_prime][i][j] * t_instance.getGraph().getTSPKernelTime(sommets_actuels[i],sommets_actuels[j]);
//                                     }
//                                 }
//                             }
//                         }
//                     }
//                     exp -= Delta[t_plus][a];
//                     name << "Delta / Zeta " << t_moins << " , " << t_plus << " , " << a;
//                     model.addConstr(exp<=0,name.str());
//                     name.str("");
//                 }
//             }
//         }

//         // Demande
//         std::cout << "                           - Demande" << std::endl;
//         for (int d=0; d<demandes_actuelles.size(); ++d){
//             for (int eps=0; eps<t_instance.getGraph().getDemand(demandes_actuelles[d]).getAmount() - quantite_client_satisfait[demandes_actuelles[d]]; ++eps){
//                 GRBLinExpr exp = 0;
//                 for (int a=0; a<=1; ++a){
//                     for (int t=1; t<nbr_dep_autor; ++t){
//                         for (int i=0; i<sommets_actuels.size(); ++i){
//                             exp += Z[t][a][d][eps][i];
//                         }
//                     }
//                     exp += Z[0][a][d][eps][0];
//                 }
//                 for (int t=0; t<nbr_dep_autor; ++t){
//                     exp += Y[t][d];
//                 }
//                 name << "Demande " << d << " , " << eps;
//                 model.addConstr(exp==1,name.str());
//                 name.str("");
//             }
//         }



//         std::cout <<"----> setting model" << std::endl;
//         model.set(GRB_DoubleParam_TimeLimit, this->timeLimit);


//         std::cout <<"----> model solving"<< std::endl;
//         model.optimize();


//         int status = model.get(GRB_IntAttr_Status);
//         if (status == GRB_OPTIMAL || (status== GRB_TIME_LIMIT && model.get(GRB_IntAttr_SolCount)>0)){
//             std::cout << "Succes! (Status: " << model.get(GRB_IntAttr_Status) << ")" << std::endl;
//             std::cout<<"--> Printing results "<< std::endl;
//             std::cout << "Runtime : " << model.get(GRB_DoubleAttr_Runtime) << " seconds"<< std::endl;

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