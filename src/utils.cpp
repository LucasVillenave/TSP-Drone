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

double euclidianDistance(double t_x_first, double t_y_first, double t_x_second, double t_y_second)
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
        throw std::invalid_argument("invalid argument size in diskstra");
    }

    if (adjacencyList.size()!=vertices.size()){
        throw std::invalid_argument("invalid argument size in diskstra");
    }

    for (int i=0; i<n; i++){
        if(returnMatrix[i].size()!=n){
            throw std::invalid_argument("invalid argument size in diskstra");
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

void convertDroneCase1(const Instance& instance, std::vector<std::vector<std::vector<int>>> z, std::vector<Event>& eventList, int startingNode, double time){
    int n = instance.getGraph().getVertices().size();
    int m = instance.getGraph().getDemands().size();
    const std::vector<Vertex>& vertices = instance.getGraph().getVertices();

    for (int a=0; a<1; a++){
        for (int d=0; d<m; ++d){
            const std::vector<Demand>& demands = instance.getGraph().getDemands();
            Vertex t = vertices[demands[d].getNodeGraphID()];
            if (z[startingNode][d][a]==1){
                eventList.push_back(Event(vertices[startingNode].getPos(),time,2,t.getPos(),m,a));
                time += euclidianDistance(vertices[startingNode].getPos(),t.getPos())/instance.getDroneSpeed();
                eventList.push_back(Event(t.getPos(),time,5,Position(),m,a));
                time += euclidianDistance(vertices[startingNode].getPos(),t.getPos())/instance.getDroneSpeed();
                eventList.push_back(Event(vertices[startingNode].getPos(),time,3,Position(),-1,a));
            }
        }
    }
}

void sortByTime(std::vector<Event>& eventList){
    std::vector<double> times;
    std::vector<Event> sortedList;
    for (Event e : eventList){
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

Solution convertCase01(const Instance& instance, std::vector<std::vector<std::vector<int>>> x, std::vector<std::vector<std::vector<int>>> z, int scenario){
    std::vector<Event> eventList;
    double time = 0;

    int n = instance.getGraph().getVertices().size();
    if (z.size()!=n){
        throw std::invalid_argument("invalid sized arguments");
    }
    for (int i=0; i<n; ++i){
        if (z[i].size()!=n){
            throw std::invalid_argument("invalid sized arguments");
        }
        for (int j=0; j<n; ++j){
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

    for (std::vector<std::vector<int>> xt : x){
        for (int i=0; i<n; ++i){
            for (int j=0; j<n; ++j){
                if (xt[i][j]==1){
                    std::vector<int> path = instance.getGraph().getTSPKernelPath(i,j);
                    for(int k=1; k<path.size(); k++){
                        eventList.push_back(Event(vertices[path[k-1]].getPos(),time,0,vertices[path[k]].getPos()));
                        time += instance.getGraph().getTSPKernelDist(path[k-1],path[k]);
                        eventList.push_back(Event(vertices[path[k]].getPos(),time,1));
                        convertDroneCase1(instance,z,eventList,vertices[path[k]].getGraphID(),time);
                    }
                    for (Demand d : instance.getGraph().getVertice(j).getDemands()){
                        for (int kl=0; kl<d.getAmount(); ++kl){
                            eventList.push_back(Event(d.getNodePos(),time,4,Position(),d.getGraphID()));
                            time += instance.getTruckDeliveryTime();
                        }
                    }
                }
            }
        }
    }

    sortByTime(eventList);

    return Solution(instance,std::vector<Event>());
}