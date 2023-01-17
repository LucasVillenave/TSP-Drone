#include "Solution/Solution.hpp"
#include "parser.hpp"
#include "Solution/preview.hpp"
#include <iostream>

int main(){
    Instance instance = load("../Data/","init");
    Solution solution = Solution(instance, {});
    // preview(solution);
    // system("pdflatex solution.tex");
    // system("xdg-open solution.pdf");

    Graph g = instance.getGraph();
    std::cout << g << std::endl;
    std::vector<std::vector<int>> dm = g.getAdjacencyMatrix();
    for (int i=0; i<dm[0].size(); ++i){
        std::cout << "tkd 0 -> " << i << " : " <<dm[0][i] << std::endl;
    }
}