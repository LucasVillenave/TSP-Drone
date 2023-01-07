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

    public :
        // I made it so you can't create an empty graph because the graph validity is checked in it's creation.
        // You must first create the graph with the vertices and edges and then add the demands
        Graph(std::vector<Vertex> vertices, std::vector<Edge> edges);

        void addDemands(std::vector<Demand> demands);
        void addDemand(Demand d);

        const std::vector<Vertex>& getVertices();
        const Vertex& getVertice(int GraphID);
        const Vertex& getVertice(int ID);

        const std::vector<Demand>& getDemands();
        const Demand& getDemand(int ID);
        const Demand& getDemand(int GraphID);

        const std::vector<Edge>& getEdges();
        const Edge& getEdge(int GraphID);
        const Edge& getEdge(int ID);


};

#endif