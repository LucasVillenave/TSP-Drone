#ifndef GRAPH
#define GRAPH

#include "Graph/Vertex.hpp"
#include "Graph/Edge.hpp"

// there is ID and GraphID. 
// -- ID should be unique and unmovable. 
// -- GraphID correspond to the position of the item in the graph List and therefore access to the item in O(1)
// -- GraphID works best as long as the lists inside the graph aren't shuffled

class Instance;

class Graph{
    private :
        std::vector<Vertex> m_vertices;
        std::vector<Edge> m_edges;
        std::vector<Demand> m_demands;

        int m_number_of_demand_nodes;

        std::vector<std::vector<Edge>> adjacencyList;

        //warning, the adjacency matrix keep -1 if m_vertices aren't adjacent and the length of it's edge otherwise
        std::vector<std::vector<int>> adjacencyMatrix;

        std::vector<std::vector<double>> TSPKernelTime;
        std::vector<std::vector<std::vector<int>>> TSPKernelPath;

    public :
        // I made it so you can't modify the structure of the graph because the graph validity is checked in it's creation.
        // You must first create the graph with the m_vertices and m_edges and then add the m_demands
        Graph()=default;
        Graph(const std::vector<Vertex>& t_vertices, std::vector<Edge> t_edges);

        void kernelize(Instance instance);

        void addDemands(const std::vector<Demand>& t_demands);
        void addDemand(Demand d);

        Graph getUnitDemandGraph() const;

        int getNumberDemandNodes() const;

        const std::vector<std::vector<Edge>>& getAdjacencyList() const;
        const std::vector<Edge>& getAdjacencyList(int vertexID) const;

        const std::vector<std::vector<int>>& getAdjacencyMatrix() const;
        const std::vector<int>& getAdjacencyMatrix(int vertexID) const;

        const std::vector<Vertex>& getVertices() const;
        const Vertex& getVertice(int GraphID) const;

        const std::vector<Demand>& getDemands() const;
        const Demand& getDemand(int GraphID) const;

        const std::vector<Demand>& getDemandsUnit() const;
        const Demand& getDemandUnit(int GraphID) const;

        const std::vector<Edge>& getEdges() const;
        const Edge& getEdge(int GraphID) const;

        const std::vector<std::vector<double>>& getTSPKernelTime() const;
        const std::vector<std::vector<std::vector<int>>>& getTSPKernelPath() const;

        double getTSPKernelTime(int i, int j) const;
        const std::vector<int>& getTSPKernelPath(int i, int j) const;
};

inline std::ostream &operator<<(std::ostream &os, const Graph& g)
{
    os << "Graph of size " << g.getVertices().size() << " vertices " << g.getEdges().size() << " edges and " << g.getDemands().size() << " demands" << std::endl << std::endl;

    os << "Vertices : " << std::endl;
    for (const Vertex& v : g.getVertices()){
        os << v << std::endl;
    }

    os << std::endl << "Edges : " << std::endl;
    for (const Edge& e : g.getEdges()){
        os << e << std::endl;
    }

    os << std::endl << "Demands : " << std::endl;
    for (const Demand& d : g.getDemands()){
        os << d << std::endl;
    }

    return os;
}

#endif