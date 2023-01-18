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


    public :
        Instance()=default;
        Instance(Graph t_graph, std::string t_instanceName);
        Instance(std::vector<Vertex> t_vertices, std::vector<Edge> t_edges, std::string t_instanceName);

        void addDemands(const std::vector<Demand>& demands);
        void addDemand(Demand d);

        const Graph & getGraph() const;
};

#endif