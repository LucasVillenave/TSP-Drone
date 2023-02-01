#ifndef PARSER
#define PARSER

#include "Solution/Solution.hpp"

std::vector<std::string> split(std::string s, char delim);

Instance load(const std::string& instancePath, const std::string& instanceName);

#endif