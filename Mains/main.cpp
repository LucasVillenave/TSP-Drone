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
    std::cout << g << std::endl;
    // graphPreview(g);
    // system("pdflatex graph.tex");
    // system("xdg-open graph.pdf");
}