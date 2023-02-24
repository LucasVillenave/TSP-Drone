#include "ConstructiveSolve.hpp"
#include <climits>

ConstructiveSolve::ConstructiveSolve(){}

Solution ConstructiveSolve::Solve(int scenario,Instance instance){

    const std::vector<Vertex>& vertices = instance.getGraph().getVertices();
    n = vertices.size();
    Graph gc = instance.getGraph(); 
    m = gc.getUnitDemandGraph().getDemands().size();
    droneDists = std::vector<std::vector<double>>(n,std::vector<double>(n,-1));
    for (int i=0; i<n; ++i){
        for (int j=0; j<n; ++j){
            droneDists[i][j]=distance(vertices[i].getPos(),vertices[j].getPos());
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
    x.push_back(instance.getGraph().getVertice(instance.getDepotId()).getGraphID());
    savings = std::vector<double>(n,-1);

    int isAmeliorating = 1;
    std::vector<int> actualX;

    std::cout << std::endl << "---> Constructing solution" << std::endl;
    greedyDemandAffectation(instance);
    calculateTotalCosts(instance);

    while (isAmeliorating == 1){
        isAmeliorating = 0;

        std::cout << "tourney : ";
        for (int i=0; i<x.size(); ++i){
            std::cout << x[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "total time " << totalCost << std::endl;

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

    greedyDemandAffectation(instance);
    calculateTotalCosts(instance);

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
    // std::cout << "in greedy" << std::endl;
    std::vector<int> affected(m,0);

    std::vector<int> toAffect;
    Graph gc = instance.getGraph();
    const std::vector<Demand> demands = gc.getUnitDemandGraph().getDemands();
    
    computeFullX(instance);

    std::vector<std::vector<int>> affectationNodeDrone(fullX.size(),std::vector<int>(2,0));
    std::vector<double> nodeTruckDemandCost(fullX.size(),0);
    std::vector<double> truckCovering(fullX.size(),0);
    nodeDroneCost = std::vector<std::vector<double>>(fullX.size(),std::vector<double>(2,0));
    nodeDemandCost = std::vector<double>(fullX.size(),0);
    z = std::vector<std::vector<std::vector<int>>>(m,std::vector<std::vector<int>>(fullX.size(),std::vector<int>(2,0)));

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
            for (int j = 0; j<fullX.size(); ++j){
                int iDist = droneDists[demandNode][fullX[j]];
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

                if (truckCovering[j]==0){
                    if ((fullX[j]==demandNode) && (minAffectTime > (instance.getTruckDeliveryTime()+nodeTruckDemandCost[j]-nodeDemandCost[j]))){
                        minAffectTime = instance.getTruckDeliveryTime();
                        fastestDemandToAffect = i;
                        fastestPointToAffect = j;
                        affectationType = 2;
                    }
                }else{
                    if (fullX[j]==demandNode){
                        minAffectTime = 0;
                        fastestDemandToAffect = i;
                        fastestPointToAffect = j;
                        affectationType = 2;
                        break;
                    }
                }
            }
        }

        affected[toAffect[fastestDemandToAffect]]+=1;
        if (affectationType!=2){
            z[toAffect[fastestDemandToAffect]][fastestPointToAffect][affectationType] = 1;
            nodeDroneCost[fastestPointToAffect][affectationType] += minAffectTime;
            affectationNodeDrone[fastestPointToAffect][affectationType] = 1;
        }else{
            nodeTruckDemandCost[fastestPointToAffect] += instance.getTruckDeliveryTime();
            truckCovering[fastestPointToAffect] = 1;
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

    for (int i=0; i<m; ++i){
        if (affected[i]!=1){
            std::cout << "demand " << i << " affected " << affected[i] << " times" << std::endl;
            throw std::invalid_argument("something went wrong in greedyDemandAffectation");
        }
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

void ConstructiveSolve::calculateTotalCosts(const Instance& instance){
    totalCost = 0;
    computeFullX(instance);
    
    // for (int node = 0; node<fullX.size(); ++node){
    //     std::cout << fullX[node] << " ";
    // }
    // std::cout << "hey" << std::endl;

    for (int node = 0; node<fullX.size(); ++node){
        // std::cout << "cost of demands of node " << fullX[node] << " : " << nodeDemandCost[node] << std::endl;

        totalCost += nodeDemandCost[node];

        if (node != fullX.size()-1){
            // std::cout << "travel between " << fullX[node] << " and " << fullX[node+1] << " : " << instance.getGraph().getTSPKernelTime(fullX[node+1],fullX[node+1]) << std::endl;
            // std::cout << "travel between " << 25 << " and " << 31 << " : " << instance.getGraph().getTSPKernelTime(25,31) << " : " << instance.getGraph().getTSPKernelTime(31,25) << std::endl;
            totalCost += instance.getGraph().getTSPKernelTime(fullX[node+1],fullX[node]);
        }else{
            // std::cout << "travel between " << fullX[fullX.size()-1] << " and " << fullX[0] << " : " << instance.getGraph().getTSPKernelTime(fullX[fullX.size()-1],fullX[0]) << std::endl;
            totalCost += instance.getGraph().getTSPKernelTime(fullX[fullX.size()-1],fullX[0]);
        }

        // overhaul
        if (node != (fullX.size()-1)){
            for (int droneID = 0; droneID<2; ++droneID){
                int travelTime = 0;
                for (int futureNode = node; futureNode<fullX.size()-1; ++futureNode){
                    travelTime += instance.getGraph().getTSPKernelTime(fullX[futureNode],fullX[futureNode+1]);
                    if (nodeDroneCost[futureNode+1][droneID]!=0){
                        if ((nodeDemandCost[futureNode] + travelTime) 
                                <
                                (nodeDroneCost[futureNode][droneID] + instance.getDroneRechargingTime())){
                                nodeDemandCost[futureNode+1]
                                += 
                                (nodeDroneCost[futureNode][droneID] + instance.getDroneRechargingTime())
                                - 
                                (nodeDemandCost[futureNode] + travelTime);
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
    Graph gc = instance.getGraph();
    std::vector<std::vector<std::vector<int>>> convertorZ = std::vector<std::vector<std::vector<int>>>(t,std::vector<std::vector<int>>(m,std::vector<int>(2,0)));
    for (int d = 0; d<m; ++d){
        int nb=0;
        for (int period = 0; period<t; ++period){
            if (z[d][period][0] == 1){
                nb++;
                convertorZ[period][d][0] = 1;
            }
            if (z[d][period][1] == 1){
                nb++;
                convertorZ[period][d][1] = 1;
            }
        }
        if (nb>gc.getUnitDemandGraph().getDemand(d).getAmount()){
            std::cout << "in demand " << d << " affected " << nb << " times with drones when " << instance.getGraph().getDemand(d).getAmount() << " is it's amount" << std::endl; 
        }
    }
    // std::cout << "fullX : " << std::endl;
    // for (int i=0; i<t; ++i){
    //     std::cout << fullX[i] << " ";
    // }
    std::cout << std::endl;
    return convertCase01(instance,fullX,convertorZ);
}