#ifndef GRAPH
#define GRAPH

#include "Graph/Vertex.hpp"
#include "Graph/Edge.hpp"

// there is ID and GraphID. 
// -- ID should be unique and unmovable. 
// -- GraphID correspond to the position of the item in the graph List and therefore access to the item in O(1)
// -- GraphID works best as long as the lists inside the graph aren't shuffled

class Graph{
    private :
        std::vector<Vertex> vertices;
        std::vector<Edge> edges;
        std::vector<Demand> demands;

        std::vector<std::vector<Edge>> adjacencyList;
        std::vector<std::vector<int>> adjacencyMatrix;

        std::vector<std::vector<int>> TSPKernelDist;
        std::vector<std::vector<std::vector<int>>> TSPKernelPath;

        int isKernelized=0;

        void kernelize();

    public :
        // I made it so you can't modify the structure of the graph because the graph validity is checked in it's creation.
        // You must first create the graph with the vertices and edges and then add the demands
        Graph();
        Graph(std::vector<Vertex> vertices, std::vector<Edge> edges);

        void addDemands(std::vector<Demand> demands);
        void addDemand(Demand d);

        const std::vector<std::vector<Edge>>& getAdjacencyList() const;
        const std::vector<Edge>& getAdjacencyList(int vertexID) const;

        const std::vector<std::vector<int>>& getAdjacencyMatrix() const;
        const std::vector<int>& getAdjacencyMatrix(int vertexID) const;

        const std::vector<Vertex>& getVertices() const;
        const Vertex& getVertices(int GraphID) const;

        const std::vector<Demand>& getDemands() const;
        const Demand& getDemand(int GraphID) const;

        const std::vector<Edge>& getEdges() const;
        const Edge& getEdge(int GraphID) const;

        const std::vector<std::vector<int>>& getTSPKernelDist();
        const std::vector<std::vector<std::vector<int>>>& getTSPKernelPath();

        int getTSPKernelDist(int i, int j);
        const std::vector<int>& getTSPKernelPath(int i, int j);
};

inline std::ostream &operator<<(std::ostream &os, Graph g)
{
    os << "Graph of size " << g.getVertices().size() << " vertices " << g.getEdges().size() << " edges and " << g.getDemands().size() << " demands" << std::endl << std::endl;

    os << "Vertices : " << std::endl;
    for (Vertex v : g.getVertices()){
        os << v << std::endl;
    }

    os << std::endl << "Edges : " << std::endl;
    for (Edge e : g.getEdges()){
        os << e << std::endl;
    }

    os << std::endl << "Demands : " << std::endl;
    for (Demand d : g.getDemands()){
        os << d << std::endl;
    }

    return os;
}

#endif