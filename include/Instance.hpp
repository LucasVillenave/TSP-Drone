#ifndef INSTANCE
#define INSTANCE

#include <vector>
#include <string>

#include "Graph/Graph.hpp"

class Instance{

    private :
        std::string instanceName;
        Graph graph;


    public :
        Instance();
        Instance(Graph g, std::string instanceName);
        Instance(std::vector<Vertex> vertices, std::vector<Edge> edges, std::string instanceName);

        void addDemands(std::vector<Demand> demands);
        void addDemand(Demand d);

        const Graph & getGraph() const;
};

#endif