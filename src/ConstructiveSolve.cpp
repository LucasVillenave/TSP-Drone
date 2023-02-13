#include "ConstructiveSolve.hpp"
#include <climits>

ConstructiveSolve::ConstructiveSolve(){}

Solution ConstructiveSolve::Solve(int scenario,Instance instance){

    const std::vector<Vertex>& vertices = instance.getGraph().getVertices();
    n = vertices.size();
    m = instance.getGraph().getUnitDemandGraph().getDemands().size();
    droneDists = std::vector<std::vector<double>>(n,std::vector<double>(n,-1));
    for (int i=0; i<n; ++i){
        for (int j=0; j<n; ++j){
            droneDists[i][j]=euclidianDistance(vertices[i].getPos(),vertices[j].getPos());
        }
    }

    switch (scenario)
    {
    case 1:
        return SolveCase1(instance);

    case 2:
        return SolveCase2(instance);

    case 3:
        return SolveCase3(instance);
    
    default:
        throw std::invalid_argument("scenario should be between 1 and 3 for neighborhoodSolve");
    }
}

Solution ConstructiveSolve::SolveCase1(Instance instance){
    x.push_back(0);
    for (int i=0; i<m; ++i){
        z.push_back(std::vector<std::vector<int>>(n,std::vector<int>(2,0)));
    }
    savings = std::vector<double>(n,-1);

    int isAmeliorating = 1;
    std::vector<int> actualX;

    std::cout << std::endl << "---> Constructing solution" << std::endl;
    greedyDemandAffectation(instance);
    calculateTotalCosts(instance);

    while (isAmeliorating == 1){
        isAmeliorating = 0;

        // std::cout << "tourney : " << std::endl;
        // for (int i=0; i<x.size(); ++i){
        //     std::cout << x[i] << " ";
        // }
        // std::cout << std::endl;

        //save temporarilly actual state
        actualX = x;

        double bestTotalCost = totalCost;
        std::vector<int> bestNeighborX = x;

        for (int i=0; i<n; ++i){
            int isInTruckTourney = 0;
            for (int j : x){
                if (i==j){
                    isInTruckTourney = 1;
                    break;
                } 
            }
            if (isInTruckTourney==0){
                x.push_back(i);
                greedyDemandAffectation(instance);
                calculateTotalCosts(instance);
                if (totalCost < bestTotalCost){
                    isAmeliorating = 1;
                    bestTotalCost = totalCost;
                    bestNeighborX = x;
                }
                x = actualX;
            }
        }
        x = bestNeighborX;
        greedyDemandAffectation(instance);
        calculateTotalCosts(instance);
    }

    return convertToSolutionCase01(instance);
}


//ToDo
Solution ConstructiveSolve::SolveCase2(Instance instance){
    return convertToSolutionCase01(instance);
}

Solution ConstructiveSolve::SolveCase3(Instance instance){
    return convertToSolutionCase01(instance);
}

