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
}