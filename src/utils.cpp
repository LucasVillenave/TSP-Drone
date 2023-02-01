#include "utils.hpp"
#include <tgmath.h>
#include <climits>
#include <iostream>

double degree_to_meter(double x)
{
    // 1' = 1852m
    return 60*1852*x;
}

int closest(std::vector<Vertex> vertices, Position pos){
    int min = INT_MAX;
    int returnID = -1;
    for (int i = 0; i<vertices.size(); ++i){
        Vertex v = vertices[i];
        double dist = euclidianDistance(v.getPos(), pos);
        if (dist < min){
            min = dist;
            returnID = i;
        }
    }
    return returnID;
}

double euclidianDistance(Position p1,Position p2){
    double deltaLon = (p1.getX()-p2.getX());
    deltaLon = deltaLon*deltaLon;
    double deltaLat = (p1.getY()-p2.getY());
    deltaLat = deltaLat*deltaLat;
    return std::sqrt(
           deltaLat+deltaLon
           );
}

double euclidianDistanceBis(double t_x_first, double t_y_first, double t_x_second, double t_y_second)
{
    double delta_lon = degree_to_meter(t_x_first - t_x_second),
            delta_lat = degree_to_meter(t_y_first - t_y_second);
    delta_lon = delta_lon*delta_lon;
    delta_lat = delta_lat*delta_lat;
    return std::sqrt(delta_lon + delta_lat);
}

int getNextVertex(const std::vector<double>& optimalDist, const std::vector<int>& isOpt){
    int minID = -1;
    int min = INT_MAX;
    for (int i=0; i<optimalDist.size(); i++){
        if (isOpt[i]==0){
            if (optimalDist[i]<min){
                min = optimalDist[i];
                minID = i;
            }
        }
    }
    return minID;
}

std::vector<std::vector<int>> dijkstra(int source, 
                                         std::vector<std::vector<double>> & returnMatrix,
                                         const std::vector<std::vector<Edge>>& adjacencyList, 
                                         const std::vector<int>& vertices,
                                         Instance instance)
{
    std::vector<std::vector<int>> optimalPath;
    std::vector<int> isOpt;
    std::vector<double> optimalDist;

    int n = vertices.size();

    for(int i=0; i<n; i++){
        optimalPath.push_back(std::vector<int>());
        optimalDist.push_back(INT_MAX);
        isOpt.push_back(0);
    }
    optimalDist[source] = 0;

    int actualVertex = source;
    int isAllOpt=0;

    while(!isAllOpt){
        isOpt[actualVertex] = 1;
        for (Edge e : adjacencyList[actualVertex]){
            int eDestination;
            if (e.getStartID()!=actualVertex){
                eDestination=e.getStartID();
            }else{
                eDestination=e.getEndID();
            }
            if(optimalDist[eDestination] > (optimalDist[actualVertex]+instance.getTravelTime(e))){
                optimalDist[eDestination] = optimalDist[actualVertex]+instance.getTravelTime(e);
                optimalPath[eDestination] = optimalPath[actualVertex];
                optimalPath[eDestination].push_back(vertices[actualVertex]);
            }
        }
        actualVertex = getNextVertex(optimalDist, isOpt);

        isAllOpt = (actualVertex==-1);
    }
    for(int i=0; i<n; i++){
        optimalPath[i].push_back(vertices[i]);
    }

    returnMatrix[source] = optimalDist;
    return optimalPath;
}

std::vector<std::vector<std::vector<int>>> updateDistMatrix (std::vector<std::vector<double>> & returnMatrix,
                                                               const std::vector<std::vector<Edge>>& adjacencyList, 
                                                               const std::vector<int>& vertices, Instance instance)
 {
    std::cout << "----> updating distMatrix" << std::endl;
    std::vector<std::vector<std::vector<int>>> optimalPath;
    int n = vertices.size();

    if (returnMatrix.size()!=vertices.size()){
        throw std::invalid_argument("invalid argument size in dijkstra");
    }

    if (adjacencyList.size()!=vertices.size()){
        throw std::invalid_argument("invalid argument size in dijkstra");
    }

    for (int i=0; i<n; i++){
        if(returnMatrix[i].size()!=n){
            throw std::invalid_argument("invalid argument size in dijkstra");
        }
    }
    int scale = (n/10)+1;

    std::cout << "----> starting dijkstra" << std::endl;
    for (int i=0; i<n; i++){
        if ((i%scale) == 0) {
            float fi = i;
            float fn = n;
            float pct = (fi*100)/fn;
            std::cout << "----> dijkstra at " << pct << "% finished" << std::endl;
        }
        optimalPath.push_back(dijkstra(i, returnMatrix, adjacencyList, vertices, instance));
    }
    std::cout << "----> dijkstra at 100% finished" << std::endl;

    return optimalPath;
}

