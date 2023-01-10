#include "Solution.hpp"
#include "Parser.hpp"
#include <iostream>

int main(){
    Instance i = load("../Data/","init");
    std::cout << i.getGraph() << std::endl;
}