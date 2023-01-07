#include "Utils.hpp"
#include <tgmath.h>
#include <climits>

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

int euclidianDistance(Position p1,Position p2){
    return std::sqrt(
           (p1.getLongitude()-p2.getLongitude())*(p1.getLongitude()-p2.getLongitude())+
           (p1.getLatitude()-p2.getLatitude())*(p1.getLatitude()-p2.getLatitude())
           );
}