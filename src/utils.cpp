#include "utils.hpp"
#include <tgmath.h>
#include <climits>
#include <iostream>

int closest(std::vector<Vertex> vertices, Position pos){
    /*
    int min = INT_MAX;
    int returnID = -1;
    for (int i = 0; i<vertices.size(); ++i){
        Vertex v = vertices[i];
        int dist = euclidianDistance(v.getPos(), pos);
        if (dist < min){
            min = dist;
            returnID = i;
        }
    }
    return returnID;
     */
    for(unsigned int v=0, n=vertices.size(); v<n; ++v)
    {
        if(vertices[v].getPos()==pos)
            return v;
    }
}

double euclidianDistance(Position p1,Position p2){
    double deltaLon = (p1.getLongitude()-p2.getLongitude());
    deltaLon = deltaLon*deltaLon;
    double deltaLat = (p1.getLatitude()-p2.getLatitude());
    deltaLat = deltaLat*deltaLat;
    return std::sqrt(
           deltaLat+deltaLon
           );
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
                                         const std::vector<int>& vertices)
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
            if(optimalDist[eDestination] > (optimalDist[actualVertex]+e.getLength())){
                optimalDist[eDestination] = optimalDist[actualVertex]+e.getLength();
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
                                                               const std::vector<int>& vertices)
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
        optimalPath.push_back(dijkstra(i, returnMatrix, adjacencyList, vertices));
    }
    std::cout << "----> dijkstra at 100% finished" << std::endl;

    return optimalPath;
}