#include "Solution/Solution.hpp"
#include "parser.hpp"
#include "Solution/SolutionPreview.hpp"
#include "Graph/GraphPreview.hpp"
#include <iostream>
#include "Mod1V1.hpp"

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

    // std::vector<std::vector<double>> tkd = g.getTSPKernelTime();
    // std::vector<std::vector<std::vector<int>>> tkp = g.getTSPKernelPath();
    // for (int i=0; i<tkd[0].size();++i){
    //     std::cout << "tkd[0][" << i << "] : " << tkd[0][i] << std::endl; 
    // }
    // for (int i=0; i<tkp[0].size();++i){
    //     for (int j=0; j<tkp[0][i].size(); ++j){
    //         std::cout << " -> " << tkp[0][i][j];
    //     }
    //     std::cout << std::endl; 
    // }

    std::cout << g.getNbr_noeuds_demandes() << std::endl;
    std::cout << g.getNbr_demandes_unitaires() << std::endl;
    std::cout << g.getDemandsUnit().size() << std::endl;

    // Mod1V1 Model(600);
    // Model.solve(instance);
}