// void convertDroneCase1(const Instance& instance, std::vector<std::vector<std::vector<int>>> z, std::vector<Event>& eventList, int startingNode, double & time){
//     int n = instance.getGraph().getVertices().size();
//     int m = instance.getGraph().getUnitDemandGraph().getDemands().size();
//     const std::vector<Vertex>& vertices = instance.getGraph().getVertices();

//     for (int a=0; a<1; a++){
//         for (int d=0; d<m; ++d){
//             const std::vector<Demand>& demands = instance.getGraph().getUnitDemandGraph().getDemands();
//             Vertex t = vertices[demands[d].getNodeGraphID()];
//             if (z[startingNode][d][a]==1){
//                 eventList.push_back(Event(vertices[startingNode].getPos(),time,2,t.getPos(),m,a));
//                 time += euclidianDistance(vertices[startingNode].getPos(),t.getPos())/instance.getDroneSpeed();
//                 eventList.push_back(Event(t.getPos(),time,5,Position(),m,a));
//                 time += euclidianDistance(vertices[startingNode].getPos(),t.getPos())/instance.getDroneSpeed();
//                 eventList.push_back(Event(vertices[startingNode].getPos(),time,3,Position(),-1,a));
//             }
//         }
//     }
// }

void sortByTime(std::vector<Event>& eventList){
    std::vector<double> times;
    std::vector<Event> sortedList;
    if (eventList.size()!=0){
        times.push_back(eventList[0].getTime());
        sortedList.push_back(eventList[0]);
    }
    for (int i=1; i<eventList.size();++i){
        Event e = eventList[i];
        double eTime = e.getTime();
        int asBeenPlaced = 0;
        if (times[0]>eTime){
            times.insert(times.begin(),eTime);
            sortedList.insert(sortedList.begin(),e);
        }else{
            for (int i=1; i<times.size(); ++i){
                if ((times[i-1]<eTime) && (times[i]>eTime)){
                    asBeenPlaced=1;
                    times.insert(times.begin()+i,eTime);
                    sortedList.insert(sortedList.begin()+i,e);
                    break;
                }
            }
        }
        if (asBeenPlaced==0){
            times.push_back(eTime);
            sortedList.push_back(e);
        }
    }
    eventList = sortedList;
}


std::vector<int> calculateTruckCouverture(const std::vector<std::vector<std::vector<int>>>& z, const std::vector<Demand>& demands){
    std::vector<int> y;
    for (Demand d : demands){
        y.push_back(d.getAmount());
    }

    for (std::vector<std::vector<int>> zt : z){
        for (int d=0; d<y.size(); ++d){
            y[d] -= zt[d][0];
            y[d] -= zt[d][1];
        }
    }

    for (int d=0; d<y.size(); ++d){
        if (y[d]<0){
            throw std::invalid_argument("invalid z");
        }
    }

    return y;
}

