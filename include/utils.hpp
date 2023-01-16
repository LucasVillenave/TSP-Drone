#ifndef UTILS
#define UTILS

#include "Graph/Graph.hpp"

//return the id of the vertex of the given position
int closest(std::vector<Vertex> vertices, Position pos);

double euclidianDistance(Position p1,Position p2);

std::vector<std::vector<std::vector<int>>> updateDistMatrix (std::vector<std::vector<int>> & returnMatrix,
                                                               const std::vector<std::vector<Edge>>& adjacencyList, 
                                                               const std::vector<int>& vertices);

#endif