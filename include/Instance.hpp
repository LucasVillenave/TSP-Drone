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

        int truckDeliveryTime = 60;

        double droneSpeed = 50/3.6;
        double droneRechargingTime = 30;

    public :
        Instance()=default;
        Instance(Graph t_graph, std::string t_instanceName);

        Instance(const std::vector<Vertex>& t_vertices, const std::vector<Edge>& t_edges, std::string t_instanceName);

        void addDemands(const std::vector<Demand>& demands);
        void addDemand(Demand d);

        Position getDepotLocation() const;
        int getDepotId() const;
        int getTruckDeliveryTime() const;
        static double getTravelTime(const Edge& e) ;
        double getDroneSpeed() const;
        double getDroneRechargingTime() const;

        const Graph& getGraph() const;
};

#endif
