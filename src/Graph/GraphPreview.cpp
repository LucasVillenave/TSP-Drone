#include "Graph/GraphPreview.hpp"


class GraphPreview : public Preview {

    const Graph m_graph;

    void write_tikz() override;

public:
    GraphPreview(Graph t_graph, const std::string& t_filename);

};

GraphPreview::GraphPreview(Graph t_graph, const std::string& t_filename)
    : Preview(t_filename), m_graph(std::move(t_graph))
{
    set_minimum(m_graph.getVertices());
}

void graphPreview(const Graph& t_graph, const std::string& t_filename)
{
    GraphPreview(t_graph, t_filename)();
}

void GraphPreview::write_tikz()
{
    m_file << "\\begin{tikzpicture}[xscale=1]\n";
    write_edges(m_graph.getEdges(), m_graph.getVertices());
    write_vertices(m_graph.getVertices());
    new_line();
}