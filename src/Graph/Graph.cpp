#include "Graph/Graph.hpp"
#include "utils.hpp"
#include <stdexcept>
#include <iostream>

Graph::Graph(){}

Graph::Graph(std::vector<Vertex> vertices, std::vector<Edge> edges){

    for (int i=0; i<vertices.size(); ++i){
        vertices[i].setGraphID(i);
    }

    for (int i=0; i<edges.size(); ++i){
        edges[i].setGraphID(i);

        if ((edges[i].getStartID()<0 || edges[i].getStartID()>=vertices.size() ||
             edges[i].getEndID()<0 || edges[i].getEndID()>=vertices.size())){
            throw std::invalid_argument("Bad vertices index on edges");
        }
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
    d.setGraphID(demands.size());
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