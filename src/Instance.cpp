#include "Instance.hpp"
#include <iostream>

int find_depot_id(const Graph& graph, Position position)
{
    for(const Vertex& vertex : graph.getVertices())
    {
        if(vertex.getPos() == position)
            return vertex.getGraphID();
    }
    throw std::invalid_argument("no depot");
}

Instance::Instance(Graph t_graph, std::string t_instanceName)
    : graph(std::move(t_graph)), instanceName(std::move(t_instanceName)) {
        depot_id = find_depot_id(graph, depot_location);
        graph.kernelize(*this);
    }

Instance::Instance(const std::vector<Vertex>& t_vertices, const std::vector<Edge>& t_edges, std::string t_instanceName)
    : graph(Graph(t_vertices, t_edges)), instanceName(std::move(t_instanceName)) {
        depot_id = find_depot_id(graph, depot_location);
        graph.kernelize(*this);
    }

void Instance::addDemands(const std::vector<Demand>& demands){
    graph.addDemands(demands);
}

void Instance::addDemand(Demand d){
    graph.addDemand(d);
}

Position Instance::getDepotLocation() const
{
    return depot_location;
}

int Instance::getDepotId() const
{
    return depot_id;
}

int Instance::getTruckDeliveryTime() const{
    return truckDeliveryTime;
} 

const Graph& Instance::getGraph() const {
    return graph;
}

double Instance::getDroneSpeed() const {
    return droneSpeed;
}

double Instance::getTravelTime(const Edge& e) {
    return e.getCost();
}

double Instance::getDroneRechargingTime() const{
    return droneRechargingTime;
}