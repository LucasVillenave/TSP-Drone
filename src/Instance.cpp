#include "Instance.hpp"
#include <iostream>

Instance::Instance(Graph t_graph, std::string t_instanceName, 
                   std::vector<std::string> t_roadTypes, std::vector<double> t_roadSpeed, double t_droneSpeed)
    : graph(std::move(t_graph)), instanceName(std::move(t_instanceName)), roadTypes(std::move(t_roadTypes)), roadSpeed(std::move(t_roadSpeed)), droneSpeed(std::move(t_droneSpeed)) {
        
        if ((roadSpeed.size()-1)!=roadTypes.size()){
            std::cout << roadSpeed.size() << " " << roadTypes.size() << std::endl;
            throw std::invalid_argument("wrong road types/speeds size");
        }
        graph.kernelize(*this);
    }

Instance::Instance(std::vector<Vertex> t_vertices, std::vector<Edge> t_edges, std::string t_instanceName,
                   std::vector<std::string> t_roadTypes, std::vector<double> t_roadSpeed, double t_droneSpeed)
    : graph(Graph(std::move(t_vertices), std::move(t_edges))), instanceName(std::move(t_instanceName)), roadTypes(std::move(t_roadTypes)), roadSpeed(std::move(t_roadSpeed)), droneSpeed(std::move(t_droneSpeed)) {
        
        if ((roadSpeed.size()-1)!=roadTypes.size()){
            std::cout << roadSpeed.size() << " " << roadTypes.size() << std::endl;
            throw std::invalid_argument("wrong road types/speeds size");
        }
        
        graph.kernelize(*this);
    }

void Instance::addDemands(const std::vector<Demand>& demands){
    graph.addDemands(demands);
}

void Instance::addDemand(Demand d){
    graph.addDemand(d);
}

int Instance::getTruckDeliveryTime() const{
    return truckDeliveryTime;
} 

const Graph & Instance::getGraph() const {
    return graph;
}

int Instance::getDroneSpeed() const {
    return droneSpeed;
}

double Instance::getTravelTime(Edge e) const{
    for (int i=0; i<roadTypes.size();++i){
        std::string roadType = roadTypes[i];
        if (roadType==e.getRoadType()){
            return (e.getLength()/roadSpeed[i]);
        }
    }
    return e.getLength()/roadSpeed[roadSpeed.size()-1];
}

double Instance::getDroneRechargingTime() const{
    return droneRechargingTime;
}