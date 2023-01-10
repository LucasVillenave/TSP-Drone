#include "Solution.hpp"
#include "parser.hpp"
#include <iostream>

int main(){
    Instance i = load("../Data/","init");
    std::cout << i.getGraph() << std::endl;
}