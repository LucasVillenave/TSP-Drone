#include "Solution/Solution.hpp"
#include "parser.hpp"
#include "Solution/SolutionPreview.hpp"
#include "Graph/GraphPreview.hpp"
#include <iostream>

int main(){
    Instance instance = load("../Data/","init");
    Solution solution = Solution(instance, {});
    // solutionPreview(solution);
    // system("pdflatex solution.tex");
    // system("xdg-open solution.pdf");

    Graph g = instance.getGraph();
    // graphPreview(g);
    // system("pdflatex graph.tex");
    // system("xdg-open graph.pdf");
    /*
    std::cout << g << std::endl;
    std::vector<std::vector<int>> dm = g.getAdjacencyMatrix();
    for (int i=0; i<dm[0].size(); ++i){
        std::cout << "tkd 0 -> " << i << " : " <<dm[0][i] << std::endl;
    }
     */
}