#include "Graph/Graph.hpp"
#include "utils.hpp"
#include <stdexcept>
#include <iostream>

Graph::Graph(){}

Graph::Graph(std::vector<Vertex> vertices, std::vector<Edge> edges){

    for (int i=0; i<vertices.size(); ++i){
        vertices[i].setGraphID(i);
        adjacencyMatrix.push_back(std::vector<Edge>());
        TSPKernelDist.push_back(std::vector<int>(vertices.size(),-1));
        // for (int j=0; j<vertices.size(); ++j){

        // }
    }

    for (int i=0; i<edges.size(); ++i){
        edges[i].setGraphID(i);

        adjacencyMatrix[edges[i].getStartID()].push_back(edges[i]);
        adjacencyMatrix[edges[i].getEndID()].push_back(edges[i]);

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

const std::vector<Vertex>& Graph::getVertices() const{
    return vertices;
}

const Vertex& Graph::getVertices(int GraphID){
    return vertices[GraphID];
}

const std::vector<Demand>& Graph::getDemands() const{
    return demands;
}

const Demand& Graph::getDemand(int GraphID){
    return demands[GraphID];
}

const std::vector<Edge>& Graph::getEdges() const{
    return edges;
}

const Edge& Graph::getEdge(int GraphID){
    return edges[GraphID];
}

void Graph::kernelize(){
    std::vector<int> TMPvertices;
    for (int i=0; i<vertices.size();++i){
        TMPvertices.push_back(i);
    }
    TSPKernelPath = updateDistMatrix(TSPKernelDist,adjacencyMatrix,TMPvertices);
}

const std::vector<std::vector<int>>& Graph::getTSPKernelDist(){
    if (isKernelized==0){
        isKernelized=1;
        kernelize();
    }
    return TSPKernelDist;
}

const std::vector<std::vector<std::vector<int>>>& Graph::getTSPKernelPath(){
    if (isKernelized==0){
        isKernelized=1;
        kernelize();
    }
    return TSPKernelPath;
}

int Graph::getTSPKernelDist(int i, int j){
    if (isKernelized==0){
        isKernelized=1;
        kernelize();
    }
    return TSPKernelDist[i][j];
}

const std::vector<int>& Graph::getTSPKernelPath(int i, int j){
    if (isKernelized==0){
        isKernelized=1;
        kernelize();
    }
    return TSPKernelPath[i][j];
}