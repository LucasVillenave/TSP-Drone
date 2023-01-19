#ifndef PARSER
#define PARSER

#include "Solution/Solution.hpp"

Instance load(const std::string& instancePath, const std::string& instanceName);

void write(Solution sol);

#endif