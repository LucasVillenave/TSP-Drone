#ifndef UTILS
#define UTILS

#include "Solution/Solution.hpp"

double degree_to_meter(double x);

//return the id of the vertex of the given position
int closest(std::vector<Vertex> vertices, Position pos);

double euclidianDistance(Position p1,Position p2);
double euclidianDistance(double t_x_first, double t_y_first, double t_x_second, double t_y_second);

std::vector<std::vector<std::vector<int>>> updateDistMatrix (std::vector<std::vector<double>> & returnMatrix,
                                                               const std::vector<std::vector<Edge>>& adjacencyList, 
                                                               const std::vector<int>& vertices, Instance instance);

Solution convert(Instance i, std::vector<std::vector<int>> x, std::vector<std::vector<std::vector<int>>> z, std::vector<int> y, int scenario);

#endif