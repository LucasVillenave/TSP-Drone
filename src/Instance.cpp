#include "Instance.hpp"

Instance::Instance(Graph t_graph, std::string t_instanceName)
    : graph(std::move(t_graph)), instanceName(std::move(t_instanceName)) {}

Instance::Instance(std::vector<Vertex> t_vertices, std::vector<Edge> t_edges, std::string t_instanceName)
    : graph(Graph(std::move(t_vertices), std::move(t_edges))), instanceName(std::move(t_instanceName)) {}

void Instance::addDemands(const std::vector<Demand>& demands){
    graph.addDemands(demands);
}

void Instance::addDemand(Demand d){
    graph.addDemand(d);
}

const Graph & Instance::getGraph() const {
    return graph;
}