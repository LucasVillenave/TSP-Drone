#include "Solution/SolutionPreview.hpp"

class SolutionPreview : public Preview {

    const Solution m_solution;

    void write_tikz();

public:
    explicit SolutionPreview(Solution t_solution);

};


SolutionPreview::SolutionPreview(Solution t_solution)
        : Preview("solution"), m_solution(std::move(t_solution))
{
    set_minimum(m_solution.getVertices());
}

void solutionPreview(Solution t_solution)
{
    SolutionPreview(std::move(t_solution))();
}

void SolutionPreview::write_tikz()
{
    m_file << "\\begin{tikzpicture}[xscale=" << m_xscale << "]\n";
    write_edges(m_solution.getEdges(), m_solution.getVertices());
    write_vertices(m_solution.getVertices());
    new_line();
}
