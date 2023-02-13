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

int isToBePlacedBefore(int eventType, int toPlaceEventType){
    if (eventType == 0){
        eventType = 10;
    }
    if (eventType == 3){
        eventType = 5;
    }else{
        if (eventType == 5){
            eventType = 3;
        }
    }

    if (toPlaceEventType == 0){
        toPlaceEventType = 10;
    }
    if (toPlaceEventType == 3){
        toPlaceEventType = 5;
    }else{
        if (toPlaceEventType == 5){
            toPlaceEventType = 3;
        }
    }

    if (toPlaceEventType<eventType){
        return 1;
    }
    return 0;
}

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
                if ((times[i]<(eTime + 0.000001))&&(times[i]>(eTime - 0.000001))){
                    int toBePlacedBefore = isToBePlacedBefore(sortedList[i].getEventType(), e.getEventType());
                    if (toBePlacedBefore==1){
                        asBeenPlaced=1;
                        times.insert(times.begin()+i, eTime);
                        sortedList.insert(sortedList.begin()+i,e);
                        break;
                    }
                }
                if ((times[i-1] < (eTime + 0.000001)) && (times[i] > (eTime + 0.000001))){
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

Solution convertCase01(const Instance& instance, const std::vector<int>& x, const std::vector<std::vector<std::vector<int>>>& z){
    std::vector<Event> eventList;
    double time = 0;
    std::vector<double> dronesTimes(2,0);

    std::cout << "---> convertor from (x,z) to event solution" << std::endl;
    std::cout << "------> checking input" << std::endl;

    int n = instance.getGraph().getUnitDemandGraph().getVertices().size();
    int m = instance.getGraph().getUnitDemandGraph().getDemands().size();

    if (z.size() != x.size()){
        throw std::invalid_argument("invalid sized arguments z");
    }

    for (int i=0; i<z.size(); ++i){
        if (z[i].size()!=m){
            std::cout << "got " << z[i].size() << " instead of " << m << std::endl;
            throw std::invalid_argument("invalid sized arguments z[i]");
        }
        for (int j=0; j<m; ++j){
            if (z[i][j].size()!=2){
                throw std::invalid_argument("invalid sized arguments z[i][j]");
            }
        }
    }
    
    const std::vector<Vertex>& vertices = instance.getGraph().getVertices();
    std::vector<int> y = calculateTruckCouverture(z,instance.getGraph().getUnitDemandGraph().getDemands());

    std::cout << "------> generating events" << std::endl;

    for (int t=0; t<x.size(); ++t){
        int nextNode = x[t];
        std::vector<std::vector<int>> zt = z[t];

        if (t!=0){
            int actualNode = x[t-1];

            std::vector<int> path = instance.getGraph().getTSPKernelPath(actualNode,nextNode);

            //add the events of the implicit passing of the truck (possibly just i->j)
            for(int k=1; k<path.size(); k++){
                // std::cout << "from " << path[k-1] << " to " << path[k] << " : " << vertices[path[k-1]].getPos() << " -> " << vertices[path[k]].getPos() << std::endl;
                eventList.push_back(Event(vertices[path[k-1]].getPos(),time,0,vertices[path[k]].getPos()));
                time += instance.getGraph().getTSPKernelTime(path[k-1],path[k]);
                eventList.push_back(Event(vertices[path[k]].getPos(),time,1));
            }
        }

        //overHauls
        if (time>dronesTimes[0]){
            dronesTimes[0] = time;
        }
        if (time>dronesTimes[1]){
            dronesTimes[1] = time;
        }

        //we add all event of covered demands
        for (int d=0; d<m; ++d){
            Demand demand = instance.getGraph().getUnitDemandGraph().getDemand(d);
            //by drones
            for (int a=0; a<2; ++a){
                for (int k=0; k<zt[d][a]; ++k){
                    eventList.push_back(Event(vertices[nextNode].getPos(),dronesTimes[a],2,demand.getNodePos(),demand.getID(),a));
                    dronesTimes[a] += euclidianDistance(vertices[nextNode].getPos(),demand.getNodePos())/instance.getDroneSpeed();
                    eventList.push_back(Event(demand.getNodePos(),dronesTimes[a],5,Position(),demand.getID(),a));
                    dronesTimes[a] += euclidianDistance(vertices[nextNode].getPos(),demand.getNodePos())/instance.getDroneSpeed();
                    eventList.push_back(Event(vertices[nextNode].getPos(),dronesTimes[a],3,Position(),-1,a));
                    dronesTimes[a]+=instance.getDroneRechargingTime();
                }
            }
            //by truck
            if (demand.getNodeGraphID()==nextNode){
                for (int k=0; k<y[d]; ++k){
                    eventList.push_back(Event(demand.getNodePos(),time,4,Position(),demand.getID()));
                    time += instance.getTruckDeliveryTime();
                }
            }
        }

        //Truck awaits drones if needed
        if (time+instance.getDroneRechargingTime()<dronesTimes[0]){
            time = dronesTimes[0]-instance.getDroneRechargingTime();
        }
        if (time+instance.getDroneRechargingTime()<dronesTimes[1]){
            time = dronesTimes[1]-instance.getDroneRechargingTime();
        }
    }

    std::vector<int> path = instance.getGraph().getTSPKernelPath(x[x.size()-1],x[0]);
    for(int k=1; k<path.size(); k++){
        eventList.push_back(Event(vertices[path[k-1]].getPos(),time,0,vertices[path[k]].getPos()));
        time += instance.getGraph().getTSPKernelTime(path[k-1],path[k]);
        eventList.push_back(Event(vertices[path[k]].getPos(),time,1));
    }

    sortByTime(eventList);

    std::cout << "-----> printing solution events" << std::endl << std::endl;
    for (Event e : eventList){
        std::cout << e << std::endl;
    }
    std::cout << std::endl << "-----> conversion done" << std::endl;



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