void ConstructiveSolve::greedyDemandAffectation(const Instance& instance){
    std::vector<int> affected;

    std::vector<int> toAffect;
    const std::vector<Demand> demands = instance.getGraph().getUnitDemandGraph().getDemands();

    nodeDroneCost = std::vector<std::vector<double>>(n,std::vector<double>(2,0));
    std::vector<std::vector<int>> affectationNodeDrone(n,std::vector<int>(2,0));
    std::vector<double> nodeTruckDemandCost(n,0);
    nodeDemandCost = std::vector<double>(n,0);

    computeFullX(instance);

    for (int i=0; i<m; ++i){
        toAffect.push_back(i);
    }

    while(toAffect.size()>0){
        double minAffectTime = INT_MAX;
        int fastestDemandToAffect = -1;
        int fastestPointToAffect = -1;
        int affectationType = 0;

        for (int i=0; i<toAffect.size(); ++i){
            int demandID = toAffect[i];
            int demandNode = demands[demandID].getNodeGraphID();
            for (int j : fullX){
                int iDist = droneDists[demandNode][j];
                int droneID = 0;
                
                //consider the drone availability in the node j
                int bestDroneTime = nodeDroneCost[j][0];
                if (nodeDroneCost[j][1]<bestDroneTime){
                    droneID = 1;
                    bestDroneTime = nodeDroneCost[j][1];
                }

                if (affectationNodeDrone[j][droneID]!=0){
                    if (minAffectTime > (2*iDist+instance.getDroneRechargingTime()+bestDroneTime-nodeDemandCost[j])){
                        minAffectTime = 2*iDist+instance.getDroneRechargingTime()+bestDroneTime;
                        fastestDemandToAffect = i;
                        fastestPointToAffect = j;
                        affectationType = droneID;
                    }
                }else{
                    if (minAffectTime > (2*iDist+bestDroneTime-nodeDemandCost[j])){
                        minAffectTime = 2*iDist+bestDroneTime;
                        fastestDemandToAffect = i;
                        fastestPointToAffect = j;
                        affectationType = droneID;
                    }
                }

                if ((j==demandNode) && (minAffectTime > (instance.getTruckDeliveryTime()+nodeTruckDemandCost[j]-nodeDemandCost[j]))){
                    minAffectTime = instance.getTruckDeliveryTime();
                    fastestDemandToAffect = i;
                    fastestPointToAffect = j;
                    affectationType = 2;
                }
            }
        }

        affected.push_back(toAffect[fastestDemandToAffect]);
        z[toAffect[fastestDemandToAffect]] = std::vector<std::vector<int>>(n,std::vector<int>(2,0));
        if (affectationType!=2){
            z[toAffect[fastestDemandToAffect]][fastestPointToAffect][affectationType] = 1;
            nodeDroneCost[fastestPointToAffect][affectationType] += minAffectTime;
            affectationNodeDrone[fastestPointToAffect][affectationType] = 1;
        }else{
            nodeTruckDemandCost[fastestPointToAffect] += instance.getTruckDeliveryTime();
        }

        nodeDemandCost[fastestPointToAffect] = nodeTruckDemandCost[fastestPointToAffect];
        if (nodeDemandCost[fastestPointToAffect] < nodeDroneCost[fastestPointToAffect][0]){
            nodeDemandCost[fastestPointToAffect] = nodeDroneCost[fastestPointToAffect][0];
        }
        if (nodeDemandCost[fastestPointToAffect] < nodeDroneCost[fastestPointToAffect][1]){
            nodeDemandCost[fastestPointToAffect] = nodeDroneCost[fastestPointToAffect][1];
        }
        toAffect.erase(toAffect.begin()+fastestDemandToAffect);
    }

    if (affected.size()!=m){
        throw std::invalid_argument("something went wrong in greedyDemandAffectation");
    }
    // std::cout << "out greedy" << std::endl;
}

void ConstructiveSolve::computeFullX(const Instance& instance){
    fullX = std::vector<int>();
    for (int i=1; i<x.size(); ++i){
        fullX.push_back(x[i-1]);
        std::vector<int> path = instance.getGraph().getTSPKernelPath(x[i-1],x[i]);
        for (int nodePath = 1; nodePath<path.size()-1; ++nodePath){
            fullX.push_back(path[nodePath]);
        }
    }
    fullX.push_back(x[x.size()-1]);
    std::vector<int> path = instance.getGraph().getTSPKernelPath(x[x.size()-1],x[0]);
    for (int nodePath = 1; nodePath<path.size()-1; ++nodePath){
        fullX.push_back(path[nodePath]);
    }
}


// !!! vestiges of the past, keep out !!!

// void ConstructiveSolve::computeNodeDroneCosts(const Instance& instance){
//     computeFullX(instance);
//     nodeDroneCost = std::vector<std::vector<double>>(n,std::vector<double>(2,0));
//     for (int node : fullX){
//         for (int i = 0; i<instance.getGraph().getUnitDemandGraph().getDemands().size(); ++i){
//             if (z[i][node][0] == 1){
//                 nodeDroneCost[node][0] += 2*droneDists[node][instance.getGraph().getUnitDemandGraph().getDemand(i).getNodeGraphID()];
//             }

//             if (z[i][node][1] == 1){
//                 nodeDroneCost[node][1] += 2*droneDists[node][instance.getGraph().getUnitDemandGraph().getDemand(i).getNodeGraphID()];
//             }
//         }
//     }
// }

