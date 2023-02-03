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
        z.push_back(std::vector<int>(n,0));
        z[i][0] = 1;
    }
    savings = std::vector<double>(n,-1);

    int isAmeliorating = 1;
    std::vector<int> actualX;

    while (isAmeliorating == 1){
        isAmeliorating = 0;
        updateSavings(instance);

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
                    double bestTotalCost = totalCost;
                    std::vector<int> bestNeighborX = x;
                }
                x = actualX;
            }
        }

        x = bestNeighborX;
        greedyDemandAffectation(instance);
        calculateTotalCosts(instance);
    }

    return convertToSolutionCase1(instance);
}


//ToDo
Solution ConstructiveSolve::SolveCase2(Instance instance){
    return convertToSolutionCase1(instance);
}

Solution ConstructiveSolve::SolveCase3(Instance instance){
    return convertToSolutionCase1(instance);
}

void ConstructiveSolve::greedyDemandAffectation(const Instance& instance){
    std::vector<int> affected;

    std::vector<int> toAffect;
    const std::vector<Demand> demands = instance.getGraph().getUnitDemandGraph().getDemands();

    nodeDroneCost = std::vector<std::vector<double>>(n,std::vector<double>(2,0));

    //demands in the tourney are served directly, others are to be delivered by drones
    for (int i=0; i<m; ++i){
        int isInTruckTourney = 0;
        for (int j : x){
           if (demands[i].getNodeGraphID()==j){
                affected.push_back(i);
                z[i] = std::vector<int>(n,0);
                z[i][j] = 1;
                isInTruckTourney = 1;
                break;
           } 
        }
        if (isInTruckTourney==0){
            toAffect.push_back(i);
        }
    }

    while(toAffect.size()>0){
        double minAffectDist = INT_MAX;
        int closestDemandToAffect = -1;
        int closestPointToAffect = -1;

        for (int i=0; i<toAffect.size(); ++i){
            int demandID = toAffect[i];
            int demandNode = demands[demandID].getNodeGraphID();
            for (int j : x){
                int iDist = droneDists[demandNode][j];
                
                //consider the drone availability in the node j
                int droneID = 0;
                int bestDroneTime = nodeDroneCost[j][0];
                if (nodeDroneCost[j][1]<bestDroneTime){
                    droneID = 1;
                    bestDroneTime = nodeDroneCost[j][1];
                }

                if (minAffectDist > (iDist+bestDroneTime)){
                    minAffectDist = iDist + bestDroneTime;
                    nodeDroneCost[j][droneID] += iDist + instance.getDroneRechargingTime();
                    closestDemandToAffect = i;
                    closestPointToAffect = j;
                }
            }
        }

        affected.push_back(toAffect[closestDemandToAffect]);
        z[toAffect[closestDemandToAffect]] = std::vector<int>(n,0);
        z[toAffect[closestDemandToAffect]][closestPointToAffect] = 1;
        toAffect.erase(toAffect.begin()+closestDemandToAffect);
    }

    if (affected.size()!=m){
        throw std::invalid_argument("something went wrong in greedyDemandAffectation");
    }
}

void ConstructiveSolve::updateSavings(const Instance& instance){
    for (int node=0; node<n; ++node){
        int isNodeInTourney = 0;

        // we don't consider nodes in the tourney
        for (int j : x){
           if (node==j){
                savings[node] = INT_MAX;
                isNodeInTourney = 1;
                break;
           } 
        }

        // we aproximate the cost of adding the node to the tourney
        if (isNodeInTourney==0){
            // this part is the truck travel cost of adding node to the tourney
            savings[node] = - truckTravelCost;
            std::vector<int> newTravel = x;
            newTravel.push_back(node);
            newTravel = TSP2OPT(instance.getGraph().getTSPKernelTime(),newTravel);
            for (int j=1; j<newTravel.size(); ++j){
                savings[node] += instance.getGraph().getTSPKernelTime(newTravel[j-1],newTravel[j]);
            }
            savings[node] += instance.getGraph().getTSPKernelTime(newTravel[0],newTravel[newTravel.size()-1]);

            // but then we have to consider the potential lowering cost of new drones affectations
            // if a demand is affected to a point further than node, then we deduce the difference to the savings
            const std::vector<Demand> demands = instance.getGraph().getUnitDemandGraph().getDemands();
            for (int demand=0; demand<m; ++demand){
                for (int k=0; k<n; ++k){
                    if (z[demand][k]==1){
                        if (droneDists[demands[demand].getNodeGraphID()][k] > droneDists[demands[demand].getNodeGraphID()][node]){
                            savings[node] -= (droneDists[demands[demand].getNodeGraphID()][k] - droneDists[demands[demand].getNodeGraphID()][node]);
                        }
                    }
                }
            } 
        }
    }
}

void ConstructiveSolve::calculateTotalCosts(const Instance& instance){

}

Solution ConstructiveSolve::convertToSolutionCase1(Instance instance){
    int t = x.size();
    return Solution(instance,std::vector<Event>());
}