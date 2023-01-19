#include "Instance.hpp"

Instance::Instance(Graph t_graph, std::string t_instanceName, 
                   std::vector<std::string> t_roadTypes, std::vector<double> t_roadSpeed)
    : graph(std::move(t_graph)), instanceName(std::move(t_instanceName)), roadTypes(std::move(t_roadTypes)), roadSpeed(std::move(t_roadSpeed)) {}

Instance::Instance(std::vector<Vertex> t_vertices, std::vector<Edge> t_edges, std::string t_instanceName,
                   std::vector<std::string> t_roadTypes, std::vector<double> t_roadSpeed)
    : graph(Graph(std::move(t_vertices), std::move(t_edges))), instanceName(std::move(t_instanceName)), roadTypes(std::move(t_roadTypes)), roadSpeed(std::move(t_roadSpeed)) {}

void Instance::addDemands(const std::vector<Demand>& demands){
    graph.addDemands(demands);
}

void Instance::addDemand(Demand d){
    graph.addDemand(d);
}

const Graph & Instance::getGraph() const {
    return graph;
}

double Instance::getTravelTime(Edge e){
    for (int i=0; i<roadTypes.size();++i){
        std::string roadType = roadTypes[i];
        if (roadType==e.getRoadType()){
            return (e.getLength()/roadSpeed[i]);
        }
    }
    return -1;
}