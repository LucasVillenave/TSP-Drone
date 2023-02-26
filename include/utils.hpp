#ifndef UTILS
#define UTILS

#include "Solution/Solution.hpp"

double degree_to_meter(double x);

//return the id of the vertex of the given position
int closest(std::vector<Vertex> vertices, Position pos);

double distance(Position t_first, Position t_second);

std::vector<std::vector<std::vector<int>>> updateDistMatrix (std::vector<std::vector<double>> & returnMatrix,
                                                               const std::vector<std::vector<Edge>>& adjacencyList, 
                                                               const std::vector<int>& vertices, Instance instance);

//x_t the truck moving between x_t-1 and x_t at time period t.
//z_t,d,a binnary of unitary demand d being fullfilled by drones a at time period t
Solution convertCase01(const Instance& i, const std::vector<int>& x, const std::vector<std::vector<std::vector<int>>>& z);

// interchange 2 edges such that the cost is reduced, does this until local minimum is reached.
// easily put it uncross edges until no edges cross anymore in the tourney.
std::vector<int> TSP2OPT(const std::vector<std::vector<double>>& distMastrix, std::vector<int> points);

#endif