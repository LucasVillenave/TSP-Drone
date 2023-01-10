#include "utils.hpp"
#include <tgmath.h>
#include <climits>
#include <iostream>

int closest(std::vector<Vertex> vertices, Position pos){
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
}

double euclidianDistance(Position p1,Position p2){
    double deltaLon = (p1.getLongitude()-p2.getLongitude())*100000;
    deltaLon = deltaLon*deltaLon;
    double deltaLat = (p1.getLatitude()-p2.getLatitude())*100000;
    deltaLat = deltaLat*deltaLat;
    return std::sqrt(
           deltaLat+deltaLon
           );
}