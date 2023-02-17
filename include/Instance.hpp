#ifndef INSTANCE
#define INSTANCE

#include <vector>
#include <string>
#include <utility>

#include "Graph/Graph.hpp"

class Instance{

    private :
        std::string instanceName;
        Graph graph;

        Position depot_location = Position(44.8500102, 0.5370699);
        int depot_id = -1;

        std::vector<std::string> roadTypes;
        std::vector<double> roadSpeed = std::vector<double>(1,1);
        int truckDeliveryTime = 60;

        double droneSpeed;
        double droneRechargingTime = 30;

    public :
        Instance()=default;
        Instance(Graph t_graph, std::string t_instanceName, 
                 std::vector<std::string> t_roadTypes = std::vector<std::string>(), std::vector<double> t_roadSpeed = std::vector<double>(1,1), double t_droneSpeed = (50/3.6));

        Instance(std::vector<Vertex> t_vertices, std::vector<Edge> t_edges, std::string t_instanceName,
                 std::vector<std::string> t_roadTypes = std::vector<std::string>(), std::vector<double> t_roadSpeed = std::vector<double>(1,1), double t_droneSpeed = (50/3.6));

        void addDemands(const std::vector<Demand>& demands);
        void addDemand(Demand d);

        Position getDepotLocation() const;
        int getDepotId() const;
        int getTruckDeliveryTime() const;
        double getTravelTime(Edge e) const;
        int getDroneSpeed() const;
        double getDroneRechargingTime() const;

        const Graph& getGraph() const;
};

#endif