Solution convertCase01(const Instance& instance, const std::vector<std::vector<std::vector<int>>>& x, const std::vector<std::vector<std::vector<int>>>& z){
    std::vector<Event> eventList;
    double time = 0;

    std::cout << "---> convertor from (x,z) to event solution" << std::endl;
    std::cout << "------> checking input" << std::endl;

    int n = instance.getGraph().getVertices().size();
    int m = instance.getGraph().getDemands().size();

    if (z.size() != x.size()){
        throw std::invalid_argument("invalid sized arguments");
    }

    for (int i=0; i<z.size(); ++i){
        if (z[i].size()!=m){
            throw std::invalid_argument("invalid sized arguments");
        }
        for (int j=0; j<m; ++j){
            if (z[i][j].size()!=2){
                throw std::invalid_argument("invalid sized arguments");
            }
        }
    }

    for (std::vector<std::vector<int>> xt : x){
        if (xt.size()!=n){
            throw std::invalid_argument("invalid sized arguments");
        }
        for (int i=0; i<xt.size(); ++i){
            if (xt[i].size()!=n){
                throw std::invalid_argument("invalid sized arguments");
            }
        }

        int xLeave = 0;
        for (int i=0; i<n; ++i){
            for (int j=0; j<n; ++j){
                if (xt[i][j]==1){
                    xLeave++;
                }
            }
        }
        if (xLeave>1){
            throw std::invalid_argument("invalid x");
        }
    }
    
    const std::vector<Vertex>& vertices = instance.getGraph().getVertices();
    std::vector<int> y = calculateTruckCouverture(z,instance.getGraph().getDemands());

    std::cout << "------> generating events" << std::endl;

    for (int t=0; t<x.size(); ++t){
        std::vector<std::vector<int>> xt = x[t];
        std::vector<std::vector<int>> zt = z[t];
        for (int i=0; i<n; ++i){
            for (int j=0; j<n; ++j){
                if (xt[i][j]==1){
                    std::vector<int> path = instance.getGraph().getTSPKernelPath(i,j);
                    //add the events of the implicit passing of the truck (possibly just i->j)
                    for(int k=1; k<path.size(); k++){
                        // std::cout << "from " << path[k-1] << " to " << path[k] << " : " << vertices[path[k-1]].getPos() << " -> " << vertices[path[k]].getPos() << std::endl;
                        eventList.push_back(Event(vertices[path[k-1]].getPos(),time,0,vertices[path[k]].getPos()));
                        time += instance.getGraph().getTSPKernelTime(path[k-1],path[k]);
                        eventList.push_back(Event(vertices[path[k]].getPos(),time,1));
                    }

                    //we add all event of covered demands
                    for (int d=0; d<m; ++d){
                        Demand demand = instance.getGraph().getDemand(d);
                        //by drones
                        for (int a=0; a<2; ++a){
                            for (int k=0; k<zt[d][a]; ++k){
                                eventList.push_back(Event(vertices[j].getPos(),time,2,demand.getNodePos(),d,a));
                                time += euclidianDistance(vertices[j].getPos(),demand.getNodePos())/instance.getDroneSpeed();
                                eventList.push_back(Event(demand.getNodePos(),time,5,Position(),d,a));
                                time += euclidianDistance(vertices[j].getPos(),demand.getNodePos())/instance.getDroneSpeed();
                                eventList.push_back(Event(vertices[j].getPos(),time,3,Position(),-1,a));
                            }
                        }

                        //by truck
                        if (demand.getNodeGraphID()==j){
                            for (int k=0; k<y[d]; ++k){
                                eventList.push_back(Event(demand.getNodePos(),time,4,Position(),demand.getGraphID()));
                                time += instance.getTruckDeliveryTime();
                            }
                        }
                    }
                }
            }
        }
    }

    sortByTime(eventList);

    // for (Event e : eventList){
    //     std::cout << e << std::endl;
    // }

    return Solution(instance,eventList);
}

std::vector<int> TSP2OPT(const std::vector<std::vector<double>>& distMastrix, std::vector<int> points){
    if (points.size() < 3){
        return points;
    }
    int isAmeliorating = 1;
    while(isAmeliorating){
        isAmeliorating = 0;
        for (int i=1; i<points.size()-1; i++){
            for (int j=i+1; j<points.size(); j++){
                if (isAmeliorating==0){
                    int p1 = points[i];
                    int p2 = points[j];

                    int predecesseurP1 = points[i-1];
                    int successeurP2 = points[j+1];

                    if (j==(points.size()-1)){
                        successeurP2 = points[0];
                    }

                    int partialCost = 0;
                    partialCost += distMastrix[predecesseurP1][p2];
                    partialCost -= distMastrix[predecesseurP1][p1];

                    partialCost += distMastrix[p1][successeurP2];
                    partialCost -= distMastrix[p2][successeurP2];

                    if (partialCost<0){
                        for (int k=0; k<(((j-i)/2)+1); k++){
                            int tmp = points[i+k];
                            points[i+k] = points[j-k];
                            points[j-k] = tmp;
                        }
                        isAmeliorating = 1;
                    }
                }
            }
        }
    }
    return points;
}