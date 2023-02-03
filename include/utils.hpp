#ifndef UTILS
#define UTILS

#include "Solution/Solution.hpp"

double degree_to_meter(double x);

//return the id of the vertex of the given position
int closest(std::vector<Vertex> vertices, Position pos);

double euclidianDistance(Position p1,Position p2);
double euclidianDistanceBis(double t_x_first, double t_y_first, double t_x_second, double t_y_second);

std::vector<std::vector<std::vector<int>>> updateDistMatrix (std::vector<std::vector<double>> & returnMatrix,
                                                               const std::vector<std::vector<Edge>>& adjacencyList, 
                                                               const std::vector<int>& vertices, Instance instance);

//x_t,i,j the truck moving between i and j at time period t. z_t,d,a amount of demand d fullfilled by drones at time period t
Solution convertCase01(const Instance& i, const std::vector<int>& x, const std::vector<std::vector<std::vector<int>>>& z);

// interchange 2 edges such that the cost is reduced, does this until local minimum is reached.
// easily put it uncross edges until no edges cross anymore in the tourney.
std::vector<int> TSP2OPT(const std::vector<std::vector<double>>& distMastrix, std::vector<int> points);

#endif