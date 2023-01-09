#include "Instance.hpp"

Instance::Instance(){}
Instance::Instance(Graph g, std::string instanceName){
    this->instanceName = instanceName;
    this->graph = g;
}

Instance::Instance(std::vector<Vertex> vertices, std::vector<Edge> edges, std::string instanceName){
    this->instanceName = instanceName;
    this->graph = Graph(vertices,edges);
}

void Instance::addDemands(std::vector<Demand> demands){
    this->graph.addDemands(demands);
}

void Instance::addDemand(Demand d){
    this->graph.addDemand(d);
}

const Graph& Instance::getGraph(){
    return this->graph;
}