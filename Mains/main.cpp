#include "Solution/Solution.hpp"
#include "parser.hpp"
#include "Solution/SolutionPreview.hpp"
#include "Graph/GraphPreview.hpp"
#include <iostream>
#include "Mod1V1.hpp"
#include "Heur_Cas1_leP.hpp"

int main(){
    Instance instance = load("../Data/","100");
    Solution solution = Solution(instance, {});
    // solutionPreview(solution);
    // system("pdflatex solution.tex");
    // system("xdg-open solution.pdf");

    Graph g = instance.getGraph();
    std::cout << g << std::endl;
    // graphPreview(g);
    // system("pdflatex graph.tex");
    // system("xdg-open graph.pdf");


    // // init
    // std::vector<int> ordre = {2,12,21,13,11,19,20,17,5,1,16,4,6,15,18,3,14,9,10,7,8};
    // // 30
    // std::vector<int> ordre = {14, 10, 4, 9, 7, 13, 21, 18, 5, 1, 6, 29, 24, 23, 27, 2, 12, 28, 11, 25, 20, 16, 30, 3, 17, 8, 22, 15, 26, 19};
    // // 50
    // std::vector<int> ordre = {20, 36, 43, 35, 30, 21, 18, 33, 40, 8, 13, 31, 14, 38, 15, 41, 26, 39, 5, 10, 46, 17, 3, 37, 4, 25, 29, 24, 45, 16, 11, 44, 23, 42, 27, 7, 48, 9, 34, 12, 28, 6, 32, 19, 2, 47, 1, 22};
    // // 80
    // std::vector<int> ordre = {18, 34, 64, 45, 73, 65, 19, 41, 57, 55, 33, 44, 22, 13, 66, 16, 61, 8, 28, 30, 40, 62, 9, 48, 2, 59, 46, 47, 36, 26, 54, 4, 58, 69, 67, 63, 25, 71, 10, 53, 35, 72, 29, 7, 17, 3, 43, 68, 56, 24, 52, 42, 12, 31, 21, 27, 70, 11, 1, 14, 15, 50, 51, 60, 20, 37, 38, 23, 6, 5, 39, 49, 32};
    // 100
    // std::vector<int> ordre = {35, 37, 59, 11, 12, 68, 27, 83, 73, 40, 23, 64, 81, 75, 70, 22, 46, 76, 48, 25, 4, 56, 54, 3, 84, 65, 30, 50, 58, 41, 2, 14, 1, 33, 31, 6, 55, 74, 61, 57, 69, 21, 15, 80, 44, 49, 13, 45, 72, 34, 5, 51, 20, 26, 18, 8, 63, 62, 42, 17, 53, 87, 29, 16, 47, 82, 86, 28, 36, 77, 78, 66, 24, 32, 19, 39, 43, 38, 60, 67, 7, 9, 10, 71, 52, 85, 79};

    // la spéciale de bazile
    // std::vector<int> ordre = {10,4,13,9,7,14,16,5,18,21,1,6,29,23,15,20,27,25,2,28,12,11,30,17,8,22,3,26,19,24};

    std::vector<int> ordre={};

    for (int d=0; d<ordre.size(); ++d){
        ordre[d] -= 1;
    }
    Heur_Cas1_leP heuris(ordre);
    heuris.solve(instance);

    
}