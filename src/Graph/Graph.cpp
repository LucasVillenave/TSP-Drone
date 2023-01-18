#include "Graph/Graph.hpp"
#include "utils.hpp"
#include <stdexcept>
#include <iostream>

Graph::Graph(std::vector<Vertex> t_vertices, std::vector<Edge> t_edges) : vertices(t_vertices)
{

    for (int i=0; i < t_vertices.size(); ++i){
        vertices[i].setGraphID(i);
        adjacencyList.emplace_back();
        adjacencyMatrix.emplace_back(t_vertices.size(), 0);
        TSPKernelDist.emplace_back(t_vertices.size(), -1);
    }

    for (int i=0; i < t_edges.size(); ++i){
        t_edges[i].setGraphID(i);

        if (adjacencyMatrix[t_edges[i].getStartID()][t_edges[i].getEndID()] == 0){
            adjacencyMatrix[t_edges[i].getStartID()][t_edges[i].getEndID()] = 1;

            adjacencyList[t_edges[i].getStartID()].push_back(t_edges[i]);
            adjacencyList[t_edges[i].getEndID()].push_back(t_edges[i]);

            edges.push_back(t_edges[i]);
        }

        if ((t_edges[i].getStartID() < 0 || t_edges[i].getStartID() >= t_vertices.size() ||
             t_edges[i].getEndID() < 0 || t_edges[i].getEndID() >= t_vertices.size())){
            throw std::invalid_argument("Bad t_vertices index on t_edges");
        }
    }
    kernelize();
}

void Graph::addDemands(const std::vector<Demand>& t_demands){
    for (const Demand& d : t_demands){
        addDemand(d);
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

const Vertex& Graph::getVertices(int GraphID) const{
    return vertices[GraphID];
}

const std::vector<Demand>& Graph::getDemands() const{
    return demands;
}

const Demand& Graph::getDemand(int GraphID) const{
    return demands[GraphID];
}

const std::vector<Edge>& Graph::getEdges() const{
    return edges;
}

const Edge& Graph::getEdge(int GraphID) const{
    return edges[GraphID];
}

void Graph::kernelize(){
    std::vector<int> TMPvertices;
    for (int i=0; i<vertices.size();++i){
        TMPvertices.push_back(i);
    }
    TSPKernelPath = updateDistMatrix(TSPKernelDist,adjacencyList,TMPvertices);
}

const std::vector<std::vector<double>>& Graph::getTSPKernelDist() const{
    return TSPKernelDist;
}

const std::vector<std::vector<std::vector<int>>>& Graph::getTSPKernelPath() const{
    return TSPKernelPath;
}

double Graph::getTSPKernelDist(int i, int j) const{
    return TSPKernelDist[i][j];
}

const std::vector<int>& Graph::getTSPKernelPath(int i, int j) const{
    return TSPKernelPath[i][j];
}

const std::vector<std::vector<Edge>>& Graph::getAdjacencyList() const{
    return adjacencyList;
}

const std::vector<Edge>& Graph::getAdjacencyList(int vertexID) const{
    return adjacencyList[vertexID];
}

const std::vector<std::vector<int>>& Graph::getAdjacencyMatrix() const{
    return adjacencyMatrix;
}

const std::vector<int>& Graph::getAdjacencyMatrix(int vertexID) const{
    return adjacencyMatrix[vertexID];
}