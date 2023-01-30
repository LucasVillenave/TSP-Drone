#ifndef VRP_DRONE_SOLUTIONPARSER_H
#define VRP_DRONE_SOLUTIONPARSER_H

#include <vector>
#include <fstream>
#include <iostream>
#include "Solution/Solution.hpp"
#include "parser.hpp"

Solution solution_parse(const std::string& t_instance_path, const std::string& t_instance_name, const std::string& t_solution_path);

#endif //VRP_DRONE_SOLUTIONPARSER_H
