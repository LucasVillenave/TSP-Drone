#ifndef UTILS
#define UTILS

#include "Solution/Solution.hpp"

//return the id of the vertex of the given position
int closest(std::vector<Vertex> vertices, Position pos);

double euclidianDistance(Position p1,Position p2);

std::vector<std::vector<std::vector<int>>> updateDistMatrix (std::vector<std::vector<double>> & returnMatrix,
                                                               const std::vector<std::vector<Edge>>& adjacencyList, 
                                                               const std::vector<int>& vertices, Instance instance);

Solution convertCase01(Instance i, std::vector<std::vector<int>> x, std::vector<std::vector<std::vector<int>>> z, int scenario);

#endif