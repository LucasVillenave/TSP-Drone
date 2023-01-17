#include "Graph/GraphPreview.hpp"


class GraphPreview : public Preview {

    const Graph m_graph;

    void write_tikz() override;

public:
    explicit GraphPreview(Graph t_graph);

};

GraphPreview::GraphPreview(Graph t_graph)
    : Preview("graph"), m_graph(std::move(t_graph))
{
    set_minimum(m_graph.getVertices());
}

void graphPreview(Graph t_graph)
{
    GraphPreview(std::move(t_graph))();
}

void GraphPreview::write_tikz()
{
    m_file << "\\begin{tikzpicture}[xscale=" << m_xscale << "]\n";
    write_edges(m_graph.getEdges(), m_graph.getVertices());
    write_vertices(m_graph.getVertices());
    new_line();
}