// void ConstructiveSolve::updateSavings(const Instance& instance){
//     for (int node=0; node<n; ++node){
//         int isNodeInTourney = 0;

//         // we don't consider nodes in the tourney
//         for (int j : x){
//            if (node==j){
//                 savings[node] = INT_MAX;
//                 isNodeInTourney = 1;
//                 break;
//            } 
//         }

//         // we aproximate the cost of adding the node to the tourney
//         if (isNodeInTourney==0){
//             // this part is the truck travel cost of adding node to the tourney
//             savings[node] = - truckTravelCost;
//             std::vector<int> newTravel = x;
//             newTravel.push_back(node);
//             newTravel = TSP2OPT(instance.getGraph().getTSPKernelTime(),newTravel);
//             for (int j=1; j<newTravel.size(); ++j){
//                 savings[node] += instance.getGraph().getTSPKernelTime(newTravel[j-1],newTravel[j]);
//             }
//             savings[node] += instance.getGraph().getTSPKernelTime(newTravel[0],newTravel[newTravel.size()-1]);

//             // but then we have to consider the potential lowering cost of new drones affectations
//             // if a demand is affected to a point further than node, then we deduce the difference to the savings
//             const std::vector<Demand> demands = instance.getGraph().getUnitDemandGraph().getDemands();
//             for (int demand=0; demand<m; ++demand){
//                 for (int k=0; k<n; ++k){
//                     if (z[demand][k][0]==1 || z[demand][k][1]==1){
//                         if (droneDists[demands[demand].getNodeGraphID()][k] > droneDists[demands[demand].getNodeGraphID()][node]){
//                             savings[node] -= (droneDists[demands[demand].getNodeGraphID()][k] - droneDists[demands[demand].getNodeGraphID()][node]);
//                         }
//                     }
//                 }
//             } 
//         }
//     }
// }

void ConstructiveSolve::calculateTotalCosts(const Instance& instance){
    totalCost = 0;
    computeFullX(instance);
    
    // for (int node = 0; node<fullX.size(); ++node){
    //     std::cout << fullX[node] << " ";
    // }
    // std::cout << "hey" << std::endl;

    for (int node = 0; node<fullX.size(); ++node){

        totalCost += nodeDemandCost[fullX[node]];

        if (node != fullX.size()-1){
            totalCost += instance.getGraph().getTSPKernelTime(fullX[node+1],fullX[node+1]);
        }else{
            totalCost += instance.getGraph().getTSPKernelTime(fullX[fullX.size()-1],fullX[0]);
        }

        // overhaul
        if (node != (fullX.size()-1)){
            for (int droneID = 0; droneID<2; ++droneID){
                int travelTime = 0;
                for (int futureNode = node; futureNode<fullX.size()-1; ++futureNode){
                    travelTime += instance.getGraph().getTSPKernelTime(fullX[futureNode],fullX[futureNode+1]);
                    if (nodeDroneCost[fullX[futureNode+1]][droneID]!=0){
                        if ((nodeDemandCost[fullX[futureNode]] + travelTime) 
                                <
                                (nodeDroneCost[fullX[futureNode]][droneID] + instance.getDroneRechargingTime())){
                                nodeDroneCost[fullX[futureNode+1]][droneID] 
                                += 
                                (nodeDroneCost[fullX[futureNode]][droneID] + instance.getDroneRechargingTime())
                                - 
                                (nodeDemandCost[fullX[futureNode]] + travelTime);
                                }
                        break;
                    }
                }
            }
        }
    }
}

Solution ConstructiveSolve::convertToSolutionCase01(Instance instance){
    int t = fullX.size();
    std::vector<std::vector<std::vector<int>>> convertorZ = std::vector<std::vector<std::vector<int>>>(t,std::vector<std::vector<int>>(m,std::vector<int>(2,0)));
    for (int period = 0; period<t; ++period){
        for (int d = 0; d<m; ++d){
            if (z[d][fullX[period]][0] == 1){
                convertorZ[period][d][0] = 1;
            }
            if (z[d][fullX[period]][1] == 1){
                convertorZ[period][d][1] = 1;
            }
        }
    }
    return convertCase01(instance,fullX,convertorZ);
}