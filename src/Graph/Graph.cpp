#include "Graph/Graph.hpp"
#include "Utils.hpp"
#include <stdexcept>
#include <iostream>

Graph::Graph(){}

Graph::Graph(std::vector<Vertex> vertices, std::vector<Edge> edges){
    for (int i=0; i<edges.size(); ++i){
        edges[i].setGraphID(i);

        // test if vertices of the edge exists
        int startc = 0;
        int endc = 0;
        Vertex start = edges[i].getStart();
        Vertex end = edges[i].getEnd();
        for (Vertex v : vertices){
            if (v==start){
                ++startc;
            }
            if (v==end){
                ++endc;
            }
        }
        if (!(startc==1 && endc==1)){
            throw std::invalid_argument("Edges and Vertices don't match");
        }
    }

    for (int i=0; i<vertices.size(); ++i){
        vertices[i].setGraphID(i);
    }
    this->vertices = vertices;
    this->edges = edges;
}

void Graph::addDemands(std::vector<Demand> demands){
    for (Demand d : demands){
        this->addDemand(d);
    }
}

void Graph::addDemand(Demand d){
    int closestID = closest(vertices,d.getPos());
    vertices[closestID].addDemand(d);
    demands.push_back(d);
}

const std::vector<Vertex>& Graph::getVertices(){
    return vertices;
}

const Vertex& Graph::getVertice(int GraphID){
    return vertices[GraphID];
}

const std::vector<Demand>& Graph::getDemands(){
    return demands;
}

const Demand& Graph::getDemand(int GraphID){
    return demands[GraphID];
}

const std::vector<Edge>& Graph::getEdges(){
    return edges;
}

const Edge& Graph::getEdge(int GraphID){
    return edges[GraphID];
}