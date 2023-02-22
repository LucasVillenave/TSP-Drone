#include <iostream>
#include "Solution/SolutionParser.hpp"

int main(int argc, char** argv){
    if(argc < 3) {
        throw std::runtime_error("Usage: ./toCheck <INSTANCE_PATH> <SOLUTION_FILE>");
    }
    std::string instance_path = argv[1];
    std::string instance_name = instance_path.substr(0, instance_path.size()-1);
    size_t pos = 0;
    while ((pos = instance_name.find('/')) != std::string::npos) {
        instance_name.erase(0, pos + 1);
    }
    instance_path = instance_path.substr(0, instance_path.size()-instance_name.size()-1);
    std::string solution_file = argv[2];

    Solution solution = solution_parse(instance_path, instance_name, solution_file);
    std::cout << "Solution check : " << std::endl;
    for(unsigned int i = 0, n = solution.getIsValid().size(); i < n; ++i)
        std::cout << "\tCas " << i << " : " << solution.getIsValid(i) << std::endl;
}