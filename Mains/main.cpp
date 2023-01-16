#include "Solution/Solution.hpp"
#include "parser.hpp"
#include "Solution/preview.hpp"
#include <iostream>

int main(){
    Instance instance = load("../Data/","init");
    Solution solution = Solution(instance, {});
    preview(solution);
    system("pdflatex solution.tex");
    system("xdg-open solution.pdf");

    Graph g = instance.getGraph();
    std::cout << g << std::endl;
    /*
    std::vector<std::vector<int>> tkd = g.getTSPKernelDist();
    for (int i=0; i<tkd[0].size(); ++i){
        std::cout << "tkd 0 -> " << i << " : " <<tkd[0][i] << std::endl;
    }
    for (int i=0; i<tkd[126].size(); ++i){
        std::cout << "tkd 126 -> " << i << " : " <<tkd[126][i] << std::endl;
    }
    for (int i=0; i<125; ++i){
        for (int j=126; j<tkd.size(); ++j){
            if (tkd[j][i]<INT_MAX){
                std::cout << "gg at " << " -> " << tkd[j][i] << std::endl;
            }
        }
    }
    for (Edge e : g.getEdges()){
        int i = e.getEndID();
        if (i>=126){
            if (e.getStartID()<=125){
                std::cout << "y en as une" << std::endl;
            }
        }else{
            if (e.getStartID()>=126){
                std::cout << "y en as une" << std::endl;
            }
        }
    }
     */
}