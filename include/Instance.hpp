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

        std::vector<std::string> roadTypes;
        std::vector<double> roadSpeed;

        double droneSpeed;

    public :
        Instance()=default;
        Instance(Graph t_graph, std::string t_instanceName, 
                 std::vector<std::string> t_roadTypes = std::vector<std::string>(), std::vector<double> t_roadSpeed = std::vector<double>(), double t_droneSpeed = (50/3.6));

        Instance(std::vector<Vertex> t_vertices, std::vector<Edge> t_edges, std::string t_instanceName,
                 std::vector<std::string> t_roadTypes = std::vector<std::string>(), std::vector<double> t_roadSpeed = std::vector<double>(), double t_droneSpeed = (50/3.6));

        void addDemands(const std::vector<Demand>& demands);
        void addDemand(Demand d);

        double getTravelTime(Edge e) const;
        int getDroneSpeed() const;

        const Graph & getGraph() const;
};

#endif