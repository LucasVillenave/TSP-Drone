#ifndef UTILS
#define UTILS

#include "Graph/Graph.hpp"

//return the id of the vertex of the given position
int closest(std::vector<Vertex> vertices, Position pos);

int euclidianDistance(Position p1,Position p2);

#endif